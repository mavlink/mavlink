import time
import logging
import argparse
import threading
from dataclasses import dataclass
from typing import Dict, Tuple, List
from pymavlink import mavutil

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s [%(levelname)s] %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)
logger = logging.getLogger("AvionicsRouter")

MAX_ACK_QUEUE = 5

@dataclass
class ComponentState:
    sysid: int
    compid: int
    type: int = 0
    autopilot: int = 0
    last_heartbeat: float = 0.0
    lat: float = 0.0
    lon: float = 0.0
    alt: float = 0.0
    link_warned: bool = False

class MavlinkFleetRouter:
    def __init__(self, connection_string: str):
        self.connection_string = connection_string
        
        # Thread Safety Lock
        self._lock = threading.Lock()
        
        self.components: Dict[Tuple[int, int], ComponentState] = {}
        self._pending_acks: Dict[Tuple[int, int, int], List[int]] = {}
        
        self._running = False
        self._router_thread = None
        self._monitor_thread = None

        self._connect()

    def _connect(self):
        """Creates the MAVLink connection. Safe to call during reconnects."""
        logger.info(f"Establishing MAVLink connection on {self.connection_string}...")
        self.master = mavutil.mavlink_connection(self.connection_string)
        
        # Wait for the first heartbeat to confirm physical link
        self.master.wait_heartbeat(blocking=True)
        logger.info("Heartbeat received. Connection established.")

    def _reconnect(self):
        """Tears down the broken socket and rebuilds it."""
        logger.warning("Attempting socket reconnection...")
        with self._lock:
            try:
                self.master.close()
            except Exception as e:
                logger.debug(f"Error closing dead socket: {e}")
        
        time.sleep(1.0) # Prevent rapid-fire reconnect spanning
        
        try:
            # Reassign connection
            with self._lock:
                self.master = mavutil.mavlink_connection(self.connection_string)
            logger.info("Socket recreated successfully. Waiting for telemetry resumption...")
        except Exception as e:
            logger.error(f"Reconnection failed: {e}")

    def start_routing(self):
        if self._running:
            return
        self._running = True
        
        self._router_thread = threading.Thread(target=self._message_loop, daemon=True)
        self._monitor_thread = threading.Thread(target=self._monitor_fleet, daemon=True)
        
        self._router_thread.start()
        self._monitor_thread.start()
        logger.info("Router and Monitor threads started.")

    def stop_routing(self):
        self._running = False
        if self._router_thread:
            self._router_thread.join()
        if self._monitor_thread:
            self._monitor_thread.join()
        
        with self._lock:
            try:
                self.master.close()
            except:
                pass
        logger.info("Threads and sockets stopped cleanly.")

    def _message_loop(self):
        while self._running:
            try:
                # Obtain a thread-safe reference to the current master
                with self._lock:
                    current_master = self.master
                
                msg = current_master.recv_match(blocking=True, timeout=0.1)
                if not msg:
                    continue

                mtype = msg.get_type()
                if mtype == 'BAD_DATA':
                    continue

                sysid = msg.get_srcSystem()
                compid = msg.get_srcComponent()
                
                if sysid == 255:
                    continue

                with self._lock:
                    if (sysid, compid) not in self.components:
                        self.components[(sysid, compid)] = ComponentState(sysid=sysid, compid=compid)

                if mtype == 'HEARTBEAT':
                    self._handle_heartbeat(sysid, compid, msg)
                elif mtype == 'GLOBAL_POSITION_INT':
                    self._handle_position(sysid, compid, msg)
                elif mtype == 'COMMAND_ACK':
                    self._handle_ack(sysid, compid, msg)

            except (OSError, mavutil.mavlink.MAVError) as e:
                logger.error(f"Socket/MAVLink error detected: {e}")
                self._reconnect()
            except Exception as e:
                logger.error(f"Unexpected error in router loop: {e}")
                time.sleep(0.5)

    def _handle_heartbeat(self, sysid: int, compid: int, msg):
        with self._lock:
            state = self.components[(sysid, compid)]
            if state.last_heartbeat == 0.0:
                logger.info(f"Discovered new component: SYSID={sysid}, COMPID={compid}, TYPE={msg.type}")
                state.type = msg.type
                state.autopilot = msg.autopilot
            
            state.last_heartbeat = time.time()
            if state.link_warned:
                logger.info(f"[SYS={sysid}|COMP={compid}] Link restored!")
                state.link_warned = False

    def _handle_position(self, sysid: int, compid: int, msg):
        with self._lock:
            state = self.components[(sysid, compid)]
            state.lat = msg.lat / 1e7
            state.lon = msg.lon / 1e7
            state.alt = msg.alt / 1e3

    def _handle_ack(self, sysid: int, compid: int, msg):
        ack_key = (sysid, compid, msg.command)
        with self._lock:
            queue = self._pending_acks.setdefault(ack_key, [])
            queue.append(msg.result)
            
            # Bound the queue size to prevent memory leaks from spam
            if len(queue) > MAX_ACK_QUEUE:
                logger.debug(f"ACK queue for {ack_key} full. Dropping oldest.")
                queue.pop(0)

    def _monitor_fleet(self):
        while self._running:
            now = time.time()
            dead_keys = []

            with self._lock:
                for k, state in self.components.items():
                    age = now - state.last_heartbeat
                    
                    if age > 2.0 and not state.link_warned:
                        logger.warning(f"[SYS={state.sysid}|COMP={state.compid}] Link degraded! No heartbeat in {age:.1f}s")
                        state.link_warned = True
                    
                    if age > 5.0:
                        dead_keys.append(k)

                for k in dead_keys:
                    logger.error(f"[SYS={k[0]}|COMP={k[1]}] Dropping stale component from routing table.")
                    del self.components[k]

            time.sleep(1.0)

    def get_autopilot_components(self) -> list[Tuple[int, int]]:
        with self._lock:
            return [(s, c) for (s, c), state in self.components.items() if c == 1 and state.last_heartbeat > 0]

    def print_fleet_status(self):
        with self._lock:
            for (sysid, compid), state in self.components.items():
                age = time.time() - state.last_heartbeat
                logger.info(f"[SYS={sysid}|COMP={compid}] Age: {age:.1f}s | Pos: {state.lat:.5f}, {state.lon:.5f}, {state.alt:.1f}m")

    def send_command_with_ack(self, target_sysid: int, target_compid: int, 
                              command: int, params: list, timeout: float = 3.0) -> bool:
        padded_params = list(params) + [0] * (7 - len(params))
        ack_key = (target_sysid, target_compid, command)

        logger.debug(f"Sending command {command} to SYS={target_sysid}, COMP={target_compid}...")
        
        with self._lock:
            current_master = self.master
            
        try:
            current_master.mav.command_long_send(
                target_sysid, target_compid, command, 0, *padded_params
            )
        except Exception as e:
            logger.error(f"Failed to send command over socket: {e}")
            return False

        start_time = time.time()
        while time.time() - start_time < timeout:
            with self._lock:
                if ack_key in self._pending_acks and self._pending_acks[ack_key]:
                    result = self._pending_acks[ack_key].pop(0)
                    
                    # Memory leak fix: Cleanup empty queues immediately
                    if not self._pending_acks[ack_key]:
                        del self._pending_acks[ack_key]

                    if result == mavutil.mavlink.MAV_RESULT_ACCEPTED:
                        logger.info(f"[SYS={target_sysid}|COMP={target_compid}] Command {command} ACCEPTED.")
                        return True
                    else:
                        logger.error(f"[SYS={target_sysid}|COMP={target_compid}] Command {command} REJECTED with code: {result}")
                        return False
            time.sleep(0.05)
            
        logger.error(f"[SYS={target_sysid}|COMP={target_compid}] Command {command} TIMED OUT.")
        
        # Memory leak fix: Cleanup abandoned keys on timeout
        with self._lock:
            if ack_key in self._pending_acks:
                del self._pending_acks[ack_key]
                
        return False


def main():
    parser = argparse.ArgumentParser(description="Multi-Vehicle MAVLink Router")
    parser.add_argument('--connect', type=str, default='udpin:0.0.0.0:14550', help="MAVLink connection string")
    args = parser.parse_args()

    router = MavlinkFleetRouter(args.connect)
    router.start_routing()

    try:
        logger.info("Building routing table...")
        time.sleep(3.0)
        
        router.print_fleet_status()

        autopilots = router.get_autopilot_components()
        if autopilots:
            target_sysid, target_compid = autopilots[0]

            logger.info(f"Attempting to arm SYS={target_sysid}, COMP={target_compid}...")
            router.send_command_with_ack(
                target_sysid=target_sysid,
                target_compid=target_compid,
                command=mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM,
                params=[1] 
            )

        logger.info("Running fleet monitor for 15 seconds. (Disconnect network or vehicle to test reconnect/teardown)")
        for _ in range(15):
            time.sleep(1)

    except KeyboardInterrupt:
        logger.info("Interrupted by user.")
    finally:
        router.stop_routing()

if __name__ == "__main__":
    main()
