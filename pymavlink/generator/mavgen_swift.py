#!/usr/bin/env python
"""
Parse a MAVLink protocol XML file and generate Swift implementation

Copyright Max Odnovolyk 2015
Released under GNU GPL version 3 or later
"""
from __future__ import print_function

import os
from . import mavtemplate

abbreviations = ["MAV", "PX4", "UDB", "PPZ", "PIXHAWK", "SLUGS", "FP", "ASLUAV", "VTOL", "ROI", "UART", "UDP", "IMU", "IMU2", "3D", "RC", "GPS", "GPS1", "GPS2", "NED", "RTK"]
swift_types = {'char' : ("String", '"\\0"', "mavString(offset: %u, length: %u)"),
               'uint8_t' : ("UInt8", 0, "mavNumber(offset: %u)"),
               'int8_t' : ("Int8", 0, "mavNumber(offset: %u)"),
               'uint16_t' : ("UInt16", 0, "mavNumber(offset: %u)"),
               'int16_t' : ("Int16", 0, "mavNumber(offset: %u)"),
               'uint32_t' : ("UInt32", 0, "mavNumber(offset: %u)"),
               'int32_t' : ("Int32", 0, "mavNumber(offset: %u)"),
               'uint64_t' : ("UInt64", 0, "mavNumber(offset: %u)"),
               'int64_t' : ("Int64", 0, "mavNumber(offset: %u)"),
               'float' : ("Float", 0, "mavNumber(offset: %u)"),
               'double' : ("Double", 0, "mavNumber(offset: %u)"),
               'uint8_t_mavlink_version' : ("UInt8", 0, "mavNumber(offset: %u)")}

t = mavtemplate.MAVTemplate()

def generate_header(outf, filelist, xml):
    """Generate Swift file header with source files list and creation date"""

    print("Generating Swift file header")

    t.write(outf, """
//
//  MAVLink.swift
//  MAVLink Micro Air Vehicle Communication Protocol
//
//  Generated from ${FILELIST} on ${PARSE_TIME} by mavgen_swift.py 
//  http://qgroundcontrol.org/mavlink/start
//

import Foundation


/**
    Common protocol for all MAVLink entities which describes type metadata properties
*/
public protocol MAVLinkEntity: CustomStringConvertible, CustomDebugStringConvertible {
    
    /// Original MAVLink enum name (from declarations xml)
    static var typeName: String { get }

    /// Compact type description
    static var typeDescription: String { get }

    /// Verbose type description
    static var typeDebugDescription: String { get }
}


// MARK: MAVLink enums


/**
    Enum protocol description with common for all MAVLink enums property requirements
*/
public protocol Enum: RawRepresentable, Equatable, MAVLinkEntity {

    /// Array with all members of current enum
    static var allMembers: [Self] { get }

    // Array with `Name` - `Description` tuples (values from declarations xml file)
    static var membersInfo: [(String, String)] { get }

    /// Original MAVLinks enum member name (as declared in definitions xml file)
    var memberName: String { get }

    /// Specific member description from definitions xml
    var memberDescription: String { get }
}


/**
    Enum protocol default implementations
*/
extension Enum {
    public static var typeDebugDescription: String {
        // It seems Xcode 7 beta does not support default protocol implementations for type methods.
        // Calling this method without specific implementations inside enums will cause following error on Xcode 7.0 beta (7A120f):
        // "Command failed due to signal: Illegal instruction: 4"
        let cases = "\\n\\t".join(allMembers.map { $0.debugDescription })
        return "Enum \(typeName): \(typeDescription)\\nMembers:\\n\\t\(cases)"
    }
    
    public var description: String {
        return memberName
    }

    public var debugDescription: String {
        return "\(memberName): \(memberDescription)"
    }
    
    public var memberName: String {
        return Self.membersInfo[Self.allMembers.indexOf(self)!].0
    }

    public var memberDescription: String {
        return Self.membersInfo[Self.allMembers.indexOf(self)!].1
    }
}

""", {'FILELIST' : ", ".join(filelist),
      'PARSE_TIME' : xml[0].parse_time})

def generate_enums(outf, enums, msgs):
    """Iterate through all enums and create Swift equivalents"""

    print("Generating Enums")

    for enum in enums:
        t.write(outf, """
${formatted_description}public enum ${swift_name}: ${raw_value_type}, Enum {
${{entry:${formatted_description}\tcase ${swift_name} = ${value}\n}}
}

extension ${swift_name} {
    public static var typeName = "${name}"
    public static var typeDescription = "${entity_description}"
    public static var typeDebugDescription: String {
        let cases = "\\n\\t".join(allMembers.map { $0.debugDescription })
        return "Enum \(typeName): \(typeDescription)\\nMembers:\\n\\t\(cases)"
    }
    public static var allMembers = [${all_entities}]
    public static var membersInfo = [${entities_info}]
}

""", enum)

