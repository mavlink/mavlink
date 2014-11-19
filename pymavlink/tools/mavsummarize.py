#!/usr/bin/env python

'''
Summarize MAVLink logs. Useful for identifying which log is of interest in a large set.
'''

import sys, time, os, glob

from argparse import ArgumentParser
parser = ArgumentParser(description=__doc__)
parser.add_argument("--no-timestamps", dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
parser.add_argument("--condition", default=None, help="condition for packets")
parser.add_argument("--dialect", default="ardupilotmega", help="MAVLink dialect")
parser.add_argument("logs", metavar="LOG", nargs="+")

args = parser.parse_args()

from pymavlink import mavutil
from pymavlink.mavextra import distance_two


def PrintSummary(logfile):
    '''Calculate some interesting datapoints of the file'''
    # Open the log file
    mlog = mavutil.mavlink_connection(filename, notimestamps=args.notimestamps, dialect=args.dialect)

    autonomous_sections = 0 # How many different autonomous sections there are
    autonomous = False # Whether the vehicle is currently autonomous at this point in the logfile
    auto_time = 0.0 # The total time the vehicle was autonomous/guided (seconds)
    start_time = None # The datetime of the first received message (seconds since epoch)
    total_dist = 0.0 # The total ground distance travelled (meters)
    last_gps_msg = None # The first GPS message received
    first_gps_msg = None # The last GPS message received

    while True:
        m = mlog.recv_match(condition=args.condition)

        # If there's no match, it means we're done processing the log.
        if m is None:
            break

        # Ignore any failed messages
        if m.get_type() == 'BAD_DATA':
            continue

        # Keep track of the latest timestamp for various calculations
        timestamp = getattr(m, '_timestamp', 0.0)

        # Log the first message time
        if start_time is None:
            start_time = timestamp

        # Track the vehicle's speed and status
        if m.get_type() == 'GPS_RAW_INT':

            # Ignore GPS messages without a proper fix
            if m.fix_type < 3 or m.lat == 0 or m.lon == 0:
                continue

            # Log the first GPS location found, being sure to skip GPS fixes
            # that are bad (at lat/lon of 0,0)
            if first_gps_msg is None:
                first_gps_msg = m

            # Track the distance travelled, being sure to skip GPS fixes
            # that are bad (at lat/lon of 0,0)
            if last_gps_msg is not None:
                total_dist += distance_two(last_gps_msg, m)

            # Save this GPS message to do simple distance calculations with
            last_gps_msg = m

        elif m.get_type() == 'HEARTBEAT':
            if (m.base_mode & mavutil.mavlink.MAV_MODE_FLAG_GUIDED_ENABLED or
                m.base_mode & mavutil.mavlink.MAV_MODE_FLAG_AUTO_ENABLED) and autonomous == False:
                autonomous = True
                autonomous_sections += 1
                start_auto_time = timestamp
            elif (not m.base_mode & mavutil.mavlink.MAV_MODE_FLAG_GUIDED_ENABLED and
                not m.base_mode & mavutil.mavlink.MAV_MODE_FLAG_AUTO_ENABLED) and autonomous == True:
                autonomous = False
                auto_time += timestamp - start_auto_time

    # If there were no messages processed, say so
    if start_time is None:
        print("ERROR: No messages found.")
        return

    # If the vehicle ends in autonomous mode, make sure we log the total time
    if autonomous == True:
        auto_time += timestamp - start_auto_time

    # Compute the total logtime, checking that timestamps are 2009 or newer for validity
    # (MAVLink didn't exist until 2009)
    if start_time >= 1230768000:
        start_time_str = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(start_time))
        end_time_str = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(timestamp))
        print("Timespan from {} to {}".format(start_time_str, end_time_str))
    else:
        print(first_gps_msg.time_usec)
        print(last_gps_msg.time_usec)
        print("ERROR: Invalid timestamps found.")

    # Print location data
    if last_gps_msg is not None:
        first_gps_position = (first_gps_msg.lat / 1e7, first_gps_msg.lon / 1e7)
        last_gps_position = (last_gps_msg.lat / 1e7, last_gps_msg.lon / 1e7)
        print("Travelled from ({0[0]}, {0[1]}) to ({1[0]}, {1[1]})".format(first_gps_position, last_gps_position))
        print("Total distance : {:0.2f}m".format(total_dist))
    else:
        print("ERROR: No GPS data found.")

    # Print out the rest of the results.
    total_time = timestamp - start_time
    print("Total time : {}:{:02}".format(int(total_time)/60, int(total_time)%60))
    # The autonomous time should be good, as a steady HEARTBEAT is required for MAVLink operation
    print("Autonomous sections : {}".format(autonomous_sections))
    if autonomous_sections > 0:
        print("Autonomous time : {}:{:02}".format(int(auto_time)/60, int(auto_time)%60))

for filename in args.logs:
    for f in glob.glob(filename):
        print("Processing log %s" % filename)
        PrintSummary(f)