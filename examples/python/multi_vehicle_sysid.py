#!/usr/bin/env python3
"""
Example: multi-vehicle SYSID disambiguation using pymavlink.

Multiple vehicles on one link get demultiplexed into per-(sysid, compid)
state, tracked for liveness, and each discovered autopilot is targeted
with a command in turn -- demonstrating that traffic from many vehicles
sharing one link can be told apart and addressed individually.
"""

import time
import logging
import argparse
import threading
from collections import deque
from dataclasses import dataclass, field
from typing import Dict, Tuple, List, Optional

from pymavlink import mavutil

MAX_ACK_QUEUE = 5
HEARTBEAT_WARN_TIMEOUT = 2.0
HEARTBEAT_DROP_TIMEOUT = 5.0


@dataclass
class ComponentState:
    sysid: int
    compid: int
    last_heartbeat: Optional[float] = None
    autopilot: int = mavutil.mavlink.MAV_AUTOPILOT_INVALID
    lat: float = 0.0
    lon: float = 0.0
    alt: float = 0.0
    ack_queue: deque = field(default_factory=lambda: deque(maxlen=MAX_ACK_QUEUE))


class MavlinkComponentTracker:
    """Demultiplexes MAVLink traffic on a single link into per-(sysid,
    compid) state, so that individual vehicles/components can be
    disambiguated and addressed individually.

    This does not implement MAVLink routing (forwarding messages between
    links) -- it only demuxes and tracks state on one incoming stream.
    """

    def __init__(
        self,
        connection_string: str,
        source_system: int = 255,
        source_component: int = mavutil.mavlink.MAV_COMP_ID_MISSIONPLANNER,
    ):
        self.connection_string = connection_string

        # Our own identity on the link. Used to filter out our own
        # traffic instead of assuming everyone else uses sysid 255 -
        # that's just a convention, not part of the spec.
        self.source_system = source_system
        self.source_component = source_component

        self.master = None
        self.components: Dict[Tuple[int, int], ComponentState] = {}
        self._lock = threading.Lock()
        self._running = False
        self._router_thread = None
        self._monitor_thread = None
        self.logger = logging.getLogger("MavlinkComponentTracker")

    def _connect(self):
        """Establish MAVLink connection using our own sysid/compid."""
        self.logger.info("Connecting to %s", self.connection_string)
        self.master = mavutil.mavlink_connection(
            self.connection_string,
            source_system=self.source_system,
            source_component=self.source_component,
        )
        self.master.wait_heartbeat()
        self.logger.info("Connected. Heartbeat received.")

    def _reconnect(self):
        """Attempt to reconnect on socket error."""
        self.logger.warning("Reconnecting...")
        try:
            self._connect()
        except Exception as e:
            self.logger.error("Reconnect failed: %s", e)

    def _message_loop(self):
        """Main loop: receive and dispatch MAVLink messages.

        self.master is re-read from `self` (not cached locally) on every
        iteration, so a reconnect that swaps it in from another thread is
        picked up on the very next pass rather than only after the old
        socket errors out.
        """
        while self._running:
            try:
                with self._lock:
                    current_master = self.master
                msg = current_master.recv_match(blocking=True, timeout=0.1)
                if msg is None:
                    continue
                self._dispatch(msg)
            except Exception as e:
                self.logger.error("Message loop error: %s", e)
                self._reconnect()

    def _dispatch(self, msg):
        """Route message to the correct component state."""
        sysid = msg.get_srcSystem()
        compid = msg.get_srcComponent()

        # Filter out our own messages. There's nothing reserved about
        # sysid 255 in the spec - it's just the conventional GCS default,
        # and other participants on the link may legitimately use it too.
        # What we actually know for certain is our own identity.
        if sysid == self.source_system and compid == self.source_component:
            return

        key = (sysid, compid)
        with self._lock:
            if key not in self.components:
                self.logger.info("Discovered new component sysid=%d compid=%d", sysid, compid)
                self.components[key] = ComponentState(sysid=sysid, compid=compid)
            state = self.components[key]

            msg_type = msg.get_type()
            if msg_type == "HEARTBEAT":
                state.last_heartbeat = time.time()
                state.autopilot = msg.autopilot
            elif msg_type == "GLOBAL_POSITION_INT":
                state.lat = msg.lat / 1e7
                state.lon = msg.lon / 1e7
                state.alt = msg.alt / 1000.0
            elif msg_type == "COMMAND_ACK":
                state.ack_queue.append(msg)

    def _monitor_loop(self):
        """Monitor component liveness based on heartbeat timeouts."""
        while self._running:
            now = time.time()
            with self._lock:
                to_drop = []
                for key, state in self.components.items():
                    if state.last_heartbeat is None:
                        continue
                    elapsed = now - state.last_heartbeat
                    if elapsed > HEARTBEAT_DROP_TIMEOUT:
                        self.logger.warning(
                            "Dropping component sysid=%d compid=%d (no heartbeat for %.1fs)",
                            state.sysid, state.compid, elapsed
                        )
                        to_drop.append(key)
                    elif elapsed > HEARTBEAT_WARN_TIMEOUT:
                        self.logger.warning(
                            "Component sysid=%d compid=%d heartbeat late (%.1fs)",
                            state.sysid, state.compid, elapsed
                        )
                for key in to_drop:
                    del self.components[key]
            time.sleep(1.0)

    def start(self):
        """Start tracking and monitoring threads."""
        self._connect()
        self._running = True
        self._router_thread = threading.Thread(target=self._message_loop, daemon=True)
        self._monitor_thread = threading.Thread(target=self._monitor_loop, daemon=True)
        self._router_thread.start()
        self._monitor_thread.start()

    def stop(self):
        """Stop tracking and monitoring threads."""
        self._running = False
        self._router_thread.join(timeout=2.0)
        self._monitor_thread.join(timeout=2.0)
        if self._router_thread.is_alive():
            self.logger.warning("Router thread did not exit cleanly.")
        if self._monitor_thread.is_alive():
            self.logger.warning("Monitor thread did not exit cleanly.")

    def get_autopilot_components(self) -> List[Tuple[int, int]]:
        """Return list of (sysid, compid) for all known autopilots.

        Identifies autopilots via the HEARTBEAT `autopilot` field rather
        than assuming component ID 1 - compid is not a reliable signal
        of role, only MAV_COMP_ID_ALL (0) is reserved by the spec.
        """
        with self._lock:
            return [
                (s.sysid, s.compid)
                for s in self.components.values()
                if s.autopilot != mavutil.mavlink.MAV_AUTOPILOT_INVALID
            ]

    def send_command_with_ack(
        self,
        target_sysid: int,
        target_compid: int,
        command: int,
        params: List[float],
        timeout: float = 3.0
    ):
        """Send COMMAND_LONG to one specific (sysid, compid) and wait for
        its COMMAND_ACK. This is the "disambiguation" in action: the same
        command can be aimed at any one of several vehicles sharing the
        same link, and only that vehicle's ACK is matched and returned.
        """
        assert len(params) <= 7, "MAVLink COMMAND_LONG supports max 7 params"
        padded_params = params + [0.0] * (7 - len(params))
        self.master.mav.command_long_send(
            target_sysid, target_compid, command, 0, *padded_params
        )
        deadline = time.time() + timeout
        key = (target_sysid, target_compid)
        while time.time() < deadline:
            with self._lock:
                state = self.components.get(key)
                if state and state.ack_queue:
                    return state.ack_queue.pop()
            time.sleep(0.05)
        self.logger.warning(
            "No ACK received for command %d from sysid=%d compid=%d",
            command, target_sysid, target_compid
        )
        return None


