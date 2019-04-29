#!/usr/bin/env python

'''
extract ISBH and ISBD messages from AP_Logging files and produce FFT plots
'''
from __future__ import print_function

import numpy
import os
import pylab
import sys
import time

from argparse import ArgumentParser
parser = ArgumentParser(description=__doc__)
parser.add_argument("--condition", default=None, help="select packets by condition")
parser.add_argument("logs", metavar="LOG", nargs="+")

args = parser.parse_args()

from pymavlink import mavutil

def mavfft_fttd(logfile):
    '''display fft for raw ACC data in logfile'''

    '''object to store data about a single FFT plot'''
    class PlotData(object):
        def __init__(self, ffth):
            self.seqno = -1
            self.fftnum = ffth.N
            self.sensor_type = ffth.type
            self.instance = ffth.instance
            self.sample_rate_hz = ffth.smp_rate
            self.multiplier = ffth.mul
            self.data = {}
            self.data["X"] = []
            self.data["Y"] = []
            self.data["Z"] = []
            self.holes = False
            self.freq = None

        def add_fftd(self, fftd):
            if fftd.N != self.fftnum:
                print("Skipping ISBD with wrong fftnum (%u vs %u)\n" % (fftd.fftnum, self.fftnum))
                return
            if self.holes:
                print("Skipping ISBD(%u) for ISBH(%u) with holes in it" % (fftd.seqno, self.fftnum))
                return
            if fftd.seqno != self.seqno+1:
                print("ISBH(%u) has holes in it" % fftd.N)
                self.holes = True
                return
            self.seqno += 1
            self.data["X"].extend(fftd.x)
            self.data["Y"].extend(fftd.y)
            self.data["Z"].extend(fftd.z)

        def prefix(self):
            if self.sensor_type == 0:
                return "Accel"
            elif self.sensor_type == 1:
                return "Gyro"
            else:
                return "?Unknown Sensor Type?"

        def tag(self):
            return str(self)

        def __str__(self):
            return "%s[%u]" % (self.prefix(), self.instance)

    print("Processing log %s" % filename)
    mlog = mavutil.mavlink_connection(filename)

    things_to_plot = []
    plotdata = None
    msgcount = 0
    start_time = time.time()
    while True:
        m = mlog.recv_match(condition=args.condition)
        if m is None:
            break
        msgcount += 1
        if msgcount % 1000 == 0:
            sys.stderr.write(".")
        msg_type = m.get_type()
        if msg_type == "ISBH":
            if plotdata is not None:
                # close off previous data collection
                things_to_plot.append(plotdata)
            # initialise plot-data collection object
            plotdata = PlotData(m)
            continue

        if msg_type == "ISBD":
            if plotdata is None:
                print("Skipping ISBD outside ISBH (fftnum=%u)\n" % m.N)
                continue
            plotdata.add_fftd(m)

    print("", file=sys.stderr)
    time_delta = time.time() - start_time
    print("%us messages  %u messages/second  %u kB/second" % (msgcount, msgcount/time_delta, os.stat(filename).st_size/time_delta))
    print("Extracted %u fft data sets" % len(things_to_plot), file=sys.stderr)

    sum_fft = {}
    freqmap = {}
    count = 0

    first_freq = None
    for thing_to_plot in things_to_plot:
        for axis in [ "X","Y","Z" ]:
            d = numpy.array(thing_to_plot.data[axis])/float(thing_to_plot.multiplier)
            if len(d) == 0:
                print("No data?!?!?!")
                continue
            
            avg = numpy.sum(d) / len(d)
            d -= avg
            d_fft = numpy.fft.rfft(d)
            if thing_to_plot.tag() not in sum_fft:
                sum_fft[thing_to_plot.tag()] = {
                    "X": 0,
                    "Y": 0,
                    "Z": 0
                }
            sum_fft[thing_to_plot.tag()][axis] = numpy.add(sum_fft[thing_to_plot.tag()][axis], d_fft)
            count += 1
            freq = numpy.fft.rfftfreq(len(d), 1.0/thing_to_plot.sample_rate_hz)
            freqmap[thing_to_plot.tag()] = freq

    for sensor in sum_fft:
        print("Sensor: %s" % str(sensor))
        pylab.figure(str(sensor))
        for axis in [ "X","Y","Z" ]:
            pylab.plot(freqmap[sensor], numpy.abs(sum_fft[sensor][axis]/count), label=axis)
        pylab.legend(loc='upper right')
        pylab.xlabel('Hz')

for filename in args.logs:
    mavfft_fttd(filename)

pylab.show()
