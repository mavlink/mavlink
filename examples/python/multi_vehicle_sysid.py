#!/usr/bin/env python3
"""
Example: Multi-vehicle SYSID disambiguation using pymavlink.

Demonstrates how to receive MAVLink messages from multiple vehicles
on a single connection and route them by (sysid, compid) pairs.
"""

import time, logging, argparse, threading
from dataclasses import dataclass
from typing import Dict, Tuple, List
from collections import deque

from pymavlink import mavutil


@dataclass
class ComponentState:
        sysid: int
        compid: int
        last_heartbeat: float = None
        last_position: Tuple[float, float, float] = (0.0, 0.0, 0.0)
        command_ack_queue: deque = None

    def __post_init__(self):
                self.command_ack_queue = deque(maxlen=20)


class MavlinkFleetRouter:
        def __init__(self, connection_string, baud=115200):
                    self.master = mavutil.mavlink_connection(connection_string, baud=baud)
                    self.components: Dict[Tuple[int, int], ComponentState] = {}
                    self._lock = threading.Lock()
                    self._running = True
                    self._router_thread = threading.Thread(target=self._message_loop, daemon=True)
                    self._monitor_thread = threading.Thread(target=self._monitor_loop, daemon=True)
                    self.logger = logging.getLogger("MavlinkFleetRouter")

        def start(self):
                    self._router_thread.start()
                    self._monitor_thread.start()

        def stop(self):
                    self._running = False
                    self._router_thread.join(timeout=2.0)
                    self._monitor_thread.join(timeout=2.0)
                    with self._lock:
                                    try:
                                                        self.master.close()
except Exception:
                pass
        self.logger.info("Threads and sockets stopped cleanly.")

    def _message_loop(self):
                while self._running:
                                try:
                                                    with self._lock:
                                                                            msg = self.master.recv_match(blocking=True, timeout=0.1)
                                                                        if not msg:
                                                                                                continue
                                                                                            sysid = msg.get_srcSystem()
                                                    compid = msg.get_srcComponent()
                                                    if sysid == 255:
                                                                            continue
                                                                        key = (sysid, compid)
                                                    with self._lock:
                                                                            if key not in self.components:
                                                                                                        self.components[key] = ComponentState(sysid=sysid, compid=compid)
                                                                                                    state = self.components[key]
                                                                        mtype = msg.get_type()
                                                    if mtype == 'HEARTBEAT':
                                                                            state.last_heartbeat = time.time()
elif mtype == 'GLOBAL_POSITION_INT':
                    state.last_position = (msg.lat / 1e7, msg.lon / 1e7, msg.alt / 1e3)
elif mtype == 'COMMAND_ACK':
                    state.command_ack_queue.append(msg.command)
except Exception as e:
                self.logger.error(f"Error in message loop: {e}")

    def _monitor_loop(self):
                while self._running:
                                with self._lock:
                                                    now = time.time()
                                                    for key, state in list(self.components.items()):
                                                                            if state.last_heartbeat is None or (now - state.last_heartbeat > 5.0):
                                                                                                        self.logger.warning(f"Vehicle {key} lost heartbeat!")
                                                                                            time.sleep(1.0)

                                        def send_command_with_ack(self, target_sysid, target_compid, command, params):
                                                    assert len(params) <= 7, "MAVLink COMMAND_LONG supports max 7 params"
                                                    padded_params = params + [0.0] * (7 - len(params))
                                                    self.master.mav.command_long_send(target_sysid, target_compid, command, 0, *padded_params)


        def main():
                logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
                parser = argparse.ArgumentParser(description="Multi-vehicle SYSID example")
                parser.add_argument("--master", required=True, help="MAVLink connection string")
                args = parser.parse_args()
                router = MavlinkFleetRouter(args.master)
                router.start()
                try:
                            while True:
                                            time.sleep(1)
                except KeyboardInterrupt:
                            router.stop()


if __name__ == "__main__":
        main()
    
