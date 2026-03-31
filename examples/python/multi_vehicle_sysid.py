from pymavlink import mavutil
import time

master = mavutil.mavlink_connection('udpin:0.0.0.0:14550')
vehicles = {}

def wait_for_vehicles(timeout=10):
      print("Discovering vehicles...")
      start = time.time()
      while time.time() - start < timeout:
                msg = master.recv_match(type='HEARTBEAT', blocking=True, timeout=1)
                if msg and msg.get_srcSystem() != 255:
                              sysid = msg.get_srcSystem()
                              if sysid not in vehicles:
                                                print(f"Found vehicle: sysid={sysid}")
                                            vehicles[sysid] = time.time()
                      return vehicles

def get_position(target_sysid):
      while True:
                msg = master.recv_match(type='GLOBAL_POSITION_INT', blocking=True, timeout=5)
                if msg and msg.get_srcSystem() == target_sysid:
                              print(f"[sysid={target_sysid}] lat={msg.lat/1e7}, lon={msg.lon/1e7}, alt={msg.alt/1e3}m")
                              return msg

        def send_command(target_sysid, target_compid, command, *params):
              master.mav.command_long_send(
                        target_sysid,
                        target_compid,
                        command,
                        0,
                        *params
              )
              print(f"Command {command} sent to sysid={target_sysid}")

wait_for_vehicles(timeout=10)
if vehicles:
      first_sysid = list(vehicles.keys())[0]
    get_position(first_sysid)
    send_command(1, 1, mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM, 1, 0, 0, 0, 0, 0, 0)
