#!/usr/bin/env python

'''
parse a MAVLink protocol XML file and generate a python implementation

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later

'''
import sys, textwrap, os
try:
    import mavparse
except ImportError:
    from pymavlink.generator import mavparse

# XSD schema file
schemaFile = os.path.join(os.path.dirname(os.path.realpath(__file__)), "mavschema.xsd")

# Set defaults for generating MAVLink code
DEFAULT_WIRE_PROTOCOL = mavparse.PROTOCOL_1_0
DEFAULT_LANGUAGE = 'Python'
DEFAULT_ERROR_LIMIT = 200
DEFAULT_VALIDATE = True

# List the supported languages. This is done globally because it's used by the GUI wrapper too
supportedLanguages = ["C", "CS", "JavaScript", "Python", "WLua"]

def mavgen(opts, args) :
    """Generate mavlink message formatters and parsers (C and Python ) using options
    and args where args are a list of xml files. This function allows python
    scripts under Windows to control mavgen using the same interface as
    shell scripts under Unix"""

    xml = []

    # Enable validation by default, disabling it if explicitly requested
    if opts.validate:
        try:
            from lxml import etree
        except ImportError:
            print("WARNING: Unable to load XML validator libraries. XML validation will not be performed")
            opts.validate = False

    if opts.validate:
        with open(schemaFile, 'r') as f:
            schemaRoot = etree.XML(f.read())
        schema = etree.XMLSchema(schemaRoot)
        log = schema.error_log
        xmlparser = etree.XMLParser(schema=schema)

    def mavgen_validate(xmlparser, xmlfilename):
        with open(xmlfilename, 'r') as f:
            etree.fromstring(f.read(), xmlparser)

    # Process all XML files, validating them as necessary.
    for fname in args:
        if opts.validate:
            print("Validating %s" % fname)
            mavgen_validate(xmlparser, fname);
        else:
            print("Validation skipped for %s." % fname)

        print("Parsing %s" % fname)
        xml.append(mavparse.MAVXML(fname, opts.wire_protocol))

    # expand includes
    for x in xml[:]:
        for i in x.include:
            fname = os.path.join(os.path.dirname(x.filename), i)

            ## Validate XML file with XSD file if possible.
            if opts.validate:
                print("Validating %s" % fname)
                mavgen_validate(xmlparser, fname);
            else:
                print("Validation skipped for %s." % fname)

            ## Parsing
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

    # Convert language option to lowercase and validate
    opts.language = opts.language.lower()
    if opts.language == 'python':
        import mavgen_python
        mavgen_python.generate(opts.output, xml)
    elif opts.language == 'c':
        try:
            import mavgen_c
        except Exception:
            from pymavlink.generator import mavgen_c
        mavgen_c.generate(opts.output, xml)
    elif opts.language == 'wlua':
        try:
            import mavgen_wlua
        except Exception:
            from pymavlink.generator import mavgen_wlua
        mavgen_wlua.generate(opts.output, xml)
    elif opts.language == 'cs':
        try:
            import mavgen_cs
        except Exception:
            from pymavlink.generator import mavgen_cs
        mavgen_cs.generate(opts.output, xml)
    elif opts.language == 'javascript':
        try:
            import mavgen_javascript
        except Exception:
            from pymavlink.generator import mavgen_javascript
        mavgen_javascript.generate(opts.output, xml)
    elif opts.language == 'objc':
        try:
            import mavgen_objc
        except Exception:
            from pymavlink.generator import mavgen_objc
        mavgen_objc.generate(opts.output, xml)
    else:
        print("Unsupported language %s" % opts.language)


# build all the dialects in the dialects subpackage
class Opts:
    def __init__(self, output, wire_protocol=DEFAULT_WIRE_PROTOCOL, language=DEFAULT_LANGUAGE, validate=DEFAULT_VALIDATE, error_limit=DEFAULT_ERROR_LIMIT):
        self.wire_protocol = wire_protocol
        self.error_limit = error_limit
        self.language = language
        self.output = output
        self.validate = validate

def mavgen_python_dialect(dialect, wire_protocol):
    '''generate the python code on the fly for a MAVLink dialect'''
    dialects = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'dialects')
    mdef = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..', 'message_definitions')
    if wire_protocol == mavparse.PROTOCOL_0_9:
        py = os.path.join(dialects, 'v09', dialect + '.py')
        xml = os.path.join(dialects, 'v09', dialect + '.xml')
        if not os.path.exists(xml):
            xml = os.path.join(mdef, 'v0.9', dialect + '.xml')
    else:
        py = os.path.join(dialects, 'v10', dialect + '.py')
        xml = os.path.join(dialects, 'v10', dialect + '.xml')
        if not os.path.exists(xml):
            xml = os.path.join(mdef, 'v1.0', dialect + '.xml')
    opts = Opts(py, wire_protocol)

    # Python 2 to 3 compatibility
    try:
        import StringIO as io
    except ImportError:
        import io

    # throw away stdout while generating
    stdout_saved = sys.stdout
    sys.stdout = io.StringIO()
    try:
        xml = os.path.relpath(xml)
        mavgen( opts, [xml] )
    except Exception:
        sys.stdout = stdout_saved
        raise
    sys.stdout = stdout_saved


if __name__ == "__main__":
    from argparse import ArgumentParser

    parser = ArgumentParser(description="This tool generate implementations from MAVLink message definitions")
    parser.add_argument("-o", "--output", default="mavlink", help="output directory.")
    parser.add_argument("--lang", dest="language", choices=supportedLanguages, default=DEFAULT_LANGUAGE, help="language of generated code [default: %(default)s]")
    parser.add_argument("--wire-protocol", choices=[mavparse.PROTOCOL_0_9, mavparse.PROTOCOL_1_0], default=DEFAULT_WIRE_PROTOCOL, help="MAVLink protocol version. [default: %(default)s]")
    parser.add_argument("--no-validate", action="store_false", dest="validate", default=DEFAULT_VALIDATE, help="Do not perform XML validation. Can speed up code generation if XML files are known to be correct.")
    parser.add_argument("--error-limit", default=DEFAULT_ERROR_LIMIT, help="maximum number of validation errors to display")
    parser.add_argument("definitions", metavar="XML", nargs="+", help="MAVLink definitions")
    args = parser.parse_args()

    mavgen(args, args.definitions)
