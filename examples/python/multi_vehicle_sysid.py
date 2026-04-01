#!/usr/bin/env python3
"""
multi_vehicle_sysid.py - Example demonstrating MAVLink multi-vehicle system ID handling.

This script connects to multiple MAVLink vehicles (or simulated endpoints),
listens for heartbeats, and tracks each unique system ID (SYSID) seen on the link.

Usage:
    python multi_vehicle_sysid.py --master udp:127.0.0.1:14550

"""

import argparse
import time
from collections import defaultdict

try:
    from pymavlink import mavutil
except Exception:
    print("ERROR: pymavlink is not installed. Run: pip install pymavlink")
    raise


def parse_args():
    parser = argparse.ArgumentParser(description="MAVLink multi-vehicle system ID tracker")
    parser.add_argument(
        "--master",
        default="udp:127.0.0.1:14550",
        help="MAVLink connection string (default: udp:127.0.0.1:14550)",
    )
    parser.add_argument(
        "--timeout",
        type=float,
        default=30.0,
        help="How long to listen for vehicles, in seconds (default: 30)",
    )
    parser.add_argument(
        "--baud",
        type=int,
        default=57600,
        help="Baud rate for serial connections (default: 57600)",
    )
    return parser.parse_args()


def connect(connection_string, baud):
    """Establish a MAVLink connection."""
    print(f"Connecting to: {connection_string}")
    try:
        master = mavutil.mavlink_connection(connection_string, baud=baud)
    except Exception:
        print(f"Failed to open connection to '{connection_string}'")
        raise
    return master


def listen_for_vehicles(master, timeout):
    """
    Listen for MAVLink heartbeat messages and collect unique system IDs.

    Returns a dict mapping sysid -> list of component IDs seen.
    """
    vehicles = defaultdict(set)
    start = time.time()

    print(f"Listening for vehicles for {timeout:.1f} seconds...")

    while time.time() - start < timeout:
        try:
            msg = master.recv_match(type="HEARTBEAT", blocking=True, timeout=1.0)
        except KeyboardInterrupt:
            print("\nInterrupted by user.")
            break
        except Exception:
            print("Warning: error receiving MAVLink message, retrying...")
            continue

        if msg is None:
            continue

        sysid = msg.get_srcSystem()
        compid = msg.get_srcComponent()

        if sysid == 0:
            # Ignore broadcast system ID
            continue

        if compid not in vehicles[sysid]:
            vehicles[sysid].add(compid)
            print(
                f"  [+] Discovered vehicle — SYSID: {sysid}, COMPID: {compid}"
                f"  (type={msg.type}, autopilot={msg.autopilot})"
            )

    return vehicles


def report(vehicles):
    """Print a summary of discovered vehicles."""
    print("\n--- Multi-Vehicle Summary ---")
    if not vehicles:
        print("No vehicles detected.")
        return

    for sysid in sorted(vehicles):
        compids = sorted(vehicles[sysid])
        print(f"  SYSID {sysid}: components {compids}")

    print(f"\nTotal unique system IDs: {len(vehicles)}")


def main():
    args = parse_args()

    try:
        master = connect(args.master, args.baud)
    except Exception:
        return 1

    try:
        vehicles = listen_for_vehicles(master, args.timeout)
    except ValueError:
        print("Invalid configuration value encountered.")
        return 1
    except KeyboardInterrupt:
        print("\nStopped by user.")
        vehicles = {}

    report(vehicles)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
