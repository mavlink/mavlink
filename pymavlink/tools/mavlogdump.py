#!/usr/bin/env python

'''
example program that dumps a Mavlink log file. The log file is
assumed to be in the format that qgroundcontrol uses, which consists
of a series of MAVLink packets, each with a 64 bit timestamp
header. The timestamp is in microseconds since 1970 (unix epoch)
'''

import sys, time, os, struct, json

from argparse import ArgumentParser
parser = ArgumentParser(description=__doc__)

parser.add_argument("--no-timestamps", dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
parser.add_argument("--planner", action='store_true', help="use planner file format")
parser.add_argument("--robust", action='store_true', help="Enable robust parsing (skip over bad data)")
parser.add_argument("-f", "--follow", action='store_true', help="keep waiting for more data at end of file")
parser.add_argument("--condition", default=None, help="select packets by condition")
parser.add_argument("-q", "--quiet", action='store_true', help="don't display packets")
parser.add_argument("-o", "--output", default=None, help="output matching packets to give file")
parser.add_argument("-p", "--parms", action='store_true', help="preserve parameters in output with -o")
parser.add_argument("--format", default=None, help="Change the output format between 'standard', 'json', and 'csv'. For the CSV output, you must supply types that you want.")
parser.add_argument("--csv_sep", dest="csv_sep", default=",", help="Select the delimiter between columns for the output CSV file. Use 'tab' to specify tabs. Only applies when --format=csv")
parser.add_argument("--types", default=None, help="types of messages (comma separated)")
parser.add_argument("--dialect", default="ardupilotmega", help="MAVLink dialect")
parser.add_argument("--zero-time-base", action='store_true', help="use Z time base for DF logs")
parser.add_argument("log", metavar="LOG")
args = parser.parse_args()

import inspect

from pymavlink import mavutil


filename = args.log
mlog = mavutil.mavlink_connection(filename, planner_format=args.planner,
                                  notimestamps=args.notimestamps,
                                  robust_parsing=args.robust,
                                  dialect=args.dialect,
                                  zero_time_base=args.zero_time_base)

output = None
if args.output:
    output = open(args.output, mode='wb')

types = args.types
if types is not None:
    types = types.split(',')

ext = os.path.splitext(filename)[1]
isbin = ext in ['.bin', '.BIN']
islog = ext in ['.log', '.LOG']

if args.csv_sep == "tab":
    args.csv_sep = "\t"

# Write out a header row as we're outputting in CSV format.
fields = ['timestamp']
offsets = {}
if args.format == 'csv':
    try:
        currentOffset = 1 # Store how many fields in we are for each message.
        for type in types:
            try:
                typeClass = "MAVLink_{0}_message".format(type.lower())
                fields += [type + '.' + x for x in inspect.getargspec(getattr(mavutil.mavlink, typeClass).__init__).args[1:]]
                offsets[type] = currentOffset
                currentOffset += len(fields)
            except IndexError:
                quit()
    except TypeError:
        print("You must specify a list of message types if outputting CSV format via the --types argument.")
        exit()

    # The first line output are names for all columns
    print(','.join(fields))

# Track the last timestamp value. Used for compressing data for the CSV output format.
last_timestamp = None

# Keep track of data from the current timestep. If the following timestep has the same data, it's stored in here as well. Output should therefore have entirely unique timesteps.
csv_out = ["" for x in fields]
while True:
    m = mlog.recv_match(blocking=args.follow)
    if m is None:
        # FIXME: Make sure to output the last CSV message before dropping out of this loop
        break
    if output is not None:
        if (isbin or islog) and m.get_type() == "FMT":
            output.write(m.get_msgbuf())
            continue
        if (isbin or islog) and (m.get_type() == "PARM" and args.parms):
            output.write(m.get_msgbuf())
            continue
        if m.get_type() == 'PARAM_VALUE' and args.parms:
            timestamp = getattr(m, '_timestamp', None)
            output.write(struct.pack('>Q', timestamp*1.0e6) + m.get_msgbuf())
            continue
    if not mavutil.evaluate_condition(args.condition, mlog.messages):
        continue

    if types is not None and m.get_type() not in types and m.get_type() != 'BAD_DATA':
        continue

    if m.get_type() == 'BAD_DATA' and m.reason == "Bad prefix":
        continue

    # Grab the timestamp.
    timestamp = getattr(m, '_timestamp', 0.0)

    # If we're just logging, pack in the timestamp and data into the output file.
    if output:
        if not (isbin or islog):
            output.write(struct.pack('>Q', timestamp*1.0e6))
        output.write(m.get_msgbuf())

    # If quiet is specified, don't display output to the terminal.
    if args.quiet:
        continue

    # If JSON was ordered, serve it up. Split it nicely into metadata and data.
    if args.format == 'json':
        # Format our message as a Python dict, which gets us almost to proper JSON format
        data = m.to_dict()

        # Remove the mavpackettype value as we specify that later.
        del data['mavpackettype']

        # Also, if it's a BAD_DATA message, make it JSON-compatible by removing array objects
        if 'data' in data and type(data['data']) is not dict:
            data['data'] = list(data['data'])

        # Prepare the message as a single object with 'meta' and 'data' keys holding
        # the message's metadata and actual data respectively.
        outMsg = {"meta": {"msgId": m.get_msgId(), "type": m.get_type(), "timestamp": timestamp}, "data": data}

        # Now print out this object with stringified properly.
        print(json.dumps(outMsg))
    # CSV format outputs columnar data with a user-specified delimiter
    elif args.format == 'csv':
        data = m.to_dict()
        type = m.get_type()

        # If this message has a duplicate timestamp, copy its data into the existing data list. Also
        # do this if it's the first message encountered.
        if timestamp == last_timestamp or last_timestamp is None:
            newData = [str(data[y.split('.')[-1]]) if y.split('.')[0] == type and y.split('.')[-1] in data else "" for y in fields]
            for i, val in enumerate(newData):
                if val:
                    csv_out[i] = val

        # Otherwise if this is a new timestamp, print out the old output data, and store the current message for later output.
        else:
            csv_out[0] = "{:.8f}".format(last_timestamp)
            print(args.csv_sep.join(csv_out))
            csv_out = [str(data[y.split('.')[-1]]) if y.split('.')[0] == type and y.split('.')[-1] in data else "" for y in fields]
    # Otherwise we output in a standard Python dict-style format
    else:
        print("%s.%02u: %s" % (
            time.strftime("%Y-%m-%d %H:%M:%S",
                          time.localtime(timestamp)),
                          int(timestamp*100.0)%100, m))

    # Update our last timestamp value.
    last_timestamp = timestamp

