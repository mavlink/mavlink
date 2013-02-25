#!/usr/bin/env python

'''
Use mavgen.py on all available MAVLink XML definitions to generate
C and Python MAVLink routines for sending and parsing the protocol

Copyright Pete Hollands 2011
Released under GNU GPL version 3 or later
'''

import os, sys, glob, re
from mavgen import mavgen

# allow import from the parent directory, where mavutil.py is
sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))

class options:
    """ a class to simulate the options of mavgen OptionsParser"""
    def __init__(self, lang, output, wire_protocol, error_limit):
        self.language = lang
        self.wire_protocol = wire_protocol
        self.output = output
        self.error_limit = error_limit

protocols = [ '0.9', '1.0' ]

for protocol in protocols :
    xml_directory = './message_definitions/v'+protocol
    print "xml_directory is", xml_directory
    xml_file_names = glob.glob(xml_directory+'/*.xml')

    for xml_file in xml_file_names:
        print "xml file is ", xml_file
        xml_file_base = os.path.basename(xml_file)
        xml_file_base = re.sub("\.xml","", xml_file_base)
        print "xml_file_base is", xml_file_base
        args = []
        args.append(xml_file)

        # Generate C libraries
        opts = options(lang = "C", output = "C/include_v"+protocol, \
                       wire_protocol=protocol, error_limit=200)
        mavgen(opts, args)

        # Generate Python libraries
        opts = options(lang = "python", \
                       output="python/mavlink_"+xml_file_base+"_v"+protocol+".py", \
                       wire_protocol=protocol, error_limit=200)
        mavgen(opts,args)

        # Generate C-sharp libraries
        opts = options(lang = "CS", \
                       output="CS/v" + protocol + "/mavlink_" + xml_file_base + "/messages", \
                       wire_protocol=protocol, error_limit=200)
        mavgen(opts,args)

        # Generate WLua libraries
        opts = options(lang = "wlua", \
                       output="wlua/mavlink_"+xml_file_base+"_v"+protocol+".lua", \
                       wire_protocol=protocol, error_limit=200)
        mavgen(opts,args)
