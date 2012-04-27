#!/usr/bin/env python

'''
show MAVLink packet loss
'''

import sys, time, os

# allow import from the parent directory, where mavlink.py is
sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))

from optparse import OptionParser
parser = OptionParser("sigloss.py [options]")
parser.add_option("--no-timestamps",dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
parser.add_option("--planner",dest="planner", action='store_true', help="use planner file format")
parser.add_option("--robust",dest="robust", action='store_true', help="Enable robust parsing (skip over bad data)")
parser.add_option("--mav10", action='store_true', default=False, help="Use MAVLink protocol 1.0")

(opts, args) = parser.parse_args()

if opts.mav10:
    os.environ['MAVLINK10'] = '1'
import mavutil

if len(args) < 1:
    print("Usage: mavloss.py [options] <LOGFILE...>")
    sys.exit(1)

def mavloss(logfile):
    '''work out signal loss times for a log file'''
    print("Processing log %s" % filename)
    mlog = mavutil.mavlink_connection(filename,
                                      planner_format=opts.planner,
                                      notimestamps=opts.notimestamps,
                                      robust_parsing=opts.robust)

    m = mlog.recv_match()
    seq = m.get_seq()
    count = 1
    loss = 0

    while True:
        m = mlog.recv_match()
        if m is None:
            break
        if m.get_srcSystem() == ord('3') and m.get_srcComponent() == ord('D'):
            # its the radio
            continue
        if m.get_srcSystem() == 255:
            # its the planner
            continue
        count += 1
        if m.get_seq() != (seq+1) % 256:
            diff = (m.get_seq() - seq) % 256
            loss += diff - 1
        seq = m.get_seq()
    print("%u packets, %u lost %u%%" % (
        count, loss, (100.0*loss)/(count+loss)))


total = 0.0
for filename in args:
    mavloss(filename)
