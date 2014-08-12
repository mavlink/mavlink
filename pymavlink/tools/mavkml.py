#!/usr/bin/env python

'''
simple kml export for logfiles
Thomas Gubler <thomasgubler@gmail.com>
'''

from argparse import ArgumentParser
import simplekml
from pymavlink.mavextra import *
from pymavlink import mavutil
import time
import re

mainstate_field = 'STAT.MainState'
position_field_types = ['Lon', 'Lat', 'Alt']  # kml order is lon, lat

colors = [simplekml.Color.red, simplekml.Color.green, simplekml.Color.blue,
          simplekml.Color.violet, simplekml.Color.yellow,
          simplekml.Color.orange]

kml = simplekml.Kml()
kml_linestrings = []


def add_to_linestring(position_data, kml_linestring):
    '''add a point to the kml file'''
    global kml

    # add altitude offset
    position_data[2] += float(args.aoff)
    kml_linestring.coords.addcoordinates([position_data])


def save_kml(filename):
    '''saves the kml file'''
    global kml
    kml.save(filename)
    print("KML written to %s" % filename)


def add_data(t, msg, vars, fields, field_types):
    '''add some data'''

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

        # Check if we have position or state information
        if f == mainstate_field:
            # Handle main state information
            # add_data.mainstate_current >= 0 : avoid starting a new linestring
            # when mainstate comes after the first position data in the log
            if (v != add_data.mainstate_current and
                    add_data.mainstate_current >= 0):
                add_data.new_linestring = True
            add_data.mainstate_current = v
        else:
            # Handle position information
            # make sure lon, lat, alt is saved in the correct order in
            # position_data (the same as in position_field_types)
            add_data.position_data[i] = v

    # check if we have a full gps measurement
    gps_measurement_ready = True
    for v in add_data.position_data:
        if v is None:
            gps_measurement_ready = False
    if not gps_measurement_ready:
        return

    # if new line string is needed (because of state change): add previous
    # linestring to kml_linestrings list, add a new linestring to the kml
    # multigeometry and append to the new linestring
    # else: append to current linestring
    if add_data.new_linestring:
        if add_data.current_kml_linestring is not None:
            kml_linestrings.append(add_data.current_kml_linestring)

        name = "".join([args.source, ":", str(add_data.mainstate_current)])
        add_data.current_kml_linestring = \
            kml.newlinestring(name=name, altitudemode='absolute')

        # set rendering options
        if args.extrude:
            add_data.current_kml_linestring.extrude = 1
        add_data.current_kml_linestring.style.linestyle.color = \
            colors[max([add_data.mainstate_current, 0])]

        add_data.new_linestring = False

    add_to_linestring(add_data.position_data,
                      add_data.current_kml_linestring)

    # reset position_data
    add_data.position_data = [None for n in position_field_types]


def process_file(filename, fields, field_types):
    '''process one file'''
    print("Processing %s" % filename)
    mlog = mavutil.mavlink_connection(filename, notimestamps=args.notimestamps)
    add_data.new_linestring = True
    add_data.mainstate_current = -1
    add_data.current_kml_linestring = None
    add_data.position_data = [None for n in position_field_types]

    while True:
        msg = mlog.recv_match(args.condition)
        if msg is None:
            break
        tdays = (msg._timestamp - time.timezone) / (24 * 60 * 60)
        tdays += 719163  # pylab wants it since 0001-01-01
        add_data(tdays, msg, mlog.messages, fields, field_types)

if __name__ == '__main__':
    parser = ArgumentParser(description=__doc__)
    parser.add_argument("--no-timestamps", dest="notimestamps",
                      action='store_true', help="Log doesn't have timestamps")
    parser.add_argument("--condition", default=None,
                      help="select packets by a condition [default: %(default)s]")
    parser.add_argument("--aoff", default=0.,
                      help="Altitude offset for paths that go through the"
                      "ground in google earth [default: %(default)s]")
    parser.add_argument("-o", "--output", dest="filename_out", default="mav.kml",
                      help="Output filename [default: %(default)s] ")
    parser.add_argument("-s", "--source", default="GPOS",
                      help="Select position data source"
                      "(GPOS or GPS) [default: %(default)s]")
    parser.add_argument("-e", "--extrude", default=False,
                      action='store_true',
                      help="Extrude paths to ground [default: %(default)s]")
    parser.add_argument("logs", metavar="LOG", nargs="+")

    args = parser.parse_args()

    filenames = []
    for f in args.logs:
        if os.path.exists(f):
            filenames.append(f)

    # init fields and field_types lists
    fields = [args.source + "." + s for s in position_field_types]
    fields.append(mainstate_field)
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
        process_file(f, fields, field_types)

    save_kml(args.filename_out)
