#! /usr/bin/python
"""
Script to parse all XML definition files in ../message_definitions/v1.0/ for consistency errors.
These might include flag enums that do not include bitmask attributes, and so on.
"""

from bs4 import BeautifulSoup as bs
import os
import itertools

from argparse import ArgumentParser, RawDescriptionHelpFormatter

# Integer ranges of given types
types = {
    "int8_t": [-128, 127],
    "char": [-128, 127],  # assume signed char
    "uint8_t": [0, 255],
    "uint8_t_mavlink_version": [0, 255],
    "int16_t": [-32768, 32767],
    "uint16_t": [0, 65535],
    "int32_t": [-2147483648, 2147483647],
    "uint32_t": [0, 4294967295],
    "int64_t": [-9223372036854775808, 9223372036854775807],
    "uint64_t": [0, 18446744073709551615],
}

warning_count = 0
allowed_count = 0
allowlist = set()
matched_allowlist = set()


def warn(message):
    """Print a warning. If the message is in the allowlist it is still shown
    (prefixed with [allowed]) but does not count toward the failure total
    used by --exception. This lets CI gate on new warnings while tolerating
    pre-existing ones."""
    global warning_count, allowed_count
    if message in allowlist:
        print("[allowed] %s" % message)
        allowed_count += 1
        matched_allowlist.add(message)
    else:
        print(message)
        warning_count += 1


def check_enum(enum, file_name):
    name = None
    bitmask = None

    for attr, value in enum.attrs.items():
        if attr == 'name':
            name = value
        elif attr == 'bitmask':
            bitmask = True

    if name is None:
        raise Exception("%s: No name for Enum: %s" % (file_name, enum))

    values = []
    enumEntries = enum.find_all('entry')
    for entry in enumEntries:
        values.append(int(entry.get('value'), 0))

    # Check for duplicate values
    for a, b in itertools.combinations(values, 2):
        if a == b:
            raise Exception("%s: Enum: %s duplicate value %i" %
                            (file_name, name, a))

    # Check if should be marked as a bitmask
    contains_zero = 0 in values
    if bitmask and contains_zero:
        warn("%s: Enum: %s bitmask should not contain 0" % (file_name, name))

    # Need at least three values to tell if something should be a bitmask
    # 1,2,4 vs 1,2,3 (assuming bits added sequentially)
    bitmask_values = values.copy()
    if contains_zero:
        bitmask_values.remove(0)

    if len(bitmask_values) > 2:
        if bitmask is None:
            bitmask = False

        overlap = False
        for a, b in itertools.combinations(bitmask_values, 2):
            if (a & b) != 0:
                overlap = True
                break

        if not bitmask and not overlap:
            warn("%s: Enum: %s should be a marked as bitmask?" %
                 (file_name, name))

        if bitmask and overlap:
            warn("%s: Enum: %s should be not be a marked as bitmask" %
                 (file_name, name))

    return {"name": name, "bitmask": bitmask, "values": values}


def check_field(file_name, msg_name, field, enums):
    name = field.get('name')
    enum = field.get('enum')
    units = field.get('units')

    # Enum with units doesn't make sense
    if enum is not None and units is not None:
        warn("%s: Message %s field %s has both units and enum" %
             (file_name, msg_name, name))

    if enum is not None:
        # Enum should exist
        if enum not in enums:
            warn("%s: Message %s field %s enum %s does not exist" %
                 (file_name, msg_name, name, enum))
            return

        enums[enum]["used"] = True

        # An enum with no entries has min/max set to None during aggregation
        # and was already warned about there, so there's nothing to range-check.
        if enums[enum]["min"] is None:
            return

        # Enum should fit in given type
        type = field.get('type').split('[')[0]
        if type not in types:
            warn("%s: Message %s field %s enum %s unexpected type: %s" %
                 (file_name, msg_name, name, enum, type))

        elif (enums[enum]["min"] < types[type][0]) or (enums[enum]["max"] > types[type][1]):
            warn("%s: Message %s field %s enum %s does not fit in type: %s" %
                 (file_name, msg_name, name, enum, type))


