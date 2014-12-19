#!/usr/bin/env python

'''
search a set of log files for signs of inconsistent IMU data
'''

import sys, time, os, glob
import zipfile

from pymavlink import mavutil
from math import degrees

# extra imports for pyinstaller
import json
from pymavlink.dialects.v10 import ardupilotmega

search_dirs = ['c:\Program Files\APM Planner',
               'c:\Program Files\Mission Planner',
               'c:\Program Files (x86)\APM Planner',
               'c:\Program Files (x86)\Mission Planner']
results = 'SearchResults.zip'
email = 'Craig Elder <craig@3drobotics.com>'

def IMUCheckFail(filename):
    try:
        mlog = mavutil.mavlink_connection(filename)
    except Exception:
        return False

    accel1 = None
    accel2 = None
    gyro1 = None
    gyro2 = None
    t1 = 0
    t2 = 0
    ecount_accel = [0]*3
    ecount_gyro = [0]*3
    athreshold = 3.0
    gthreshold = 30.0
    count_threshold = 100
    imu1_count = 0
    imu2_count = 0
    
    while True:
        try:
            m = mlog.recv_match(type=['RAW_IMU','SCALED_IMU2','IMU','IMU2'])
        except Exception as e:
            print('Error: %s' % e)
            break
        if m is None:
            break
        mtype = m.get_type()
        gotimu2 = False
        if mtype == 'RAW_IMU':
            accel1 = [m.xacc*9.81*0.001, m.yacc*9.81*0.001, m.zacc*9.81*0.001]
            gyro1  = [degrees(m.xgyro*0.001), degrees(m.ygyro*0.001), degrees(m.zgyro*0.001)]
            t1 = m.time_usec/1000
            imu1_count += 1
        elif mtype == 'SCALED_IMU2':
            accel2 = [m.xacc*9.81*0.001, m.yacc*9.81*0.001, m.zacc*9.81*0.001]
            gyro2  = [degrees(m.xgyro*0.001), degrees(m.ygyro*0.001), degrees(m.zgyro*0.001)]
            gotimu2 = True
            t2 = m.time_boot_ms
            imu2_count += 1
        elif mtype == 'IMU':
            accel1 = [m.AccX, m.AccY, m.AccZ]
            gyro1  = [m.GyrX, m.GyrY, m.GyrZ]
            t1 = m.TimeMS
            imu1_count += 1
        elif mtype == 'IMU2':
            accel2 = [m.AccX, m.AccY, m.AccZ]
            gyro2  = [m.GyrX, m.GyrY, m.GyrZ]
            gotimu2 = True
            t2 = m.TimeMS
            imu2_count += 1

        # skip out early if we don't have two IMUs
        if imu1_count > imu2_count + 100:
            return False
        
        if accel1 is not None and accel2 is not None and gotimu2 and t2 >= t1:
            for i in range(3):
                adiff = accel1[i] - accel2[i]
                if adiff > athreshold:
                    if ecount_accel[i] < 0:
                        ecount_accel[i] = 0
                    else:
                        ecount_accel[i] += 1
                elif adiff < -athreshold:
                    if ecount_accel[i] > 0:
                        ecount_accel[i] = 0
                    else:
                        ecount_accel[i] -= 1
                gdiff = gyro1[i] - gyro2[i]
                if gdiff > gthreshold:
                    if ecount_gyro[i] < 0:
                        ecount_gyro[i] = 0
                    else:
                        ecount_gyro[i] += 1
                elif gdiff < -gthreshold:
                    if ecount_gyro[i] > 0:
                        ecount_gyro[i] = 0
                    else:
                        ecount_gyro[i] -= 1
                if abs(ecount_accel[i]) > count_threshold:
                    print("acceldiff[%u] %.1f" % (i, adiff))
                    print(m)
                    return True
                if abs(ecount_gyro[i]) > count_threshold:
                    print("gyrodiff[i] %.1f" % (i, adiff))
                    print(m)
                    return True
        
    return False

found = []
directories = sys.argv[1:]
if not directories:
    directories = search_dirs
    
filelist = []

extensions = ['.tlog','.bin']

def match_extension(f):
    '''see if the path matches a extension'''
    (root,ext) = os.path.splitext(f)
    return ext.lower() in extensions

for d in directories:
    if not os.path.exists(d):
        continue
    if os.path.isdir(d):
        print("Searching in %s" % d)
        for (root, dirs, files) in os.walk(d):
            for f in files:
                if not match_extension(f):
                    continue
                path = os.path.join(root, f)
                filelist.append(path)
    elif match_extension(d):
        filelist.append(d)

for i in range(len(filelist)):
    f = filelist[i]
    print("Checking %s ... [found=%u i=%u/%u]" % (f, len(found), i, len(filelist)))
    try:
        if IMUCheckFail(f):
            found.append(f)
    except Exception as e:
        print("Failed - %s" % e)
        continue
    sys.stdout.flush()

if len(found) == 0:
    print("No matching files found - all OK!")
    raw_input('Press enter to close')
    sys.exit(0)

print("Creating zip file %s" % results)
try:
    zip = zipfile.ZipFile(results, 'w')
except Exception:
    print("Unable to create zip file %s" % results)
    print("Please send matching files manually")
    for f in found:
        print('MATCHED: %s' % f)
    raw_input('Press enter to close')
    sys.exit(1)

for f in found:
    zip.write(f, arcname=os.path.basename(f))
zip.close()

print('==============================================')
print("Created %s with %u of %u matching logs" % (results, len(found), len(filelist)))
print("Please send this file to %s" % email)
print('==============================================')

raw_input('Press enter to close')
sys.exit(0)
