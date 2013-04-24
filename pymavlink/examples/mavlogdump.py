#!/usr/bin/env python

'''
example program that dumps a Mavlink log file. The log file is
assumed to be in the format that qgroundcontrol uses, which consists
of a series of MAVLink packets, each with a 64 bit timestamp
header. The timestamp is in microseconds since 1970 (unix epoch)
'''

import sys, time, os, struct, json

# allow import from the parent directory, where mavlink.py is
sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))

from optparse import OptionParser
parser = OptionParser("mavlogdump.py [options]")

parser.add_option("--no-timestamps",dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
parser.add_option("--planner",dest="planner", action='store_true', help="use planner file format")
parser.add_option("--robust",dest="robust", action='store_true', help="Enable robust parsing (skip over bad data)")
parser.add_option("-f", "--follow",dest="follow", action='store_true', help="keep waiting for more data at end of file")
parser.add_option("--condition",dest="condition", default=None, help="select packets by condition")
parser.add_option("-q", "--quiet", dest="quiet", action='store_true', help="don't display packets")
parser.add_option("-o", "--output", default=None, help="output matching packets to give file")
parser.add_option("--format", dest="format", default=None, help="Change the output format between 'standard', 'json', and 'csv'. For the CSV output, you must supply types that you want.")
parser.add_option("--csv_sep", dest="csv_sep", default=",", help="Select the delimiter between columns for the ouput CSV file. Use 'tab' to specify tabs. Only applies when --format=csv")
parser.add_option("--zoh", dest="zoh", action='store_true', help="Indicates that data should be used from the previous timestep if there isn't updated data in this timestep. Only applies when --format=csv")
parser.add_option("--types",  default=None, help="types of messages (comma separated)")
(opts, args) = parser.parse_args()

import mavutil

if len(args) < 1:
    print("Usage: mavlogdump.py [options] <LOGFILE>")
    sys.exit(1)

filename = args[0]
mlog = mavutil.mavlink_connection(filename, planner_format=opts.planner,
                                  notimestamps=opts.notimestamps,
                                  robust_parsing=opts.robust)

output = None
if opts.output:
    output = mavutil.mavlogfile(opts.output, write=True)

types = opts.types
if types is not None:
    types = types.split(',')

if opts.csv_sep == "tab":
    opts.csv_sep = "\t"

# Write out a header row is we're outputting in CSV format.
fields = ['timestamp']
offsets = {}
if opts.format == 'csv':
    try:
        currentOffset = 1 # Store how many fields in we are for each message.
        for type in types:
            try:
                typeClass = "MAVLink_{0}_message".format(type.lower())
                fields += [type + '.' + x for x in getattr(mavutil.mavlink, typeClass)._fieldnames]
                offsets[type] = currentOffset
                currentOffset += len(fields)
            except IndexError, e:
                quit()
    except TypeError, e:
        print("You must specify a list of message types if outputting CSV format via the --types argument.")
        exit()
print(opts.csv_sep.join(fields))

lastData = dict.fromkeys(fields, "") # Stores data from previous messages for performing a zero-order-hold with CSV ouput
lastTimestamp = None # Track the last timestamp for merging messages from the same timestamp with CSV output
lastMessageData = {} # Store the values from the last message so that redundant timesteps can be removed
while True:
    # Scan for another MAVLink message quitting if failure occurs.
    m = mlog.recv_match(condition=opts.condition, blocking=opts.follow)
    if m is None:
        break
        
    # Make sure the first timestamp comparison always fails for CSV timestep compression
    if not lastTimestamp:
        lastTimestamp = str(m._timestamp)

    # Filter for any desired messages if necessary
    if types is not None and m.get_type() not in types:
        continue
    
    # If specified, re-output the MAVLink binary stream with timestamps 
    last_timestamp = 0
    if output and m.get_type() != 'BAD_DATA':
        timestamp = getattr(m, '_timestamp', None)
        if not timestamp:
            timestamp = last_timestamp
        last_timestamp = timestamp
        output.write(struct.pack('>Q', timestamp*1.0e6))
        output.write(m.get_msgbuf())

    # Don't output anything if silent mode is enabled
    if opts.quiet:
        continue

    # If JSON was ordered, serve it up. Split it nicely into metadata and data.
    if opts.format == 'json':
        data = m.to_dict()
        del data['mavpackettype']
        outMsg = {"meta": {"msgId": m.get_msgId(), "type": m.get_type(), "timestamp": m._timestamp}, "data": json.dumps(data)}
        print(outMsg)
    # Output CSV
    elif opts.format == 'csv':
        # If this is a new timestep, output the old one
        if str(m._timestamp) != lastTimestamp:
            if opts.zoh:
                out = [str(lastMessageData[y]) if y in lastMessageData else str(lastData[y]) for y in fields]
                lastData.update(lastMessageData)
            else:
                out = [str(lastMessageData[y]) if y in lastMessageData else "" for y in fields]
            out[0] = lastTimestamp
            print(opts.csv_sep.join(out))
            lastMessageData.clear()

        # Format the data into a dictionary keyed by 'MESSAGE_NAME.FIELD_NAME'
        data = m.to_dict().items()
        renamedData = {}
        type = m.get_type()
        for i in data:
            renamedData[type + '.' + i[0]] = i[1]
        lastMessageData.update(renamedData)
        lastTimestamp = str(m._timestamp)

    # Output easily-parsable text by default
    else:
        print(m._timestamp)
        print("%s.%02u: %s" % (
            time.strftime("%Y-%m-%d %H:%M:%S",
                          time.localtime(m._timestamp)),
                          int(m._timestamp*100.0)%100, m))

# Print out the last message here as the output lags the scanning by 1 timestep.
if opts.format == 'csv':
    if opts.zoh:
        out = [str(lastMessageData[y]) if y in lastMessageData else str(lastData[y]) for y in fields]
        lastData.update(lastMessageData)
    else:
        out = [str(lastMessageData[y]) if y in lastMessageData else "" for y in fields]
    out[0] = lastTimestamp
    print(opts.csv_sep.join(out))