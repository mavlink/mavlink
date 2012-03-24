#!/usr/bin/env python

'''
fit best estimate of magnetometer offsets
'''

import sys, time, os, math

# allow import from the parent directory, where mavlink.py is
sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))

from optparse import OptionParser
parser = OptionParser("magfit.py [options]")
parser.add_option("--no-timestamps",dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
parser.add_option("--condition",dest="condition", default=None, help="select packets by condition")
parser.add_option("--mav10", action='store_true', default=False, help="Use MAVLink protocol 1.0")

(opts, args) = parser.parse_args()

if opts.mav10:
    os.environ['MAVLINK10'] = '1'
import mavutil

if len(args) < 1:
    print("Usage: magfit.py [options] <LOGFILE...>")
    sys.exit(1)

def select_data(data):
    ret = []
    counts = {}
    for d in data:
        x,y,z = d
        key = "%u:%u:%u" % (x/20,y/20,z/20)
        if key in counts:
            counts[key] += 1
        else:
            counts[key] = 1
        if counts[key] < 3:
            ret.append(d)
    print(len(data), len(ret))
    return ret

def radius(data, offsets):
    '''return radius give data point and offsets'''
    x,y,z = data
    return math.sqrt((x+offsets[0])**2 + (y+offsets[1])**2 + (z+offsets[2])**2)

def radius_cmp(a, b, offsets):
    '''return radius give data point and offsets'''
    diff = radius(a, offsets) - radius(b, offsets)
    if diff > 0:
        return 1
    if diff < 0:
        return -1
    return 0

def sphere_error(p, data):
    from scipy import sqrt
    xofs,yofs,zofs,r = p
    ret = []
    for d in data:
        x,y,z = d
        err = r - sqrt((x+xofs)**2 + (y+yofs)**2 + (z+zofs)**2)
        ret.append(err)
    return ret

def fit_data(data):
    import numpy, scipy
    from scipy import optimize

    p0 = [0.0, 0.0, 0.0, 0.0]
    p1, ier = optimize.leastsq(sphere_error, p0[:], args=(data))
    if not ier in [1, 2, 3, 4]:
        raise RuntimeError("Unable to find solution")
    return p1

def magfit(logfile):
    '''find best magnetometer offset fit to a log file'''

    print("Processing log %s" % filename)
    mlog = mavutil.mavlink_connection(filename, notimestamps=opts.notimestamps)

    data = []

    last_t = 0

    # get the current mag offsets
    m = mlog.recv_match(type='SENSOR_OFFSETS',condition=opts.condition)
    offsets = (m.mag_ofs_x, m.mag_ofs_y, m.mag_ofs_z)

    # now gather all the data
    while True:
        m = mlog.recv_match(condition=opts.condition)
        if m is None:
            break
        if m.get_type() == "SENSOR_OFFSETS":
            # update current offsets
            offsets = (m.mag_ofs_x, m.mag_ofs_y, m.mag_ofs_z)
        if m.get_type() == "RAW_IMU":
            # add data point after subtracting the current offsets
            data.append((m.xmag - offsets[0], m.ymag - offsets[1], m.zmag - offsets[2]))

    print("Extracted %u data points" % len(data))
    print("Current offsets: ( %6.1f %6.1f %6.1f )" % (offsets[0], offsets[1], offsets[2]))

    data = select_data(data)

    # do an initial fit with all data
    ofs = fit_data(data)
    offsets = ofs[0:3]
    field_strength = ofs[3]

    for count in range(3):
        # sort the data by the radius
        data.sort(lambda a,b : radius_cmp(a,b,offsets))

        print("Fit %u    : ( %6.1f %6.1f %6.1f )  field_strength=%6.1f to %6.1f" % (
            count, offsets[0], offsets[1], offsets[2],
            radius(data[0], offsets), radius(data[-1], offsets)))
        
        # discard outliers, keep the middle 3/4
        data = data[len(data)/8:-len(data)/8]

        # fit again
        ofs = fit_data(data)
        offsets = ofs[0:3]
        field_strength = ofs[3]

    print("Final    : ( %6.1f %6.1f %6.1f )  field_strength=%6.1f to %6.1f" % (
        offsets[0], offsets[1], offsets[2],
        radius(data[0], offsets), radius(data[-1], offsets)))

total = 0.0
for filename in args:
    magfit(filename)
