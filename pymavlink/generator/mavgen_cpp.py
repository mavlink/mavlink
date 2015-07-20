#!/usr/bin/env python
'''
parse a MAVLink protocol XML file and generate a C++ implementation

Copyright Amaury Louarn 2015

Released under GNU GPL version 3 or later

----
Based on the C generator :
Copyright Andrew Tridgell 2011
'''

import sys, textwrap, os, time
from . import mavparse, mavtemplate

t = mavtemplate.MAVTemplate()

def generate_version_h(directory, xml):
    '''generate version.h'''
    f = open(os.path.join(directory, "version.h"), mode='w')
    t.write(f,'''
/** @file
 *	@brief MAVLink comm protocol built from ${basename}.xml
 *	@see http://mavlink.org
 */
#ifndef MAVLINK_CPP_VERSION_H
#define MAVLINK_CPP_VERSION_H

namespace mavlink
{
  namespace version
  {
    const char * const build_date = "${parse_time}";
    const char * const wire_protocol = "${wire_protocol_version}";
    const uint8_t max_dialect_payload_size = ${largest_payload};
  };
};

#endif // MAVLINK_CPP_VERSION_H
''', xml)
    f.close()

def generate_mavlink_h(directory, xml):
    '''generate mavlink.h'''
    f = open(os.path.join(directory, "mavlink.h"), mode='w')
    t.write(f,'''
/** @file
 *	@brief MAVLink comm protocol built from ${basename}.xml
 *	@see http://mavlink.org
 */
#ifndef MAVLINK_CPP_H
#define MAVLINK_CPP_H

#include <cstdint>

namespace mavlink
{
  const uint8_t PROTOCOL_MARKER = ${protocol_marker};
  const bool PROTOCOL_USE_LITTLE_ENDIAN = ${mavlink_endian};
  const bool ALIGNED_FIELDS = ${aligned_fields_define};
  const bool USE_CRC_EXTRA = ${crc_extra_define};
};

#include "version.h"
#include "${basename}.h"

#endif // MAVLINK_CPP_H
''', xml)
    f.close()

def generate_main_h(directory, xml):
    '''generate main header per XML file'''
    f = open(os.path.join(directory, xml.basename + ".h"), mode='w')
    t.write(f, '''
/** @file
 *	@brief MAVLink comm protocol generated from ${basename}.xml
 *	@see http://mavlink.org
 */
#ifndef MAVLINK_CPP_${basename_upper}_H
#define MAVLINK_CPP_${basename_upper}_H

#ifndef MAVLINK_CPP_H
    #error Wrong include order: MAVLINK_${basename_upper}.H MUST NOT BE DIRECTLY USED. Include mavlink.h from the same directory instead or set ALL AND EVERY defines from MAVLINK.H manually accordingly, including the #define MAVLINK_CPP_H call.
#endif

// MESSAGE LENGTHS AND CRCS

namespace mavlink
{
  namespace msg
  {
      const uint8_t lengths[] = {${message_lengths_array}};
      const uint8_t crcs[] = {${message_crcs_array}};
  };
};

#include "../protocol.h"

#define MAVLINK_ENABLED_${basename_upper}

// ENUM DEFINITIONS

namespace mavlink
{
  ${{enum:
  /** @brief ${description} */
  #ifndef HAVE_ENUM_${name}
  #define HAVE_ENUM_${name}
  typedef enum ${name}
  {
  ${{entry:	${name}=${value}, /* ${description} |${{param:${description}| }} */
  }}
  } ${name};
  #endif
  }}
};



${{include_list:#include "../${base}/${base}.h"
}}

// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION ${version}
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION ${version}
#endif

// MESSAGE DEFINITIONS
${{message:#include "./mavlink_msg_${name_lower}.h"
}}

#endif // MAVLINK_CPP_${basename_upper}_H
''', xml)

    f.close()


def generate_message_h(directory, m):
    '''generate per-message header for a XML file'''
    f = open(os.path.join(directory, 'mavlink_msg_%s.h' % m.name_lower), mode='w')
    t.write(f, '''
#ifndef MAVLINK_CPP_MSG_${name}_H
#define MAVLINK_CPP_MSG_${name}_H

#include "../message.h"

namespace mavlink
{
  namespace msg
  {
    const uint8_t ${name_lower}_id = ${id};
    const uint8_t ${name_lower}_length = ${wire_length};
    const uint8_t ${name_lower}_crc = ${crc_extra};

    class ${name_lower} : public mavlink::message
    {
      public:
        ${name_lower}(uint8_t system_id, uint8_t component_id, ${{arg_fields: ${array_const}${type} ${array_prefix} ${name},}}):
          mavlink::message( mavlink::msg::${name_lower}_length,
                            system_id,
                            component_id,
                            mavlink::msg::${name_lower}_id)
        {
          ${{scalar_fields: m_payload.push_back<${type}>(${putname}); ///< ${description}
          }}
          ${{array_fields:	m_payload.push_back_array<${type}>(${name}, ${array_length}); ///< ${description}
          }}
        }

      ${{scalar_fields:	${type} get_${putname}() const
          {return m_payload.get<${type}>(${wire_offset});}
      }}
      ${{array_fields: ${type} * get_${name}() const
          {return m_payload.get_array<${type}>(${wire_offset}, ${array_length});}
      }}
    };
  };
};

#endif //MAVLINK_CPP_MSG_${name}_H
    ''', m)
    f.close()

