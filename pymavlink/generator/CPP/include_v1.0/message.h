#ifndef MAVLINK_CPP_MESSAGE_H
#define MAVLINK_CPP_MESSAGE_H

#define X25_INIT_CRC 0xffff
#define X25_VALIDATE_CRC 0xf0b8

#include <iostream>
#include "bytebuffer.h"
#include "protocol.h"

namespace mavlink
{
  class message
  {
    public:
      message(ByteBuffer & buffer):
        m_payload()
      {
        m_header = buffer.get<uint8_t>(0);
        m_length = buffer.get<uint8_t>(1);
        m_sequence_number = buffer.get<uint8_t>(2);
        m_system_id = buffer.get<uint8_t>(3);
        m_component_id = buffer.get<uint8_t>(4);
        m_message_id = buffer.get<uint8_t>(5);
        for(unsigned int i = 6; i < buffer.size() - 2; ++i)
          m_payload << buffer.get<uint8_t>(i);
      }

      message(message const& msg):
        m_header(msg.m_header),
        m_length(msg.m_length),
        m_sequence_number(msg.m_sequence_number),
        m_system_id(msg.m_system_id),
        m_component_id(msg.m_component_id),
        m_message_id(msg.m_message_id),
        m_payload(msg.m_payload)
      {}

      ByteBuffer toByteBuffer() const
      {
        mavlink::ByteBuffer buffer(_toByteBufferWithoutChecksum());

        buffer << _calculateChecksum(buffer);

        return buffer;
      }

      uint8_t get_header() const{return m_header;}
      uint8_t get_length() const{return m_length;}
      uint8_t get_sequence_number() const{return m_sequence_number;}
      uint8_t get_system_id() const{return m_system_id;}
      uint8_t get_component_id() const{return m_component_id;}
      uint8_t get_message_id() const{return m_message_id;}
      ByteBuffer get_payload() const{return m_payload;}

      bool isValid() const
      {
        /*  if the checksum is empty, we cannot compare to see if the message is valid
            but, as the checksum is empty, it means that this message was crafted by
            this application. Therefore, it is valid
        */
        if(m_checksum.size() == 0)
          return true;
        else
        {
          mavlink::ByteBuffer buffer(_toByteBufferWithoutChecksum());

          return m_checksum == _calculateChecksum(buffer);
        }
      }

    protected:
      message(uint8_t length, uint8_t system_id, uint8_t component_id, uint8_t message_id):
        m_header(mavlink::PROTOCOL_MARKER),
        m_length(length),
        m_sequence_number(mavlink::sequenceNumber()),
        m_system_id(system_id),
        m_component_id(component_id),
        m_message_id(message_id)
      {}

      ByteBuffer m_payload;       ///< payload

    private:
      ByteBuffer _toByteBufferWithoutChecksum() const
      {
        mavlink::ByteBuffer buffer;

        buffer << m_header;
        buffer << m_length;
        buffer << m_sequence_number;
        buffer << m_system_id;
        buffer << m_component_id;
        buffer << m_message_id;
        buffer << m_payload;

        return buffer;
      }

      ByteBuffer _calculateChecksum(ByteBuffer const& buffer) const
      {
        mavlink::message::checksum cs;

        //we skip the protocol magic marker (0xFE)
        for(int i = 1; i < buffer.size(); ++i)
          cs.accumulate(buffer[i]);

        if(mavlink::USE_CRC_EXTRA)
          cs.accumulate(mavlink::msg::crcs[m_message_id]);

        uint16_t checksum = (uint16_t) cs;

        mavlink::ByteBuffer retBuffer;
        retBuffer << (uint8_t)(checksum & 0xFF);
        retBuffer << (uint8_t)(checksum >> 8);

        return retBuffer;
      }

      uint8_t m_header;           ///< 0xFE (MAVLink header)
      uint8_t m_length;           ///< payload length
      uint8_t m_sequence_number;  ///< sequence number. Rolls between 0x00 and 0xFF
      uint8_t m_system_id;        ///< id of the sender system
      uint8_t m_component_id;     ///< id of the sender component
      uint8_t m_message_id;       ///< id of the message

      ByteBuffer m_checksum;

      class checksum
      {
        public:
          checksum(){m_crc = X25_INIT_CRC;}

          void accumulate(uint8_t data)
          {
            uint8_t tmp;
            tmp = data ^ (uint8_t)(m_crc &0xff);
            tmp ^= (tmp<<4);
            m_crc = (m_crc>>8) ^ (tmp<<8) ^ (tmp <<3) ^ (tmp>>4);
          }

          operator uint16_t() const{return m_crc;}

        private:
          uint16_t m_crc;
      };
  };
};
#endif /* MAVLINK_CPP_MESSAGE_H */
