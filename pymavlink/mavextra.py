#!/usr/bin/env python
'''
useful extra functions for use by mavlink clients

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later
'''

from math import *


def norm_heading(RAW_IMU, ATTITUDE, declination):
    '''calculate heading from RAW_IMU and ATTITUDE'''
    xmag = RAW_IMU.xmag
    ymag = RAW_IMU.ymag
    zmag = RAW_IMU.zmag
    pitch = ATTITUDE.pitch
    roll  = ATTITUDE.roll
    
    headX = xmag*cos(pitch) + ymag*sin(roll)*sin(pitch) + zmag*cos(roll)*sin(pitch)
    headY = ymag*cos(roll) - zmag*sin(roll)
    heading = atan2(-headY, headX)
    heading = fmod(degrees(heading) + declination + 360, 360)
    return heading

def TrueHeading(SERVO_OUTPUT_RAW):
    rc3_min = 1060
    rc3_max = 1850
    p = float(SERVO_OUTPUT_RAW.servo3_raw - rc3_min) / (rc3_max - rc3_min)
    return 172 + (1.0-p)*(326 - 172)

def kmh(mps):
    '''convert m/s to Km/h'''
    return mps*3.6

def altitude(press_abs, ground_press=955.0, ground_temp=30):
    '''calculate barometric altitude'''
    return log(ground_press/press_abs)*(ground_temp+273.15)*29271.267*0.001


def mag_heading(RAW_IMU, ATTITUDE, declination):
    mag_x = RAW_IMU.xmag
    mag_y = RAW_IMU.ymag
    mag_z = RAW_IMU.zmag

    headX = mag_x*cos(ATTITUDE.pitch) + mag_y*sin(ATTITUDE.roll)*sin(ATTITUDE.pitch) + mag_z*cos(ATTITUDE.roll)*sin(ATTITUDE.pitch)
    headY = mag_y*cos(ATTITUDE.roll) - mag_z*sin(ATTITUDE.roll)
    heading = degrees(atan2(-headY,headX)) + declination
    if heading < 0:
        heading += 360
    return heading

def mag_heading2(RAW_IMU, ATTITUDE, SENSOR_OFFSETS, xofs, yofs, zofs, declination):
    from numpy import dot
    mag_x = RAW_IMU.xmag - SENSOR_OFFSETS.mag_ofs_x
    mag_y = RAW_IMU.ymag - SENSOR_OFFSETS.mag_ofs_y
    mag_z = RAW_IMU.zmag - SENSOR_OFFSETS.mag_ofs_z

    a = [[1.0, -0.085552080501107491, 0.25004786935177803], [0.10804184802593279, 0.72383183408994123, 0.0008465509310943448], [-0.33378127013801168, 0.002222017405852832, 0.93692648558215874]]


#    a = [[1,0,0],[0,1,0],[0,0,1]]

    mag_x += xofs
    mag_y += yofs
    mag_z += zofs

    mv = [mag_x, mag_y, mag_z]
    mv2 = dot(a, mv)

    mag_x = mv2[0]
    mag_y = mv2[1]
    mag_z = mv2[2]

    headX = mag_x*cos(ATTITUDE.pitch) + mag_y*sin(ATTITUDE.roll)*sin(ATTITUDE.pitch) + mag_z*cos(ATTITUDE.roll)*sin(ATTITUDE.pitch)
    headY = mag_y*cos(ATTITUDE.roll) - mag_z*sin(ATTITUDE.roll)
    heading = degrees(atan2(-headY,headX)) + declination
    if heading < 0:
        heading += 360
    return heading

def angle_diff(angle1, angle2):
    ret = angle1 - angle2
    if ret > 180:
        ret -= 360;
    if ret < -180:
        ret += 360
    return ret

