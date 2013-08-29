#!/usr/bin/env python

'''
fit best estimate of magnetometer rotation to gyro data
'''

import sys, time, os, math

from optparse import OptionParser
parser = OptionParser("magfit_rotation_gyro.py [options]")
parser.add_option("--no-timestamps",dest="notimestamps", action='store_true', help="Log doesn't have timestamps")

(opts, args) = parser.parse_args()

from pymavlink import mavutil
from pymavlink.rotmat import Vector3, Matrix3
from math import radians, degrees

if len(args) < 1:
    print("Usage: magfit_rotation_gyro.py [options] <LOGFILE...>")
    sys.exit(1)

class Rotation(object):
    def __init__(self, roll, pitch, yaw, r):
        self.roll = roll
        self.pitch = pitch
        self.yaw = yaw
        self.r = r

def in_rotations_list(rotations, m):
    for r in rotations:
        m2 = m.transposed() * r.r
        (r, p, y) = m2.to_euler()
        if (abs(r) < radians(1) and
            abs(p) < radians(1) and
            abs(y) < radians(1)):
            return True
    return False

def generate_rotations():
    '''generate all 90 degree rotations'''
    rotations = []
    for yaw in [0, 90, 180, 270]:
        for pitch in [0, 90, 180, 270]:
            for roll in [0, 90, 180, 270]:
                m = Matrix3()
                m.from_euler(radians(roll), radians(pitch), radians(yaw))
                if not in_rotations_list(rotations, m):
                    rotations.append(Rotation(roll, pitch, yaw, m))
    return rotations

def add_errors(mag, gyr, last_mag, deltat, total_error, rotations):
    for i in range(len(rotations)):
        r = rotations[i].r
        m = Matrix3()
        m.rotate(gyr * deltat)
        rmag1 = r * last_mag
        rmag2 = r * mag
        rmag3 = m.transposed() * rmag1
        err = rmag3 - rmag2
        total_error[i] += err.length()
        

def magfit(logfile):
    '''find best magnetometer rotation fit to a log file'''

    print("Processing log %s" % filename)
    mlog = mavutil.mavlink_connection(filename, notimestamps=opts.notimestamps)

    # generate 90 degree rotations
    rotations = generate_rotations()
    print("Generated %u rotations" % len(rotations))

    last_mag = None
    last_usec = 0
    count = 0
    total_error = [0]*len(rotations)

    # now gather all the data
    while True:
        m = mlog.recv_match()
        if m is None:
            break
        if m.get_type() == "RAW_IMU":
            mag = Vector3(m.xmag, m.ymag, m.zmag)
            gyr = Vector3(m.xgyro, m.ygyro, m.zgyro) * 0.001
            usec = m.time_usec
            if last_mag is not None and gyr.length() > radians(5):
                add_errors(mag, gyr, last_mag, (usec - last_usec)*1.0e-6, total_error, rotations)
            last_mag = mag
            last_usec = usec
            count += 1

    best_i = 0
    best_err = total_error[0]
    for i in range(len(rotations)):
        r = rotations[i]
        print("(%u,%u,%u) err=%.2f" % (
            r.roll,
            r.pitch,
            r.yaw,
            total_error[i]/count))
        if total_error[i] < best_err:
            best_i = i
            best_err = total_error[i]
    r = rotations[best_i]
    print("Best rotation (%u,%u,%u) err=%.2f" % (
        r.roll,
        r.pitch,
        r.yaw,
        best_err/count))

for filename in args:
    magfit(filename)