def get_enum_raw_type(enum, msgs):
    """Search appropirate raw type for enums in messages fields"""

    for msg in msgs:
        for field in msg.fields:
            if  field.enum == enum.name:
                return swift_types[field.type][0]
    return "Int"

def generate_messages(outf, msgs):
    """Generate Swift structs to represent all MAVLink messages"""

    print("Generating Messages")

    t.write(outf, """

// MARK: MAVLink messages


/**
    Message protocol describes common for all MAVLink messages properties and methods requirements
*/
public protocol Message: MAVLinkEntity {

    /**
        Initialize Message from received data

        - Warning: Throws `ParserError` or `ParserEnumError` if any parsing error occurred
    */
    init(data: NSData) throws

    /// Array of tuples with fields name, offset, type and description information
    static var fieldsInfo: [(String, Int, String, String)] { get }

    /// All fields names and values of current Message
    var allFields: [(String, Any)] { get }
}


/**
    Message protocol default implementations
*/
extension Message {
    public static var typeDebugDescription: String {
        // It seems Xcode 7 beta does not support default protocol implementations for type methods.
        // Calling this method without specific implementations inside messages will cause following error in Xcode 7.0 beta (7A120f):
        // "Command failed due to signal: Illegal instruction: 4"
        let fields = "\\n\\t".join(fieldsInfo.map { "\($0.0): \($0.2): \($0.3)" })
        return "Struct \(typeName): \(typeDescription)\\nFields:\\n\\t\(fields)"
    }

    public var description: String {
        let describeField: ((String, Any)) -> String = { (let name, var value) in
            value = value is String ? "\\"\(value)\\"" : value
            return "\(name): \(value)"
        }
        let fieldsDescription = ", ".join(allFields.map(describeField))
        return "\(self.dynamicType)(\(fieldsDescription))"
    }

    public var debugDescription: String {
        let describeFieldVerbose: ((String, Any)) -> String = { (let name, var value) in
            value = value is String ? "\\"\(value)\\"" : value
            let (_, _, _, description) = Self.fieldsInfo.filter { $0.0 == name }.first!
            return "\(name) = \(value) : \(description)"
        }
        let fieldsDescription = "\\n\\t".join(allFields.map(describeFieldVerbose))
        return "\(Self.typeName): \(Self.typeDescription)\\nFields:\\n\\t\(fieldsDescription)"
    }

    public var allFields: [(String, Any)] {
        var result: [(String, Any)] = []
        let mirror = reflect(self)
        for i in 0..<mirror.count {
            result.append((mirror[i].0, mirror[i].1.value))
        }
        return result
    }
}

""")

    for msg in msgs:
        t.write(outf, """
${formatted_description}public struct ${swift_name}: Message {
${{fields:${formatted_description}\tpublic let ${swift_name}: ${return_type}\n}}

    public init(data: NSData) throws {
${{ordered_fields:\t\tself.${swift_name} = ${initial_value}\n}}
    }
}

extension ${swift_name} {
    public static var typeName = "${name}"
    public static var typeDescription = "${message_description}"
    public static var typeDebugDescription: String {
        let fields = "\\n\\t".join(fieldsInfo.map { "\($0.0): \($0.2): \($0.3)" })
        return "Struct \(typeName): \(typeDescription)\\nFields:\\n\\t\(fields)"
    }
    public static var fieldsInfo = [${fields_info}]
}

""", msg)

def append_static_code(filename, outf):
    """Open and copy static code from specified file"""

    basepath = os.path.dirname(os.path.realpath(__file__))
    filepath = os.path.join(basepath, 'swift/%s' % filename)
    
    print("Appending content of %s" % filename)
    
    with open(filepath) as inf:
        for line in inf:
            outf.write(line) 

def generate_message_mappings_array(outf, msgs):
    """Create array for mapping message Ids to proper structs"""

    classes = []
    for msg in msgs:
        classes.append("%u: %s.self" % (msg.id, msg.swift_name))
    t.write(outf, """



/**
    Array for mapping message id to proper struct
*/
private let messageIdToClass: [UInt8: Message.Type] = [${ARRAY_CONTENT}]
""", {'ARRAY_CONTENT' : ", ".join(classes)})

def generate_message_lengths_array(outf, msgs):
    """Create array with message lengths to validate known message lengths"""

    # form message lengths array
    lengths = []
    for msg in msgs:
        lengths.append("%u: %u" % (msg.id, msg.wire_length))

    t.write(outf, """


/**
    Message lengths array for known messages length validation
*/
private let messageLengths: [UInt8: UInt8] = [${ARRAY_CONTENT}]
""", {'ARRAY_CONTENT' : ", ".join(lengths)})

