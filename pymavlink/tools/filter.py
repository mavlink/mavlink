#!/usr/bin/python
"""Generate a MAVLink datastream from another one by excluding certain messages. Useful for excluding error messages."""

from pymavlink import mavutil

import sys

# Set up input arguments
from argparse import ArgumentParser
parser = ArgumentParser(description=__doc__)
parser.add_argument("--no-timestamps", dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
parser.add_argument("--planner", action='store_true', help="Use the planner file format")
parser.add_argument("--robust", action='store_true', help="Enable robust parsing (skip over bad data)")
parser.add_argument("log", help="MAVLink file to process")
parser.add_argument("--out", required=True, help="Output filename")
parser.add_argument("--exclude", help="Comma-separated list of messages to remove.")
parser.add_argument("--dialect", default="ardupilotmega", help="MAVLink dialect")
parser.add_argument("--zero-time-base", action='store_true', help="use Z time base for DF logs")

# Process input arguments
args = parser.parse_args()

# Open both the input and output files
mlog = mavutil.mavlink_connection(args.log, planner_format=args.planner,
                                  notimestamps=args.notimestamps,
                                  robust_parsing=args.robust,
                                  dialect=args.dialect,
                                  zero_time_base=args.zero_time_base)
olog = open(args.out, 'wb')

# Turn the message filter types into an array
if args.exclude:
    unwanted = args.exclude.split(",")
else:
    unwanted = []

# Read through the file and output all messages that don't match our exclusion filter.
m = mlog.recv_msg()
while m != None:
    if m.get_type() not in unwanted:
        olog.write(m.get_msgbuf())
    m = mlog.recv_msg()
