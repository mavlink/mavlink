#!/usr/bin/env python

'''
check bandwidth of link
'''

import sys, struct, time, os

import devpath
import pymavlink.mavutil as mavutil

from optparse import OptionParser
parser = OptionParser("bwtest.py [options]")

parser.add_option("--baudrate", dest="baudrate", type='int',
                  help="master port baud rate", default=115200)
parser.add_option("--device", dest="device", default=None, help="serial device")
(opts, args) = parser.parse_args()

if opts.device is None:
    print("You must specify a serial device")
    sys.exit(1)

# create a mavlink serial instance
master = mavutil.mavlink_connection(opts.device, baud=opts.baudrate)

t1 = time.time()

counts = {}

bytes_sent = 0
bytes_recv = 0

while True:
    master.mav.heartbeat_send(1, 1, 1, 1, 1, 1)
    master.mav.sys_status_send(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13)
    master.mav.gps_raw_int_send(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    master.mav.attitude_send(1, 2, 3, 4, 5, 6, 7)
    master.mav.vfr_hud_send(1, 2, 3, 4, 5, 6)
    while master.port.inWaiting() > 0:
        m = master.recv_msg()
        if m == None: break
        if m.get_type() not in counts:
            counts[m.get_type()] = 0
        counts[m.get_type()] += 1
    t2 = time.time()
    if t2 - t1 > 1.0:
        print("%u sent, %u received, %u errors bwin=%.1f kB/s bwout=%.1f kB/s" % (
            master.mav.total_packets_sent,
            master.mav.total_packets_received,
            master.mav.total_receive_errors,
            0.001*(master.mav.total_bytes_received-bytes_recv)/(t2-t1),
            0.001*(master.mav.total_bytes_sent-bytes_sent)/(t2-t1)))
        bytes_sent = master.mav.total_bytes_sent
        bytes_recv = master.mav.total_bytes_received
        t1 = t2
