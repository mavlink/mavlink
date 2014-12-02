#!/usr/bin/env python

'''
parse a MAVLink protocol XML file and generate a python implementation

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later

'''
from pymavlink.generator import mavgen
from pymavlink.generator import mavparse

# Set defaults for generating MAVLink code
DEFAULT_WIRE_PROTOCOL = mavparse.PROTOCOL_1_0
DEFAULT_LANGUAGE = 'Python'
DEFAULT_ERROR_LIMIT = 200
DEFAULT_VALIDATE = True

# List the supported languages. This is done globally because it's used by the GUI wrapper too
supportedLanguages = ["C", "CS", "JavaScript", "Python", "WLua", "ObjC", "Java"]

from argparse import ArgumentParser

parser = ArgumentParser(description="This tool generate implementations from MAVLink message definitions")
parser.add_argument("-o", "--output", default="mavlink", help="output directory.")
parser.add_argument("--lang", dest="language", choices=supportedLanguages, default=DEFAULT_LANGUAGE, help="language of generated code [default: %(default)s]")
parser.add_argument("--wire-protocol", choices=[mavparse.PROTOCOL_0_9, mavparse.PROTOCOL_1_0], default=DEFAULT_WIRE_PROTOCOL, help="MAVLink protocol version. [default: %(default)s]")
parser.add_argument("--no-validate", action="store_false", dest="validate", default=DEFAULT_VALIDATE, help="Do not perform XML validation. Can speed up code generation if XML files are known to be correct.")
parser.add_argument("--error-limit", default=DEFAULT_ERROR_LIMIT, help="maximum number of validation errors to display")
parser.add_argument("definitions", metavar="XML", nargs="+", help="MAVLink definitions")
args = parser.parse_args()

mavgen.mavgen(args, args.definitions)
