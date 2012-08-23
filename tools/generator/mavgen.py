#!/usr/bin/env python

'''
parse a MAVLink protocol XML file and generate a python implementation

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later

'''


'''
Modified by dagoodma on 2012-7-18_15:15
    This has to be here so that these imports occur when importing this
    file, when before they only occured when running this file directly

'''
import sys, textwrap, os

# allow import from the parent directory to find mavgen 
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))

import mavparse
import mavgen_python
import mavgen_c

def mavgen(opts, args) :
    """Generate mavlink message formatters and parsers (C and Python ) using options
    and args where args are a list of xml files. This function allows python
    scripts under Windows to control mavgen using the same interface as
    shell scripts under Unix"""

    xml = []

    for fname in args:
        print("Parsing %s" % fname)
        xml.append(mavparse.MAVXML(fname, opts.wire_protocol))

    # expand includes
    for x in xml[:]:
        for i in x.include:
            fname = os.path.join(os.path.dirname(x.filename), i)
            print("Parsing %s" % fname)
            xml.append(mavparse.MAVXML(fname, opts.wire_protocol))

            # include message lengths and CRCs too
            for idx in range(0, 256):
                if x.message_lengths[idx] == 0:
                    x.message_lengths[idx] = xml[-1].message_lengths[idx]
                    x.message_crcs[idx] = xml[-1].message_crcs[idx]
                    x.message_names[idx] = xml[-1].message_names[idx]

    # work out max payload size across all includes
    largest_payload = 0
    for x in xml:
        if x.largest_payload > largest_payload:
            largest_payload = x.largest_payload
    for x in xml:
        x.largest_payload = largest_payload

    if mavparse.check_duplicates(xml):
        sys.exit(1)

    print("Found %u MAVLink message types in %u XML files" % (
        mavparse.total_msgs(xml), len(xml)))

    ''' 
    Modified on August 6th, 2012 by David Goodman
        This allows compliance with the language options specified
        in --help. It will still work the same as before, plus allows
        compliance with the options specified in --help.
    '''
    opts.language = opts.language.lower()
    if opts.language == 'python':
        mavgen_python.generate(opts.output, xml)
    elif opts.language == 'c':
        mavgen_c.generate(opts.output, xml)
    else:
        print("Unsupported language %s" % opts.language)
    

if __name__=="__main__":
    from optparse import OptionParser

    parser = OptionParser("%prog [options] <XML files>")
    parser.add_option("-o", "--output", dest="output", default="mavlink", help="output directory.")
    parser.add_option("--lang", dest="language", default="python", help="language of generated code: 'Python' or 'C' [default: %default]")
    parser.add_option("--wire-protocol", dest="wire_protocol", default=mavparse.PROTOCOL_1_0, help="MAVLink protocol version: '0.9' or '1.0'. [default: %default]")
    (opts, args) = parser.parse_args()

    if len(args) < 1:
        parser.error("You must supply at least one MAVLink XML protocol definition")
    mavgen(opts, args)