def check_cmd_param(file_name, cmd_name, entry, enums):
    index = entry.get('index')
    enum = entry.get('enum')
    units = entry.get('units')

    # Enum with units doesn't make sense
    if enum is not None and units is not None:
        warn("%s: Command %s param %s has both units and enum" %
             (file_name, cmd_name, index))

    if enum is not None:
        # Enum should exist
        if enum not in enums:
            warn("%s: Command %s param %s enum %s does not exist" %
                 (file_name, cmd_name, index, enum))
            return

        enums[enum]["used"] = True

    # There are a huge amount or errors here, commented out for now
    # Should be marked as reserved correctly
    # if len(entry.contents) > 0:
    #    description = entry.contents[0]
    #    reversed_descriptions = ["Empty.", "Empty", "Reserved", "Reserved (all remaining params)", "Reserved (set to 0)"]
    #    if description in reversed_descriptions:
    #        print("%s: Command %s param %s should be marked reserved=\"true\"" % (file_name, cmd_name, index))


description = """
XML consistency parser.

Checks XML definition files in ../message_definitions/v1.0/.
Warns on consistency errors such as flag enums that do not include bitmask attributes, and so on.

Known, pre-existing warnings can be listed in an allowlist file (see --allowlist)
so that CI can gate on new warnings while tolerating ones we can't fix yet.
"""

parser = ArgumentParser(description=description,
                        formatter_class=RawDescriptionHelpFormatter)

parser.add_argument('-f', '--file', default=None, nargs='+',
                    help="XML file name(s) to check (defaults to the managed dialects)")
parser.add_argument('-e', '--exception', action='store_true',
                    help="Throw error if any non-allowlisted warnings are found")
parser.add_argument('-a', '--allowlist', default=None,
                    help="Path to allowlist file of known-acceptable warnings "
                         "(defaults to xml_consistency_allowlist.txt next to this script)")

args = parser.parse_args()

# Resolve paths relative to this script, not the working directory, so the
# defaults work regardless of where the script is invoked from.
script_dir = os.path.dirname(os.path.abspath(__file__))
source_dir = os.path.join(script_dir, "../message_definitions/v1.0/")

# Dialects this repository controls. Other dialects (e.g. ardupilotmega.xml)
# are vendored/synced from downstream projects, so we don't gate CI on their
# content. They are free to run this script on their side if they wish (pass
# their files via -f/--file).
managed_dialects = ["common.xml", "minimal.xml", "standard.xml", "development.xml"]

if args.file is None:
    files = managed_dialects
else:
    files = [f if f.endswith('.xml') else f + '.xml' for f in args.file]

# Load the allowlist of known-acceptable warnings. An explicitly-passed path
# that doesn't exist is a hard error; a missing default is reported (not
# silently ignored) so that unexpectedly-failing CI is easy to diagnose.
default_allowlist = os.path.join(script_dir, "xml_consistency_allowlist.txt")
allowlist_path = args.allowlist if args.allowlist is not None else default_allowlist

if os.path.exists(allowlist_path):
    with open(allowlist_path, 'r') as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith('#'):
                allowlist.add(line)
elif args.allowlist is not None:
    raise SystemExit("Allowlist file not found: %s" % allowlist_path)
else:
    print("Note: no allowlist found at %s; all warnings will count toward the total." %
          allowlist_path)

print(f"Files: {files}")

xml = {}
for file in files:
    # Read the XML file
    path = source_dir + file
    with open(path, 'r') as f:
        xml_content = f.read()

    # Initialize BeautifulSoup with the XML content
    xml[file] = bs(xml_content, 'xml')

# Get all enums
all_enums = {}
for key in xml:
    for enum in xml[key].find_all('enum'):
        decoded = check_enum(enum, key)
        name = decoded["name"]
        if name in all_enums:
            # Add to existing enum
            all_enums[name]['file'].append(key)
            all_enums[name]['enum'].append(decoded)

        else:
            # Create new enum
            all_enums[name] = {'name': name, 'file': [
                key], 'enum': [decoded], 'used': False}

