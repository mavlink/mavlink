#!/usr/bin/env python

'''
parse a MAVLink protocol XML file and generate a python implementation

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later

'''

from __future__ import print_function
from future import standard_library
standard_library.install_aliases()
from builtins import object
import os
import re
import sys
from . import mavparse

# XSD schema file
schemaFile = os.path.join(os.path.dirname(os.path.realpath(__file__)), "mavschema.xsd")

# Set defaults for generating MAVLink code
DEFAULT_WIRE_PROTOCOL = mavparse.PROTOCOL_1_0
DEFAULT_LANGUAGE = 'Python'
DEFAULT_ERROR_LIMIT = 200
DEFAULT_VALIDATE = True
DEFAULT_STRICT_UNITS = False

MAXIMUM_INCLUDE_FILE_NESTING = 5

# List the supported languages. This is done globally because it's used by the GUI wrapper too
supportedLanguages = ["C", "CS", "JavaScript", "Python", "WLua", "ObjC", "Swift", "Java", "C++11"]


def mavgen(opts, args):
    """Generate mavlink message formatters and parsers (C and Python ) using options
    and args where args are a list of xml files. This function allows python
    scripts under Windows to control mavgen using the same interface as
    shell scripts under Unix"""

    xml = []
    all_files = set()

    # Enable validation by default, disabling it if explicitly requested
    if opts.validate:
        try:
            from lxml import etree
            with open(schemaFile, 'r') as f:
                xmlschema_root = etree.parse(f)
                if not opts.strict_units:
                    # replace the strict "SI_Unit" list of known unit strings with a more generic "xs:string" type
                    for elem in xmlschema_root.iterfind('xs:attribute[@name="units"]', xmlschema_root.getroot().nsmap):
                        elem.set("type", "xs:string")
                xmlschema = etree.XMLSchema(xmlschema_root)
        except ImportError:
            print("WARNING: Failed to import lxml module etree. Are lxml, libxml2 and libxslt installed? XML validation will not be performed", file=sys.stderr)
            opts.validate = False
        except etree.XMLSyntaxError as err:
            print("WARNING: XML Syntax Errors detected in %s XML schema file. XML validation will not be performed" % schemaFile, file=sys.stderr)
            print(str(err.error_log), file=sys.stderr)
            opts.validate = False
        except:
            print("WARNING: Unable to load XML validator libraries. XML validation will not be performed", file=sys.stderr)
            opts.validate = False

    def expand_includes():
        """Expand includes in current list of all files, ignoring those already parsed."""
        for x in xml[:]:
            for i in x.include:
                fname = os.path.join(os.path.dirname(x.filename), i)

                # Only parse new include files
                if fname in all_files:
                    continue
                all_files.add(fname)

                # Validate XML file with XSD file if possible.
                if opts.validate:
                    print("Validating %s" % fname)
                    if not mavgen_validate(fname):
                        return False
                else:
                    print("Validation skipped for %s." % fname)

                # Parsing
                print("Parsing %s" % fname)
                xml.append(mavparse.MAVXML(fname, opts.wire_protocol))

                # include message lengths and CRCs too
                x.message_crcs.update(xml[-1].message_crcs)
                x.message_lengths.update(xml[-1].message_lengths)
                x.message_min_lengths.update(xml[-1].message_min_lengths)
                x.message_flags.update(xml[-1].message_flags)
                x.message_target_system_ofs.update(xml[-1].message_target_system_ofs)
                x.message_target_component_ofs.update(xml[-1].message_target_component_ofs)
                x.message_names.update(xml[-1].message_names)
                x.largest_payload = max(x.largest_payload, xml[-1].largest_payload)

    def mavgen_validate(xmlfile):
        """Uses lxml to validate an XML file. We define mavgen_validate
           here because it relies on the XML libs that were loaded in mavgen(), so it can't be called standalone"""
        xmlvalid = True
        try:
            with open(xmlfile, 'r') as f:
                xmldocument = etree.parse(f)
                xmlschema.assertValid(xmldocument)
                forbidden_names_re = re.compile("^(break$|case$|class$|catch$|const$|continue$|debugger$|default$|delete$|do$|else$|\
                                    export$|extends$|finally$|for$|function$|if$|import$|in$|instanceof$|let$|new$|\
                                    return$|super$|switch$|this$|throw$|try$|typeof$|var$|void$|while$|with$|yield$|\
                                    enum$|await$|implements$|package$|protected$|static$|interface$|private$|public$|\
                                    abstract$|boolean$|byte$|char$|double$|final$|float$|goto$|int$|long$|native$|\
                                    short$|synchronized$|transient$|volatile$).*", re.IGNORECASE)
                for element in xmldocument.iter('enum', 'entry', 'message', 'field'):
                    if forbidden_names_re.search(element.get('name')):
                        print("Validation error:", file=sys.stderr)
                        print("Element : %s at line : %s contains forbidden word" % (element.tag, element.sourceline), file=sys.stderr)
                        xmlvalid = False

            return xmlvalid
        except etree.XMLSchemaError:
            return False
        except etree.DocumentInvalid as err:
            sys.exit('ERROR: %s' % str(err.error_log))
        return True

    # Process all XML files, validating them as necessary.
    for fname in args:
        # only add each dialect file argument once.
        if fname in all_files:
            continue
        all_files.add(fname)

        if opts.validate:
            print("Validating %s" % fname)
            if not mavgen_validate(fname):
                return False
        else:
            print("Validation skipped for %s." % fname)

        print("Parsing %s" % fname)
        xml.append(mavparse.MAVXML(fname, opts.wire_protocol))

    # expand includes
    for i in range(MAXIMUM_INCLUDE_FILE_NESTING):
        len_allfiles = len(all_files)
        expand_includes()
        if len(all_files) == len_allfiles:
            # stop when loop doesn't add any more included files
            break

    # work out max payload size across all includes
    largest_payload = max(x.largest_payload for x in xml) if xml else 0
    for x in xml:
        x.largest_payload = largest_payload

    if mavparse.check_duplicates(xml):
        sys.exit(1)

    print("Found %u MAVLink message types in %u XML files" % (
        mavparse.total_msgs(xml), len(xml)))

    # convert language option to lowercase and validate
    opts.language = opts.language.lower()
    if opts.language == 'python':
        from . import mavgen_python
        mavgen_python.generate(opts.output, xml)
    elif opts.language == 'c':
        from . import mavgen_c
        mavgen_c.generate(opts.output, xml)
    elif opts.language == 'wlua':
        from . import mavgen_wlua
        mavgen_wlua.generate(opts.output, xml)
    elif opts.language == 'cs':
        from . import mavgen_cs
        mavgen_cs.generate(opts.output, xml)
    elif opts.language == 'javascript':
        from . import mavgen_javascript
        mavgen_javascript.generate(opts.output, xml)
    elif opts.language == 'objc':
        from . import mavgen_objc
        mavgen_objc.generate(opts.output, xml)
    elif opts.language == 'swift':
        from . import mavgen_swift
        mavgen_swift.generate(opts.output, xml)
    elif opts.language == 'java':
        from . import mavgen_java
        mavgen_java.generate(opts.output, xml)
    elif opts.language == 'c++11':
        from . import mavgen_cpp11
        mavgen_cpp11.generate(opts.output, xml)
    else:
        print("Unsupported language %s" % opts.language)

    return True


