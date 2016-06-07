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


def generate_mavlink_hpp(directory, xml):
    '''generate mavlink.h'''
    f = open(os.path.join(directory, "mavlink.hpp"), mode='w')
    t.write(f,'''
/** @file
 *	@brief MAVLink comm protocol built from ${basename}.xml
 *	@see http://mavlink.org
 */

#pragma once

#ifndef MAVLINK_STX
#define MAVLINK_STX ${protocol_marker}
#endif

#ifndef MAVLINK_ENDIAN
#define MAVLINK_ENDIAN $ {mavlink_endian}
#endif

#ifndef MAVLINK_ALIGNED_FIELDS
#define MAVLINK_ALIGNED_FIELDS $ {aligned_fields_define}
#endif

#ifndef MAVLINK_CRC_EXTRA
#define MAVLINK_CRC_EXTRA $ {crc_extra_define}
#endif

#ifndef MAVLINK_COMMAND_24BIT
#define MAVLINK_COMMAND_24BIT $ {command_24bit_define}
#endif

#ifndef MAVLINK_PACKED
#define MAVLINK_PACKED __attribute__((__packed__))
#endif

#include "version.h"
#include "${basename}.h"

// XXX ???? do i realy need that file????
''', xml)
    f.close()


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

//#include "../protocol.h"
#include "../message.hpp"

namespace mavlink {
namespace ${basename} {

/**
 * Array of msg_entry needed for @p mavlink_parse_char() (trought @p mavlink_get_msg_entry())
 */
//constexpr std::array<mavlink_msg_entry_t, ${message_entry_len}> MESSAGE_ENTRIES {{ ${message_entry_array} }};


// ENUM DEFINITIONS

${{enum:
/** @brief ${description} */
enum class ${name} : int
{
${{entry:    ${name}=${value}, /* ${description} |${{param:${description}| }} */
}}
};
}}

// MAVLINK VERSION

// XXX for what?
#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION ${version}
#endif

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
${{fields:        //XXX fix me! ss << "  ${name}: " << ${name} << std::endl;
}}

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map)
    {
        map.reset_header(MSG_ID);

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
        raise ValueError("C++ implementation require --wire-protocol=2.0")

    #if xml.little_endian:
    #    xml.mavlink_endian = "MAVLINK_LITTLE_ENDIAN"
    #else:
    #    xml.mavlink_endian = "MAVLINK_BIG_ENDIAN"

    #if xml.crc_extra:
    #    xml.crc_extra_define = "1"
    #else:
    #    xml.crc_extra_define = "0"

    #if xml.command_24bit:
    #    xml.command_24bit_define = "1"
    #else:
    #    xml.command_24bit_define = "0"

    #if xml.sort_fields:
    #    xml.aligned_fields_define = "1"
    #else:
    #    xml.aligned_fields_define = "0"

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

            if f.array_length != 0:
                f.cxx_type = 'std::array<%s, %s>' % (f.type, f.array_length)
            else:
                f.cxx_type = f.type

            # cope with uint8_t_mavlink_version
            if f.omit_arg:
                m.const_fileds.append(f)

    generate_mavlink_hpp(directory, xml)
    generate_main_hpp(directory, xml)
    for m in xml.message:
        generate_message_hpp(directory, m)
    #generate_testsuite_h(directory, xml)


def generate(basename, xml_list):
    '''generate serialization MAVLink C++ implemenation'''

    for xml in xml_list:
        generate_one(basename, xml)
    #XXX copy_fixed_headers(basename, xml_list[0])
