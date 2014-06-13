#!/usr/bin/env python

'''
show MAVLink packet loss
'''

import sys, time, os

from optparse import OptionParser
parser = OptionParser("mavloss.py [options] <LOGFILE1> [<LOGFILE2> ...]")
parser.add_option("--no-timestamps",dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
parser.add_option("--planner",dest="planner", action='store_true', help="use planner file format")
parser.add_option("--robust",dest="robust", action='store_true', help="Enable robust parsing (skip over bad data)")
parser.add_option("--condition", default=None, help="condition for packets")

(opts, args) = parser.parse_args()

from pymavlink import mavutil

if len(args) < 1:
    parser.print_help()
    sys.exit(1)

def mavloss(logfile):
    '''work out signal loss times for a log file'''
    print("Processing log %s" % filename)
    mlog = mavutil.mavlink_connection(filename,
                                      planner_format=opts.planner,
                                      notimestamps=opts.notimestamps,
                                      robust_parsing=opts.robust)

    # Track the reasons for MAVLink parsing errors and print them all out at the end.
    reason_ids = set()
    reasons = []

    while True:
        m = mlog.recv_match(condition=opts.condition)

        # Stop parsing the file once we've reached the end
        if m is None:
            break

        # Save the parsing failure reason for this message if it's a new one
        if m.get_type() == 'BAD_DATA':
            reason_id = ''.join(m.reason.split(' ')[0:3])
            if reason_id not in reason_ids:
                reason_ids.add(reason_id)
                reasons.append(m.reason)

    # Print out the final packet loss results
    print("%u packets, %u lost %.1f%%" % (
            mlog.mav_count, mlog.mav_loss, mlog.packet_loss()))

    # Also print out the reasons why losses occurred
    if len(reasons) > 0:
        print("Packet loss at least partially attributed to the following:")
        for r in reasons:
            print("  * " + r)

for filename in args:
    mavloss(filename)