# build all the dialects in the dialects subpackage
class Opts(object):
    def __init__(self, output, wire_protocol=DEFAULT_WIRE_PROTOCOL, language=DEFAULT_LANGUAGE, validate=DEFAULT_VALIDATE, error_limit=DEFAULT_ERROR_LIMIT, strict_units=DEFAULT_STRICT_UNITS):
        self.wire_protocol = wire_protocol
        self.error_limit = error_limit
        self.language = language
        self.output = output
        self.validate = validate
        self.strict_units = strict_units


def mavgen_python_dialect(dialect, wire_protocol):
    '''generate the python code on the fly for a MAVLink dialect'''
    dialects = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'dialects')
    mdef = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..', 'message_definitions')
    if wire_protocol == mavparse.PROTOCOL_0_9:
        py = os.path.join(dialects, 'v09', dialect + '.py')
        xml = os.path.join(dialects, 'v09', dialect + '.xml')
        if not os.path.exists(xml):
            xml = os.path.join(mdef, 'v0.9', dialect + '.xml')
    elif wire_protocol == mavparse.PROTOCOL_1_0:
        py = os.path.join(dialects, 'v10', dialect + '.py')
        xml = os.path.join(dialects, 'v10', dialect + '.xml')
        if not os.path.exists(xml):
            xml = os.path.join(mdef, 'v1.0', dialect + '.xml')
    else:
        py = os.path.join(dialects, 'v20', dialect + '.py')
        xml = os.path.join(dialects, 'v20', dialect + '.xml')
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
        if not mavgen(opts, [xml]):
            sys.stdout = stdout_saved
            return False
    except Exception:
        sys.stdout = stdout_saved
        raise
    sys.stdout = stdout_saved
    return True

if __name__ == "__main__":
    raise DeprecationWarning("Executable was moved to pymavlink.tools.mavgen")
