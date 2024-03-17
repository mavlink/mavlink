import socket
import time
from pymavlink import mavutil

def receive_some(socket_fd):
    try:
        buffer = socket_fd.recv(2048)
        if not buffer:
            return None
        return buffer
    except socket.timeout:
        return None
    except socket.error as e:
        print("recvfrom error:", e)
        return None

def handle_heartbeat(message):
    msg = mavutil.mavlink.MAVLink_heartbeat_decode(message)
    autopilot = "other"
    if msg.autopilot == mavutil.mavlink.MAV_AUTOPILOT_GENERIC:
        autopilot = "generic"
    elif msg.autopilot == mavutil.mavlink.MAV_AUTOPILOT_ARDUPILOTMEGA:
        autopilot = "ArduPilot"
    elif msg.autopilot == mavutil.mavlink.MAV_AUTOPILOT_PX4:
        autopilot = "PX4"
    print("Got heartbeat from", autopilot, "autopilot")

def send_heartbeat(socket_fd, dest_addr):
    system_id = 42
    base_mode = 0
    custom_mode = 0
    state = mavutil.mavlink.MAV_STATE_STANDBY

    msg = mavutil.mavlink.MAVLink_heartbeat_message(
        system_id,
        mavutil.mavlink.MAV_TYPE_GENERIC, 
        mavutil.mavlink.MAV_AUTOPILOT_GENERIC, 
        base_mode, 
        custom_mode, 
        state)

    buffer = msg.pack(mavutil.mavlink.MAVLink_dialect)
    try:
        socket_fd.sendto(buffer, dest_addr)
        print("Sent heartbeat")
    except socket.error as e:
        print("sendto error:", e)

def main():
    socket_fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    socket_fd.settimeout(0.1)

    addr = ("0.0.0.0", 14560)  
    socket_fd.bind(addr)

    dest_addr = None

    while True:
        buffer = receive_some(socket_fd)

        if buffer:
            if not dest_addr:
                dest_addr = tuple(socket_fd.getsockname())
            try:
                message = mavutil.mavlink.mavlink_parse_char(buffer)
                handle_heartbeat(message)
            except mavutil.mavlink.MAVError:
                pass

        if dest_addr:
            send_heartbeat(socket_fd, dest_addr)

        time.sleep(0.1)

if __name__ == "__main__":
    main()
