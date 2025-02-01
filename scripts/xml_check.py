#! /usr/bin/python

# Simple checks to try and spot inconsistencies.

from bs4 import BeautifulSoup as bs
import os
import itertools

# Integer ranges of given types
types = {
    "int8_t": [-128, 127],
    "char": [-128, 127], # assume signed char
    "uint8_t": [0, 255],
    "uint8_t_mavlink_version": [0, 255],
    "int16_t": [-32768, 32767],
    "uint16_t": [0, 65535],
    "int32_t": [-2147483648, 2147483647],
    "uint32_t": [0, 4294967295],
    "int64_t": [-9223372036854775808, 9223372036854775807],
    "uint64_t": [0, 18446744073709551615],
}

def check_enum(enum):
    name = None
    bitmask = None

    for attr, value in enum.attrs.items():
        if attr == 'name':
            name = value
        elif attr == 'bitmask':
            bitmask = True

    if name == None:
        raise Exception("No name for Enum:" + enum)

    values = []
    enumEntries = enum.find_all('entry')
    for entry in enumEntries:
        values.append(int(entry.get('value')))

    # Check for duplicate values
    for a, b in itertools.combinations(values, 2):
        if a == b:
            raise Exception("Enum: %s duplicate value %i" % (name, a))

    # Check if should be marked as a bitmask
    if len(values) > 3:
        if bitmask == None:
            bitmask = False

        overlap = False
        for a, b in itertools.combinations(values, 2):
            if (a & b) != 0:
                overlap = True
                break

        if not bitmask and not overlap:
            print("Enum: %s should be a marked as bitmask?" % name)

        if bitmask and overlap:
            print("Enum: %s should be not be a marked as bitmask" % name)

    #elif bitmask == None:
        # If there are less than 3 values its hard to tell if something is a bitmask or not
        #print("Enum: %s check for bitmask" % name)

    return { "name": name, "bitmask": bitmask, "min": min(values), "max": max(values), "used": False }

def check_field(msg_name, field, enums):
    name = field.get('name')
    enum = field.get('enum')
    units = field.get('units')

    # Enum with units doesn't make sense
    if (enum != None) and (units != None):
        print("Message %s field %s has both units and enum" % (msg_name, name))

    if (enum != None):
        # Enum should exist
        if not (enum in enums):
            print("Message %s field %s enum %s does not exist" % (msg_name, name, enum))

        enums[enum]["used"] = True
        bitmask = enums[enum]["bitmask"]
        if bitmask != None:
            # Bitmask should match underlying enum
            display_bitmask = field.get("display") == "bitmask"

            if bitmask and not display_bitmask:
                print("Message %s field %s enum %s should marked: display=\"bitmask\"" % (msg_name, name, enum))

            if display_bitmask and not bitmask:
                print("Message %s field %s enum %s should not marked: display=\"bitmask\"" % (msg_name, name, enum))

        # Enum should fit in given type
        type = field.get('type').split('[')[0]
        if not (type in types):
            print("Message %s field %s enum %s unexpected type: %s" % (msg_name, name, enum, type))
        elif (enums[enum]["min"] < types[type][0]) or (enums[enum]["max"] > types[type][1]):
            print("Message %s field %s enum %s does not fit in type: %s" % (msg_name, name, enum, type))


def check_cmd_param(cmd_name, entry, enums):
    index = entry.get('index')
    enum = entry.get('enum')
    units = entry.get('units')

    # Enum with units doesn't make sense
    if (enum != None) and (units != None):
        print("Command %s param %s has both units and enum" % (cmd_name, index))

    if (enum != None):
        # Enum should exist
        if not (enum in enums):
            print("Command %s param %s enum %s does not exist" % (cmd_name, index, enum))

        enums[enum]["used"] = True

    # Should be marked as reserved correctly
    if len(entry.contents) > 0:
        description = entry.contents[0]
        reversed_descriptions = ["Empty.", "Empty", "Reserved", "Reserved (all remaining params)", "Reserved (set to 0)"]
        if description in reversed_descriptions:
            print("Command %s param %s should be marked reserved=\"true\"" % (cmd_name, index))


source_dir = os.path.join(os.path.dirname(__file__), "../message_definitions/v1.0/")
files = os.listdir(source_dir)

xml = {}
for file in files:
    if not file.endswith('.xml'):
        continue

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
        decoded = check_enum(enum)
        all_enums[decoded["name"]] = decoded

# Check all fields against enums
for key in xml:
    for message in xml[key].find_all('message'):
        name = message.get('name')
        fields = message.find_all('field')
        for field in fields:
            check_field(name, field, all_enums)

# Check params in MAV_CMD
for key in xml:
    for enum in xml[key].find_all('enum', {"name": "MAV_CMD"}):
        for entry in enum.find_all('entry'):
            name = entry.get('name')
            for param in entry.find_all('param'):
                check_cmd_param(name, param, all_enums)

# Check for unused enums
for key in all_enums:
    if all_enums[key]["used"] == False:
        print("Enum: %s is unused" % all_enums[key]["name"])
