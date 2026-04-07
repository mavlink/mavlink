#!/usr/bin/env python3
"""
Example: Multi-vehicle SYSID disambiguation using pymavlink.

Demonstrates how to receive MAVLink messages from multiple vehicles
on a single connection and route them by (sysid, compid) pairs.
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
    lat: float = 0.0
    lon: float = 0.0
    alt: float = 0.0
    ack_queue: deque = field(default_factory=lambda: deque(maxlen=MAX_ACK_QUEUE))


class MavlinkFleetRouter:
    """Manages MAVLink connections and routes messages by (sysid, compid)."""

    def __init__(self, connection_string: str):
        self.connection_string = connection_string
        self.master = None
        self.components: Dict[Tuple[int, int], ComponentState] = {}
        self._lock = threading.Lock()
        self._running = False
        self._router_thread = None
        self._monitor_thread = None
        self.logger = logging.getLogger("MavlinkFleetRouter")

    def _connect(self):
        """Establish MAVLink connection."""
        self.logger.info("Connecting to %s", self.connection_string)
        self.master = mavutil.mavlink_connection(self.connection_string)
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
        """Main loop: receive and dispatch MAVLink messages."""
        while self._running:
            try:
                with self._lock:
                    # Copy reference under lock; note reconnect may replace
                    # self.master between iterations — this is intentional.
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

        # Filter out GCS self-messages
        if sysid == 255:
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
        """Start routing and monitoring threads."""
        self._connect()
        self._running = True
        self._router_thread = threading.Thread(target=self._message_loop, daemon=True)
        self._monitor_thread = threading.Thread(target=self._monitor_loop, daemon=True)
        self._router_thread.start()
        self._monitor_thread.start()

    def stop(self):
        """Stop routing and monitoring threads."""
        self._running = False
        self._router_thread.join(timeout=2.0)
        self._monitor_thread.join(timeout=2.0)
        if self._router_thread.is_alive():
            self.logger.warning("Router thread did not exit cleanly.")
        if self._monitor_thread.is_alive():
            self.logger.warning("Monitor thread did not exit cleanly.")

    def get_autopilot_components(self) -> List[Tuple[int, int]]:
        """Return list of (sysid, compid) for all known autopilots."""
        with self._lock:
            return [
                (s.sysid, s.compid)
                for s in self.components.values()
                if s.compid == mavutil.mavlink.MAV_COMP_ID_AUTOPILOT1
            ]

    def send_command_with_ack(
        self,
        target_sysid: int,
        target_compid: int,
        command: int,
        params: List[float],
        timeout: float = 3.0
    ):
        """Send COMMAND_LONG and wait for COMMAND_ACK."""
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
        self.logger.warning("No ACK received for command %d", command)
        return None


def main():
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s] %(name)s: %(message)s"
    )

    parser = argparse.ArgumentParser(description="Multi-vehicle SYSID disambiguation example")
    parser.add_argument("--master", default="udp:127.0.0.1:14550", help="MAVLink connection string")
    args = parser.parse_args()

    router = MavlinkFleetRouter(args.master)
    router.start()

    try:
        # Allow time for vehicle discovery
        time.sleep(3)

        autopilots = router.get_autopilot_components()
        print(f"Discovered autopilots: {autopilots}")

        if autopilots:
            sysid, compid = autopilots[0]
            print(f"Sending ARM to sysid={sysid} compid={compid}")
            ack = router.send_command_with_ack(
                sysid, compid,
                mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM,
                [1.0]
            )
            print(f"ACK: {ack}")

        time.sleep(15)
    except KeyboardInterrupt:
        pass
    finally:
        router.stop()


if __name__ == "__main__":
    main()
