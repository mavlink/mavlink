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

global warning_count
warning_count = 0


def check_enum(enum, file_name):
    global warning_count
    name = None
    bitmask = None

    for attr, value in enum.attrs.items():
        if attr == 'name':
            name = value
        elif attr == 'bitmask':
            bitmask = True

    if name == None:
        raise Exception("%s: No name for Enum: %s" % (file_name, enum))

    values = []
    enumEntries = enum.find_all('entry')
    for entry in enumEntries:
        values.append(int(entry.get('value')))

    # Check for duplicate values
    for a, b in itertools.combinations(values, 2):
        if a == b:
            raise Exception("%s: Enum: %s duplicate value %i" %
                            (file_name, name, a))

    # Check if should be marked as a bitmask
    contains_zero = 0 in values
    if bitmask and contains_zero:
        print("%s: Enum: %s bitmask should not contain 0" % (file_name, name))
        warning_count += 1

    # Need at least three values to tell if something should be a bitmask
    # 1,2,4 vs 1,2,3 (assuming bits added sequentially)
    bitmask_values = values.copy()
    if contains_zero:
        bitmask_values.remove(0)

    if len(bitmask_values) > 2:
        if bitmask == None:
            bitmask = False

        overlap = False
        for a, b in itertools.combinations(bitmask_values, 2):
            if (a & b) != 0:
                overlap = True
                break

        if not bitmask and not overlap:
            print("%s: Enum: %s should be a marked as bitmask?" %
                  (file_name, name))
            warning_count += 1

        if bitmask and overlap:
            print("%s: Enum: %s should be not be a marked as bitmask" %
                  (file_name, name))
            warning_count += 1

    return {"name": name, "bitmask": bitmask, "values": values, "used": False}


def check_field(file_name, msg_name, field, enums):
    global warning_count
    name = field.get('name')
    enum = field.get('enum')
    units = field.get('units')

    # Enum with units doesn't make sense
    if (enum != None) and (units != None):
        print("%s: Message %s field %s has both units and enum" %
              (file_name, msg_name, name))
        warning_count += 1

    if (enum != None):
        # Enum should exist
        if not (enum in enums):
            print("%s: Message %s field %s enum %s does not exist" %
                  (file_name, msg_name, name, enum))
            warning_count += 1
            return

        enums[enum]["used"] = True
        bitmask = enums[enum]["bitmask"]
        if bitmask != None:
            # Bitmask should match underlying enum
            display_bitmask = field.get("display") == "bitmask"

            if bitmask and not display_bitmask:
                print("%s: Message %s field %s enum %s should marked: display=\"bitmask\"" % (
                    file_name, msg_name, name, enum))
                warning_count += 1

            if display_bitmask and not bitmask:
                print("%s: Message %s field %s enum %s should not marked: display=\"bitmask\"" % (
                    file_name, msg_name, name, enum))
                warning_count += 1

        # Enum should fit in given type
        type = field.get('type').split('[')[0]
        if not (type in types):
            print("%s: Message %s field %s enum %s unexpected type: %s" %
                  (file_name, msg_name, name, enum, type))
            warning_count += 1

        elif (enums[enum]["min"] < types[type][0]) or (enums[enum]["max"] > types[type][1]):
            print("%s: Message %s field %s enum %s does not fit in type: %s" %
                  (file_name, msg_name, name, enum, type))
            warning_count += 1


def check_cmd_param(file_name, cmd_name, entry, enums):
    global warning_count
    index = entry.get('index')
    enum = entry.get('enum')
    units = entry.get('units')

    # Enum with units doesn't make sense
    if (enum != None) and (units != None):
        print("%s: Command %s param %s has both units and enum" %
              (file_name, cmd_name, index))
        warning_count += 1

    if (enum != None):
        # Enum should exist
        if not (enum in enums):
            print("%s: Command %s param %s enum %s does not exist" %
                  (file_name, cmd_name, index, enum))
            warning_count += 1
            return

        enums[enum]["used"] = True

    # There are a huge amount or errors here, commented out for now
    # Should be marked as reserved correctly
    # if len(entry.contents) > 0:
    #    description = entry.contents[0]
    #    reversed_descriptions = ["Empty.", "Empty", "Reserved", "Reserved (all remaining params)", "Reserved (set to 0)"]
    #    if description in reversed_descriptions:
    #        print("%s: Command %s param %s should be marked reserved=\"true\"" % (file_name, cmd_name, index))


description = f"""
XML consistency parser.

Checks XML definition files in ../message_definitions/v1.0/.
Warns on consistency errors such as flag enums that do not include bitmask attributes, and so on.
"""

parser = ArgumentParser(description=description,
                        formatter_class=RawDescriptionHelpFormatter)

parser.add_argument('-f', '--file', default=None,
                    help="File name to check (all xml files checked by default)")
parser.add_argument('-e', '--exception', action='store_true',
                    help="Throw error if any warnings are found")

args = parser.parse_args()

source_dir = os.path.join(os.path.dirname(
    __file__), "../message_definitions/v1.0/")

if args.file is None:
    files = list(filter(lambda x: x.endswith('.xml'), os.listdir(source_dir)))
else:
    if not args.file.endswith('.xml'):
        args.file += '.xml'
    files = [args.file]

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
        values_conflict |= len(
            list(set(values) & set(enum['enum'][i]['values']))) > 0
        values += enum['enum'][i]['values']

    enum['min'] = min(values)
    enum['max'] = max(values)

    if bitmask_conflict:
        print("%s: Enum: %s has conflicting bitmask definitions" %
              (enum['file'],  name))
        warning_count += 1

    if values_conflict:
        print("%s: Enum: %s has conflicting values" % (enum['file'],  name))
        warning_count += 1

    if (not enum['bitmask'] and len(values) <= 1) or len(values) == 0:
        print("%s: Enum: %s has only %i items?" %
              (enum['file'],  name, len(values)))
        warning_count += 1

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
            for param in entry.find_all('param'):
                check_cmd_param(key, name, param, all_enums)

# Check for unused enums
for key in all_enums:
    if all_enums[key]["used"] == False:
        print("%s: Enum: %s is unused" %
              (all_enums[key]['file'], all_enums[key]["name"]))
        warning_count += 1

# Give summary for possible CI usage
if args.exception and (warning_count > 0):
    raise Exception("Found %i issues in: %s\n" % (warning_count, files))

else:
    print("\nFound %i issues in: %s\n" % (warning_count, files))