def main():
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s] %(name)s: %(message)s"
    )

    parser = argparse.ArgumentParser(description="Multi-vehicle SYSID disambiguation example")
    parser.add_argument("--master", default="udp:127.0.0.1:14550", help="MAVLink connection string")
    parser.add_argument(
        "--discovery-time", type=float, default=3.0,
        help="Seconds to wait for vehicles to be discovered before sending commands"
    )
    args = parser.parse_args()

    tracker = MavlinkComponentTracker(args.master)
    tracker.start()

    try:
        # Allow time for vehicle discovery
        time.sleep(args.discovery_time)

        autopilots = tracker.get_autopilot_components()
        print(f"Discovered {len(autopilots)} autopilot(s): {autopilots}")

        if not autopilots:
            print("No autopilots discovered -- nothing to disambiguate. "
                  "Connect more than one vehicle to this link to see "
                  "individual targeting in action.")

        # Address each discovered vehicle individually. This is the part
        # that actually demonstrates disambiguation: the same command is
        # sent once per vehicle, and each ACK is matched back to the
        # (sysid, compid) it came from rather than to "whatever replied".
        for sysid, compid in autopilots:
            print(f"Sending ARM to sysid={sysid} compid={compid}")
            ack = tracker.send_command_with_ack(
                sysid, compid,
                mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM,
                [1.0]
            )
            if ack is not None:
                print(f"  ACK from sysid={sysid} compid={compid}: {ack}")
            else:
                print(f"  No ACK from sysid={sysid} compid={compid}")

        time.sleep(15)
    except KeyboardInterrupt:
        pass
    finally:
        tracker.stop()


if __name__ == "__main__":
    main()
