#!/usr/bin/env python

'''
convert a MAVLink tlog file to a MATLab mfile
'''

import sys
import re
from pymavlink import mavutil

def create_mfile(filename, msg_types, msg_lists):
    '''create a .m file from msg_types and msg_lists'''
    f = open(filename, mode='w')

    # put out all the headings
    for mtype in msg_types:
        m = msg_types[mtype]
        fieldnames = m._fieldnames
        f.write("%s.heading = {'timestamp'" % mtype)
        for field in fieldnames:
            val = getattr(m, field)
            if not isinstance(val, str):
                f.write(",'%s'" % field)
        f.write("};\n")
    for mtype in msg_lists:
        f.write("%s.data = [" % mtype)
        for m in msg_lists[mtype]:
            fieldnames = m._fieldnames

            # always include the timestamp
            f.write("%f" % m._timestamp)
            for field in fieldnames:
                val = getattr(m, field)
                if not isinstance(val, str):
                    f.write(",%f" % val)
            f.write(";\n")
        f.write("];\n\n")

    f.close()

def process_tlog(filename):
    '''convert a tlog to a .m file'''

    print("Processing %s" % filename)
    
    mlog = mavutil.mavlink_connection(filename, dialect=opts.dialect)
    
    # first walk the entire file, grabbing all messages into a hash of lists,
    #and the first message of each type into a hash
    msg_types = {}
    msg_lists = {}

    types = opts.types
    if types is not None:
        types = types.split(',')

    while True:
        m = mlog.recv_match(condition=opts.condition)
        if m is None:
            break

        if types is not None and m.get_type() not in types:
            continue
        if m.get_type() == 'BAD_DATA':
            continue
        
        if m.get_type() not in msg_lists.keys():
            msg_lists[m.get_type()] = []
            msg_types[m.get_type()] = m

        msg_lists[m.get_type()].append(m)

    # note that Octave doesn't like any extra '.', '*', '-', characters in the filename
    basename = '.'.join(filename.split('.')[:-1])
    mfilename = re.sub('[\.\-\+\*]','_', basename) + '.m'
    print("Creating %s" % mfilename)
    create_mfile(mfilename, msg_types, msg_lists)

from optparse import OptionParser
parser = OptionParser("mavtomfile.py [options]")

parser.add_option("--condition",dest="condition", default=None, help="select packets by condition")
parser.add_option("-o", "--output", default=None, help="output filename")
parser.add_option("--types",  default=None, help="types of messages (comma separated)")
parser.add_option("--dialect",  default="ardupilotmega", help="MAVLink dialect")
(opts, args) = parser.parse_args()

if len(args) < 1:
    print("Usage: mavtomfile.py [options] <LOGFILE>")
    sys.exit(1)

for filename in args:
    process_tlog(filename)
