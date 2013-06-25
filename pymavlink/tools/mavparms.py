#!/usr/bin/env python

'''
extract mavlink parameter values
'''

import sys, time, os

from optparse import OptionParser
parser = OptionParser("mavparms.py [options]")

(opts, args) = parser.parse_args()

from pymavlink import mavutil

if len(args) < 1:
    print("Usage: mavparms.py [options] <LOGFILE...>")
    sys.exit(1)

parms = {}

def mavparms(logfile):
    '''extract mavlink parameters'''
    mlog = mavutil.mavlink_connection(filename)

    while True:
        m = mlog.recv_match(type='PARAM_VALUE')
        if m is None:
            return
        pname = str(m.param_id).strip()
        if len(pname) > 0:
            parms[pname] = m.param_value

total = 0.0
for filename in args:
    mavparms(filename)

keys = parms.keys()
keys.sort()
for p in keys:
    print("%-15s %.6f" % (p, parms[p]))
    
