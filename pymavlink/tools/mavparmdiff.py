#!/usr/bin/env python
'''
compare two MAVLink parameter files
'''

from pymavlink import mavparm

from argparse import ArgumentParser
parser = ArgumentParser(description=__doc__)
parser.add_argument("file1", metavar="FILE1")
parser.add_argument("file2", metavar="FILE2")
args = parser.parse_args()

file1 = args.file1
file2 = args.file2

p1 = mavparm.MAVParmDict()
p2 = mavparm.MAVParmDict()
p1.load(file2)
p1.diff(file1)

