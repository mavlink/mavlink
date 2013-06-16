#!/usr/bin/env python
'''
compare two MAVLink parameter files
'''

import sys, os

# allow import from the parent directory, where mavlink.py is
sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))

import mavutil, mavparm

from optparse import OptionParser
parser = OptionParser("mavparmdiff.py [options]")
(opts, args) = parser.parse_args()

if len(args) < 2:
    print("Usage: mavparmdiff.py FILE1 FILE2")
    sys.exit(1)

file1 = args[0]
file2 = args[1]

p1 = mavparm.MAVParmDict()
p2 = mavparm.MAVParmDict()
p1.load(file2)
p1.diff(file1)

