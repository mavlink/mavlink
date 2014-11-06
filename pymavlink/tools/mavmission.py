#!/usr/bin/env python

'''
extract mavlink mission from log
'''

import sys, time, os

from argparse import ArgumentParser
parser = ArgumentParser(description=__doc__)
parser.add_argument("--output", default='mission.txt', help="output file")
parser.add_argument("logs", metavar="LOG", nargs="+")

args = parser.parse_args()

from pymavlink import mavutil, mavwp

parms = {}

def mavmission(logfile):
    '''extract mavlink mission'''
    mlog = mavutil.mavlink_connection(filename)

    wp = mavwp.MAVWPLoader()

    while True:
        m = mlog.recv_match(type=['MISSION_ITEM','CMD','WAYPOINT'])
        if m is None:
            break
        if m.get_type() == 'CMD':
            m = mavutil.mavlink.MAVLink_mission_item_message(0,
                                                             0,
                                                             m.CNum,
                                                             mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
                                                             m.CId,
                                                             0, 1,
                                                             m.Prm1, m.Prm2, m.Prm3, m.Prm4,
                                                             m.Lat, m.Lng, m.Alt)

        while m.seq > wp.count():
            print("Adding dummy WP %u" % wp.count())
            wp.set(m, wp.count())
        wp.set(m, m.seq)
    wp.save(args.output)
    print("Saved %u waypoints to %s" % (wp.count(), args.output))


total = 0.0
for filename in args.logs:
    mavmission(filename)
