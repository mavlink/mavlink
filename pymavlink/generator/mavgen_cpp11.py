#!/usr/bin/env python
'''
parse a MAVLink protocol XML file and generate a C++ implementation

Based on C implementation and require C-library for framing.

Copyright Andrew Tridgell 2011
Copyright Vladimir Ermakov 2016
Released under GNU GPL version 3 or later
'''

import sys, textwrap, os, time
from . import mavparse, mavtemplate

t = mavtemplate.MAVTemplate()


def generate_main_hpp(directory, xml):
    '''generate main header per XML file'''
    f = open(os.path.join(directory, xml.basename + ".hpp"), mode='w')
    t.write(f, '''
/** @file
 *	@brief MAVLink comm protocol generated from ${basename}.xml
 *	@see http://mavlink.org
 */

#pragma once

#include <array>
#include <cstdint>
#include <sstream>

#ifndef MAVLINK_STX
#define MAVLINK_STX ${protocol_marker}
#endif

#include "../message.hpp"

namespace mavlink {
namespace ${basename} {

/**
 * Array of msg_entry needed for @p mavlink_parse_char() (trought @p mavlink_get_msg_entry())
 */
constexpr std::array<mavlink_msg_entry_t, ${message_entry_len}> MESSAGE_ENTRIES {{ ${message_entry_array} }};

//! MAVLINK VERSION
constexpr auto MAVLINK_VERSION = ${version};


// ENUM DEFINITIONS

${{enum:
/** @brief ${description} */
enum class ${name} : int
{
${{entry:    ${name}=${value}, /* ${description} |${{param:${description}| }} */
}}
};
}}


} // namespace ${basename}
} // namespace mavlink

// MESSAGE DEFINITIONS
${{message:#include "./mavlink_msg_${name_lower}.hpp"
}}

// base include
${{include_list:#include "../${base}/${base}.h"
}}
''', xml)

    f.close()


def generate_message_hpp(directory, m):
    '''generate per-message header for a XML file'''
    f = open(os.path.join(directory, 'mavlink_msg_%s.hpp' % m.name_lower), mode='w')
    t.write(f, '''
// MESSAGE ${name} support class

#pragma once

namespace mavlink {
namespace ${dialect_name} {
namespace msg {

/**
 * @brief ${name} message
 *
 * ${description}
 */
struct ${name} : mavlink::Message {
    static constexpr uint32_t MSG_ID = ${id};
    static constexpr size_t LENGTH = ${wire_length};
    static constexpr size_t MIN_LENGTH = ${wire_min_length};
    static constexpr uint8_t CRC_EXTRA = ${crc_extra};
    static constexpr auto NAME = "${name}";


${{fields:    ${cxx_type} ${name}; /*< ${description} */
}}


    inline std::string to_yaml(void)
    {
        std::stringstream ss;

        ss << NAME << ":" << std::endl;
${{fields:        ${to_yaml_code}
}}

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map)
    {
        map.reset(MSG_ID);

${{const_fileds:        ${name} = ${const_value};
}}

${{ordered_fields:        map << ${name};${ser_whitespace}// offset: ${wire_offset}
}}
    }

    inline void deserialize(mavlink::MsgMap &map)
    {
${{ordered_fields:        map >> ${name};${ser_whitespace}// offset: ${wire_offset}
}}
    }
};

} // namespace msg
} // namespace ${dialect_name}
} // namespace mavlink
''', m)
    f.close()


# XXX: gtest-based testing suite

## XXX fixme!
def copy_fixed_headers(directory, xml):
    '''copy the fixed protocol headers to the target directory'''
    import shutil, filecmp
    hlist = {
        "2.0": [ 'protocol.h', 'mavlink_helpers.h', 'mavlink_types.h', 'checksum.h', 'mavlink_conversions.h',
                 'mavlink_get_info.h', 'mavlink_sha256.h' ]
        }
    basepath = os.path.dirname(os.path.realpath(__file__))
    srcpath = os.path.join(basepath, 'C/include_v%s' % xml.wire_protocol_version)
    print("Copying fixed headers for protocol %s to %s" % (xml.wire_protocol_version, directory))
    for h in hlist[xml.wire_protocol_version]:
        src = os.path.realpath(os.path.join(srcpath, h))
        dest = os.path.realpath(os.path.join(directory, h))
        if src == dest or (os.path.exists(dest) and filecmp.cmp(src, dest)):
            continue
        shutil.copy(src, dest)


class mav_include(object):
    def __init__(self, base):
        self.base = base


def generate_one(basename, xml):
    '''generate headers for one XML file'''

    directory = os.path.join(basename, xml.basename)

    print("Generating C++ implementation in directory %s" % directory)
    mavparse.mkdir_p(directory)

    if xml.wire_protocol_version != mavparse.PROTOCOL_2_0:
        raise ValueError("C++ implementation only support --wire-protocol=2.0")

    # work out the included headers
    xml.include_list = []
    for i in xml.include:
        base = i[:-4]
        xml.include_list.append(mav_include(base))

    # and message metadata array
    # we sort with primary key msgid
    xml.message_entry_len = len(xml.message_crcs)
    xml.message_entry_array = ', '.join([
        '{%u, %u, %u, %u, %u, %u}' % (
            msgid,
            xml.message_crcs[msgid],
            xml.message_min_lengths[msgid],
            xml.message_flags[msgid],
            xml.message_target_system_ofs[msgid],
            xml.message_target_component_ofs[msgid])
        for msgid in sorted(xml.message_crcs.keys())])

    # add some extra field attributes for convenience with arrays
    for m in xml.message:
        m.dialect_name = xml.basename
        m.msg_name = m.name
        m.const_fileds = []

        for f in m.fields:
            spaces = 30 - len(f.name)
            f.ser_whitespace = ' ' * (spaces if spaces > 1 else 1)

            to_yaml_cast = 'int' if f.type in ['uint8_t', 'int8_t'] else ''

            if f.array_length != 0:
                f.cxx_type = 'std::array<%s, %s>' % (f.type, f.array_length)
                f.to_yaml_code = """ss << "  %s: ["; for (auto &_v : %s) { ss << %s(_v) << ", "; }; ss << "]" << std::endl;""" % (
                    f.name, f.name, to_yaml_cast)
            else:
                f.cxx_type = f.type
                f.to_yaml_code = """ss << "  %s: " << %s(%s) << std::endl;""" % (f.name, to_yaml_cast, f.name)

            # cope with uint8_t_mavlink_version
            if f.omit_arg:
                m.const_fileds.append(f)

    generate_main_hpp(directory, xml)
    for m in xml.message:
        generate_message_hpp(directory, m)
    #generate_testsuite_h(directory, xml)


def generate(basename, xml_list):
    '''generate serialization MAVLink C++ implemenation'''

    for xml in xml_list:
        generate_one(basename, xml)
    #XXX copy_fixed_headers(basename, xml_list[0])
