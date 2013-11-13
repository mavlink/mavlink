#!/usr/bin/env python

'''
search a set of log files for bad accel values
'''

import sys, time, os, glob
import zipfile

from pymavlink import mavutil

# extra imports for pyinstaller
import json
from pymavlink.dialects.v10 import ardupilotmega

search_dirs = ['c:\Program Files\APM Planner', 
               'c:\Program Files\Mission Planner', 
               'c:\Program Files (x86)\APM Planner',
               'c:\Program Files (x86)\Mission Planner']
results = 'SearchResults.zip'
email = 'Craig Elder <craig@3drobotics.com>'

from optparse import OptionParser
parser = OptionParser("AccelSearch.py [options]")
parser.add_option("--directory", action='append', default=search_dirs, help="directories to search")

(opts, args) = parser.parse_args()

def AccelSearch(filename):
    print("Checking %s ..." % filename)
    mlog = mavutil.mavlink_connection(filename)
    badcount = 0
    badval = None
    while True:
        m = mlog.recv_match(type=['PARAM_VALUE','RAW_IMU'])
        if m is None:
            return False
        if m.get_type() == 'PARAM_VALUE':
            if m.param_id.startswith('INS_PRODUCT_ID'):
                if m.param_value not in [0.0, 5.0]:
                    return False
        if m.get_type() == 'RAW_IMU':
            if abs(m.xacc) >= 3000 and abs(m.yacc) > 3000 and abs(m.zacc) > 3000:
                print m
                return True
            # also look for a single axes that stays nearly constant at a large value
            for axes in ['xacc', 'yacc', 'zacc']:
                value1 = getattr(m, axes)
                if badval is None:
                    badcount = 1
                    badval = m
                    continue
                if abs(value1) > 3000:
                    value2 = getattr(badval, axes)
                    if abs(value1 - value2) < 10:
                        badcount += 1
                        badval = m
                        if badcount > 5:
                            print m
                            return True
                    else:
                        badcount = 1
                        badval = m
        
found = []
directories = opts.directory

# allow drag and drop
if len(sys.argv) > 1:
    directories = sys.argv[1:]

for d in directories:
    if not os.path.exists(d):
        continue
    if os.path.isdir(d):
        print("Searching in %s" % d)
        for (root, dirs, files) in os.walk(d):
            for f in files:
                if not f.endswith('.tlog'):
                    continue
                path = os.path.join(root, f)
                if AccelSearch(path):
                    found.append(path)
    elif d.endswith('.tlog') and AccelSearch(d):
        found.append(d)
        

if len(found) == 0:
    print("No matching files found - all OK!")
    input('Press enter to close')
    sys.exit(0)

print("Creating zip file %s" % results)
try:
    zip = zipfile.ZipFile(results, 'w')
except Exception:
    print("Unable to create zip file %s" % results)
    print("Please send matching files manually")
    for f in found:
        print('MATCHED: %s' % f)
    input('Press enter to close')
    sys.exit(1)

for f in found:
    zip.write(f, arcname=os.path.basename(f))
zip.close()
print('==============================================')
print("Created %s with %u matching logs" % (results, len(found)))
print("Please send this file to %s" % email)
print('==============================================')

input('Press enter to close')
sys.exit(0)
