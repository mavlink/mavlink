#!/usr/bin/env python

'''
extract one mode type from a log
'''

import sys, time, os, struct

from argparse import ArgumentParser
parser = ArgumentParser(description=__doc__)

parser.add_argument("--no-timestamps", dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
parser.add_argument("--robust", action='store_true', help="Enable robust parsing (skip over bad data)")
parser.add_argument("--condition", default=None, help="select packets by condition")
parser.add_argument("--mode", default='auto', help="mode to extract")
parser.add_argument("logs", metavar="LOG", nargs="+")
args = parser.parse_args()

from pymavlink import mavutil


def process(filename):
    '''process one logfile'''
    print("Processing %s" % filename)
    mlog = mavutil.mavlink_connection(filename, notimestamps=args.notimestamps,
                                      robust_parsing=args.robust)


    ext = os.path.splitext(filename)[1]
    isbin = ext in ['.bin', '.BIN']
    islog = ext in ['.log', '.LOG']
    output = None
    count = 1
    dirname = os.path.dirname(filename)

    if isbin or islog:
        extension = "bin"
    else:
        extension = "tlog"

    file_header = ''

    while True:
        m = mlog.recv_match()
        if m is None:
            break
        if (isbin or islog) and m.get_type() in ["FMT", "PARM", "CMD"]:
            file_header += m.get_msgbuf()
        if (isbin or islog) and m.get_type() == 'MSG' and m.Message.startswith("Ardu"):
            file_header += m.get_msgbuf()
        if m.get_type() in ['PARAM_VALUE','MISSION_ITEM']:
            timestamp = getattr(m, '_timestamp', None)
            file_header += struct.pack('>Q', timestamp*1.0e6) + m.get_msgbuf()

        if not mavutil.evaluate_condition(args.condition, mlog.messages):
            continue

        if mlog.flightmode.upper() == args.mode.upper():
            if output is None:
                path = os.path.join(dirname, "%s%u.%s" % (args.mode, count, extension))
                count += 1
                print("Creating %s" % path)
                output = open(path, mode='wb')
                output.write(file_header)
        else:
            if output is not None:
                output.close()
                output = None

        if output and m.get_type() != 'BAD_DATA':
            timestamp = getattr(m, '_timestamp', None)
            if not isbin:
                output.write(struct.pack('>Q', timestamp*1.0e6))
            output.write(m.get_msgbuf())

for filename in args.logs:
    process(filename)

