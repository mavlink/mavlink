#!/usr/bin/env python2

'''
simple kml export for logfiles
Thomas Gubler <thomasgubler@gmail.com>
'''

from optparse import OptionParser
import simplekml
from pymavlink.mavextra import *
from pymavlink import mavutil
import time
import re

#xxx: also global position
gps_fields = ['GPS.Lon', 'GPS.Lat', 'GPS.Alt'] #kml order is lon, lat
gps_data = [None for n in gps_fields]

kml = simplekml.Kml()
kml_linestrings = [kml.newlinestring(name='GPS', altitudemode='absolute')]
kml_linestyles = [simplekml.LineStyle(color=simplekml.Color.blue)]

#xxx altitude
def add_to_linestring(data, kml_linestring):
    '''add a point to the kml file'''
    global kml
#     v = [data['GPS.Lat'], data['GPS.Lon'], data['GPS.Alt']]
# #     print v
#     pnt = kml.newpoint(coords=(v))
#     pnt.altitudemode = 'absolute'
    kml_linestring.coords.addcoordinates([gps_data])

def style_kml(linestrings, linestyles, extrude):
    '''applies the styles to the linestrings'''
    assert len(linestrings) == len(linestyles)
    
    if extrude:
        for l in linestrings:
            l.extrude = 1   

    for i,v in enumerate(linestrings):
        v.style.linestyle = linestyles[i]

def save_kml(filename):
    '''saves the kml file'''
    global kml
    kml.save(filename)

#test
# exportKML([(47.5, 8.), ((48., 9.))], "test.kml")

def add_data(t, msg, vars):
    '''add some data'''
    global gps_data
    
    mtype = msg.get_type()
    if mtype not in msg_types:
        return
    
    for i in range(0, len(fields)):
        if mtype not in field_types[i]:
            continue
        f = fields[i]
        v = mavutil.evaluate_expression(f, vars)
        if v is None:
            continue
        
        gps_data[gps_fields.index(f)] = v
    
    #check if we have a full gps measurement
    gps_measurement_ready = True;
    for v in gps_data:
        if v == None:
            gps_measurement_ready = False
    if gps_measurement_ready:
        add_to_linestring(gps_data, kml_linestrings[0])
        gps_data = [None for n in gps_data] #reset gps_data
        
            

def process_file(filename):
    '''process one file'''
    print("Processing %s" % filename)
    mlog = mavutil.mavlink_connection(filename, notimestamps=opts.notimestamps)
    vars = {}
     
    while True:
        msg = mlog.recv_match(opts.condition)
        if msg is None: break
        tdays = (msg._timestamp - time.timezone) / (24 * 60 * 60)
        tdays += 719163 # pylab wants it since 0001-01-01
        add_data(tdays, msg, mlog.messages)

if __name__ == '__main__':
    parser = OptionParser("mavkml.py [options] <filename>")
    parser.add_option("--no-timestamps",dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
    parser.add_option("--condition",dest="condition", default=None, help="select packets by a condition")
    parser.add_option("--a",dest="aoff", default=0, help="Altitude offset for paths that go through the ground in google earth")
    parser.add_option("-o",dest="filename_out", default="mavkml.kml", help="output filename")
    parser.add_option("-e",dest="extrude", action='store_true', help="extrude paths to ground")
    
    
    (opts, args) = parser.parse_args()
        
    if len(args) < 1:
        print("Usage: mavkml.py <LOGFILES...>")
        sys.exit(1)
    
    filenames = []
    for f in args:
        if os.path.exists(f):
            filenames.append(f)
        
    fields = gps_fields
    field_types = []
    msg_types = set()
    re_caps = re.compile('[A-Z_][A-Z0-9_]+')
    
    for f in fields:
        caps = set(re.findall(re_caps, f))
        msg_types = msg_types.union(caps)
        field_types.append(caps)
 
    if len(filenames) == 0:
        print("No files to process")
        sys.exit(1)
     
    for fi in range(0, len(filenames)):
        f = filenames[fi]
        process_file(f)
    
    style_kml(kml_linestrings, kml_linestyles, opts.extrude)
    save_kml(opts.filename_out)