def copy_fixed_headers(directory, xml):
    '''copy the fixed protocol headers to the target directory'''
    import shutil
    hlist = [ 'message.h', 'bytebuffer.h', 'protocol.h', 'protocol.cpp' ]
    basepath = os.path.dirname(os.path.realpath(__file__))
    srcpath = os.path.join(basepath, 'CPP/include_v%s' % xml.wire_protocol_version)
    print("Copying fixed headers")
    for h in hlist:
        src = os.path.realpath(os.path.join(srcpath, h))
        dest = os.path.realpath(os.path.join(directory, h))
        if src == dest:
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

    if xml.little_endian:
        xml.mavlink_endian = "true"
    else:
        xml.mavlink_endian = "false"

    if xml.crc_extra:
        xml.crc_extra_define = "true"
    else:
        xml.crc_extra_define = "false"

    if xml.sort_fields:
        xml.aligned_fields_define = "true"
    else:
        xml.aligned_fields_define = "false"

    # work out the included headers
    xml.include_list = []
    for i in xml.include:
        base = i[:-4]
        xml.include_list.append(mav_include(base))

    # form message lengths array
    xml.message_lengths_array = ''
    for mlen in xml.message_lengths:
        xml.message_lengths_array += '%u, ' % mlen
    xml.message_lengths_array = xml.message_lengths_array[:-2]

    # and message CRCs array
    xml.message_crcs_array = ''
    for crc in xml.message_crcs:
        xml.message_crcs_array += '%u, ' % crc
    xml.message_crcs_array = xml.message_crcs_array[:-2]

    # form message info array
    xml.message_info_array = ''
    for name in xml.message_names:
        if name is not None:
            xml.message_info_array += 'MAVLINK_MESSAGE_INFO_%s, ' % name
        else:
            # Several C compilers don't accept {NULL} for
            # multi-dimensional arrays and structs
            # feed the compiler a "filled" empty message
            xml.message_info_array += '{"EMPTY",0,{{"","",MAVLINK_TYPE_CHAR,0,0,0}}}, '
    xml.message_info_array = xml.message_info_array[:-2]

    # add some extra field attributes for convenience with arrays
    for m in xml.message:
        m.msg_name = m.name
        if xml.crc_extra:
            m.crc_extra_arg = ", %s" % m.crc_extra
        else:
            m.crc_extra_arg = ""
        for f in m.fields:
            if f.print_format is None:
                f.c_print_format = 'NULL'
            else:
                f.c_print_format = '"%s"' % f.print_format
            if f.array_length != 0:
                f.array_suffix = '[%u]' % f.array_length
                f.array_prefix = '*'
                f.array_tag = '_array'
                f.array_arg = ', %u' % f.array_length
                f.array_return_arg = '%s, %u, ' % (f.name, f.array_length)
                f.array_const = 'const '
                f.decode_left = ''
                f.decode_right = ', %s->%s' % (m.name_lower, f.name)
                f.return_type = 'uint16_t'
                f.get_arg = ', %s *%s' % (f.type, f.name)
                if f.type == 'char':
                    f.c_test_value = '"%s"' % f.test_value
                else:
                    test_strings = []
                    for v in f.test_value:
                        test_strings.append(str(v))
                    f.c_test_value = '{ %s }' % ', '.join(test_strings)
            else:
                f.array_suffix = ''
                f.array_prefix = ''
                f.array_tag = ''
                f.array_arg = ''
                f.array_return_arg = ''
                f.array_const = ''
                f.decode_left = "%s->%s = " % (m.name_lower, f.name)
                f.decode_right = ''
                f.get_arg = ''
                f.return_type = f.type
                if f.type == 'char':
                    f.c_test_value = "'%s'" % f.test_value
                elif f.type == 'uint64_t':
                    f.c_test_value = "%sULL" % f.test_value
                elif f.type == 'int64_t':
                    f.c_test_value = "%sLL" % f.test_value
                else:
                    f.c_test_value = f.test_value

    # cope with uint8_t_mavlink_version
    for m in xml.message:
        m.arg_fields = []
        m.array_fields = []
        m.scalar_fields = []
        for f in m.ordered_fields:
            if f.array_length != 0:
                m.array_fields.append(f)
            else:
                m.scalar_fields.append(f)
        for f in m.fields:
            if not f.omit_arg:
                m.arg_fields.append(f)
                f.putname = f.name
            else:
                f.putname = f.const_value

    generate_mavlink_h(directory, xml)
    generate_version_h(directory, xml)
    generate_main_h(directory, xml)
    for m in xml.message:
        generate_message_h(directory, m)


def generate(basename, xml_list):
    '''generate complete MAVLink C++ implemenation'''

    for xml in xml_list:
        generate_one(basename, xml)
    copy_fixed_headers(basename, xml_list[0])