# Check for enums declared in multiple locations
for name in all_enums:
    # Combine results
    enum = all_enums[name]

    # Combine file names
    enum['file'] = ", ".join(enum['file'])

    values = enum['enum'][0]['values']
    enum['bitmask'] = enum['enum'][0]['bitmask']
    bitmask_conflict = False
    values_conflict = False

    for i in range(1, len(enum['enum'])):
        bitmask_conflict |= bool(enum['bitmask']) != bool(
            enum['enum'][i]['bitmask'])
        values_conflict |= not set(values).isdisjoint(enum['enum'][i]['values'])
        values += enum['enum'][i]['values']

    if not values:
        warn("%s: Enum: %s has no entries" % (enum['file'], name))
        # Leave min/max defined (as None) so downstream field checks can
        # detect the empty enum rather than hitting a KeyError.
        enum['min'] = enum['max'] = None
        continue

    enum['min'] = min(values)
    enum['max'] = max(values)

    if bitmask_conflict:
        warn("%s: Enum: %s has conflicting bitmask definitions" %
             (enum['file'],  name))

    if values_conflict:
        warn("%s: Enum: %s has conflicting values" % (enum['file'],  name))

# Check all fields against enums
for key in xml:
    for message in xml[key].find_all('message'):
        name = message.get('name')
        fields = message.find_all('field')
        for field in fields:
            check_field(key, name, field, all_enums)

# Check params in MAV_CMD
for key in xml:
    for enum in xml[key].find_all('enum', {"name": "MAV_CMD"}):
        for entry in enum.find_all('entry'):
            name = entry.get('name')
            seen_indices = set()   # Indices seen so far, to check for duplicates
            for param in entry.find_all('param'):
                check_cmd_param(key, name, param, all_enums)
                idx = param.get('index')

                # Flag duplicate indices regardless of whether they're in the
                # valid range, so e.g. two params both with index="0" are both
                # reported as a duplicate and as out of range.
                if idx is not None:
                    if idx in seen_indices:
                        warn("%s: Command %s param index %s is duplicated" %
                             (key, name, idx))
                    else:
                        seen_indices.add(idx)

                # Check if the param index is an integer and in the valid range of [1,7]
                try:
                    idx_int = int(idx)
                    if idx_int < 1 or idx_int > 7:
                        warn("%s: Command %s param index %s is out of range" %
                             (key, name, idx))
                except (TypeError, ValueError):
                    warn("%s: Command %s param index %s is not an integer" %
                         (key, name, idx))

# Check for unused enums. Defining enums not referenced by any message or
# command is a legitimate use of this library, so intended orphans should be
# added to the allowlist rather than removed.
for key in all_enums:
    if all_enums[key]["used"] is False:
        warn("%s: Enum: %s is unused" %
             (all_enums[key]['file'], all_enums[key]["name"]))

# Detect stale allowlist entries that no longer match any warning (e.g. a
# warning was fixed, or its message was reworded). Only meaningful on a full
# run: a subset run (-f) legitimately won't exercise entries for other files.
# Tracked separately from warning_count so CI can distinguish a new XML
# consistency problem from allowlist housekeeping: both fail --exception, but
# the messages tell you which one to fix (edit the XML vs. prune the allowlist).
stale = []
if args.file is None:
    stale = sorted(allowlist - matched_allowlist)
    for entry in stale:
        print("Stale allowlist entry (no longer matches any warning): %s" % entry)

# Give summary for possible CI usage
if allowed_count:
    print("\n%i allowlisted warning(s) suppressed." % allowed_count)

if stale:
    print("\n%i stale allowlist entry(ies) no longer match any warning and "
          "should be removed from %s." % (len(stale), allowlist_path))

summary = ("Found %i new consistency warning(s) and %i stale allowlist "
           "entry(ies) in: %s" % (warning_count, len(stale), files))

if args.exception and (warning_count > 0 or stale):
    raise Exception(summary + "\n")

print("\n%s\n" % summary)