def generate_message_crc_extra_array(outf, msgs):
    """Add array with CRC extra values to detect incompatible XML changes"""

    crcs = []
    for msg in msgs:
        crcs.append("%u: %u" % (msg.id, msg.crc_extra))

    t.write(outf, """


/**
    Message CRSs extra for detection incompatible XML changes
*/
private let messageCrcsExtra: [UInt8: UInt8] = [${ARRAY_CONTENT}]
""", {'ARRAY_CONTENT' : ", ".join(crcs)})

def camel_case_from_underscores(string):
    """Generate a CamelCase string from an underscore_string"""

    components = string.split('_')
    string = ''

    for component in components:
        if component in abbreviations:
            string += component
        else:
            string += component[0].upper() + component[1:].lower()

    return string

def lower_camel_case_from_underscores(string):
    """Generate a lower-cased camelCase string from an underscore_string"""

    components = string.split('_')
    string = components[0]
    for component in components[1:]:
        string += component[0].upper() + component[1:]

    return string

def generate_enums_info(enums, msgs):
    """Add camel case swift names for enums an entries, descriptions and sort enums alphabetically"""

    for enum in enums:
        enum.swift_name = camel_case_from_underscores(enum.name)
        enum.raw_value_type = get_enum_raw_type(enum, msgs)

        enum.formatted_description = ""
        if enum.description:
            enum.description = " ".join(enum.description.split())
            enum.formatted_description = "\n/**\n    %s\n*/\n" % enum.description

        all_entities = []
        entities_info = []

        for entry in enum.entry:
            name = entry.name.replace(enum.name + '_', '')
            """Ensure that enums entry name does not start from digit"""
            if name[0].isdigit():
                name = "MAV_" + name
            entry.swift_name = camel_case_from_underscores(name)
            
            entry.formatted_description = ""
            if entry.description:
                entry.description = " ".join(entry.description.split())
                entry.formatted_description = "\n\t/// " + entry.description + "\n"

            all_entities.append(entry.swift_name)
            entities_info.append('("%s", "%s")' % (entry.name, entry.description.replace('"','\\"')))

        enum.all_entities = ", ".join(all_entities)
        enum.entities_info = ", ".join(entities_info)
        enum.entity_description = enum.description.replace('"','\\"')

    enums.sort(key = lambda enum : enum.swift_name)

def generate_messages_info(msgs):
    """Add proper formated variable names, initializers and type names to use in templates"""

    for msg in msgs:
        msg.swift_name = camel_case_from_underscores(msg.name)

        msg.formatted_description = ""
        if msg.description:
            msg.description = " ".join(msg.description.split())
            msg.formatted_description = "\n/**\n    %s\n*/\n" % " ".join(msg.description.split())
        msg.message_description = msg.description.replace('"','\\"')

        for field in msg.ordered_fields:
            field.swift_name = lower_camel_case_from_underscores(field.name)
            field.return_type = swift_types[field.type][0]
            
            # configure fields initializers
            if field.enum:
                # handle enums
                field.return_type = camel_case_from_underscores(field.enum)
                field.initial_value = "try data.mavEnumeration(offset: %u)" % field.wire_offset
            elif field.array_length > 0:
                if field.return_type == "String":
                    # handle strings
                    field.initial_value = "data." + swift_types[field.type][2] % (field.wire_offset, field.array_length)
                else:
                    # other array types
                    field.return_type = "[%s]" % field.return_type
                    field.initial_value = "try data.mavArray(offset: %u, count: %u)" % (field.wire_offset, field.array_length)
            else:
                # simple type field
                field.initial_value = "try data." + swift_types[field.type][2] % field.wire_offset
            
            field.formatted_description = ""
            if field.description:
                field.description = " ".join(field.description.split())
                field.formatted_description = "\n\t/// " + field.description + "\n"
         
        fields_info = ['("%s", %u, "%s", "%s")' % (field.swift_name, field.wire_offset, field.return_type, field.description.replace('"','\\"')) for field in msg.fields]
        msg.fields_info = ", ".join(fields_info)

    msgs.sort(key = lambda msg : msg.id)

def generate(basename, xml_list):
    """Generate complete MAVLink Swift implemenation"""

    if os.path.isdir(basename):
        filename = os.path.join(basename, 'MAVLink.swift')
    else:
        filename = basename

    msgs = []
    enums = []
    filelist = []
    for xml in xml_list:
        msgs.extend(xml.message)
        enums.extend(xml.enum)
        filelist.append(os.path.basename(xml.filename))
    
    outf = open(filename, "w")
    generate_header(outf, filelist, xml_list)
    generate_enums_info(enums, msgs)
    generate_enums(outf, enums, msgs)
    generate_messages_info(msgs)
    generate_messages(outf, msgs)
    append_static_code('Parser.swift', outf)
    generate_message_mappings_array(outf, msgs)
    generate_message_lengths_array(outf, msgs)
    generate_message_crc_extra_array(outf, msgs)
    outf.close()