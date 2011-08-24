#ifndef  _MAVLINK_PROTOCOL_H_
#define  _MAVLINK_PROTOCOL_H_

#include "string.h"
#include "mavlink_types.h"

/* 
   If you want MAVLink on a system that is native big-endian,
   you need to define NATIVE_BIG_ENDIAN
*/
#ifdef NATIVE_BIG_ENDIAN
# define MAVLINK_NEED_BYTE_SWAP (MAVLINK_ENDIAN == MAVLINK_LITTLE_ENDIAN)
#else
# define MAVLINK_NEED_BYTE_SWAP (MAVLINK_ENDIAN != MAVLINK_LITTLE_ENDIAN)
#endif

#ifdef MAVLINK_SEPARATE_HELPERS
#define MAVLINK_HELPER
#else
#define MAVLINK_HELPER static inline
#include "mavlink_helpers.h"
#endif // MAVLINK_SEPARATE_HELPERS

/* always include the prototypes to ensure we don't get out of sync */
MAVLINK_HELPER mavlink_status_t* mavlink_get_channel_status(uint8_t chan);
MAVLINK_HELPER uint16_t mavlink_finalize_message_chan(mavlink_message_t* msg, uint8_t system_id, uint8_t component_id, 
						      uint8_t chan, uint16_t length, uint8_t crc_extra);
MAVLINK_HELPER uint16_t mavlink_finalize_message(mavlink_message_t* msg, uint8_t system_id, uint8_t component_id, 
						 uint16_t length, uint8_t crc_extra);
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS
MAVLINK_HELPER void mavlink_finalize_message_chan_send(mavlink_message_t* msg,
						       mavlink_channel_t chan, uint16_t length, uint8_t crc_extra);
#endif
MAVLINK_HELPER uint16_t mavlink_msg_to_send_buffer(uint8_t *buffer, const mavlink_message_t *msg);
MAVLINK_HELPER void mavlink_start_checksum(mavlink_message_t* msg);
MAVLINK_HELPER void mavlink_update_checksum(mavlink_message_t* msg, uint8_t c);
MAVLINK_HELPER uint8_t mavlink_parse_char(uint8_t chan, uint8_t c, mavlink_message_t* r_message, mavlink_status_t* r_mavlink_status);
MAVLINK_HELPER uint8_t put_bitfield_n_by_index(int32_t b, uint8_t bits, uint8_t packet_index, uint8_t bit_index, 
					       uint8_t* r_bit_index, uint8_t* buffer);
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS
MAVLINK_HELPER void mavlink_send_uart(mavlink_channel_t chan, mavlink_message_t* msg);
#endif

/**
 * @brief Get the required buffer size for this message
 */
static inline uint16_t mavlink_msg_get_send_buffer_length(const mavlink_message_t* msg)
{
	return msg->len + MAVLINK_NUM_NON_PAYLOAD_BYTES;
}

#if MAVLINK_NEED_BYTE_SWAP
typedef union __generic_16bit
{
	uint8_t b[2];
	int16_t s;
} generic_16bit;

typedef union __generic_32bit
{
	uint8_t b[4];
	float f;
	int32_t i;
	int16_t s;
} generic_32bit;

typedef union __generic_64bit
{
	uint8_t b[8];
	int64_t ll; ///< Long long (64 bit)
	double  d;  ///< IEEE-754 double precision floating point
} generic_64bit;
#endif // MAVLINK_NEED_BYTE_SWAP

/**
 * @brief Place an unsigned byte into the buffer
 *
 * @param b the byte to add
 * @param bindex the position in the packet
 * @param buffer the packet buffer
 */
static inline void put_uint8_t_by_index(uint8_t b, uint8_t bindex, uint8_t* buffer)
{
	*(buffer + bindex) = b;
}

/**
 * @brief Place a signed byte into the buffer
 *
 * @param b the byte to add
 * @param bindex the position in the packet
 * @param buffer the packet buffer
 * @return the new position of the last used byte in the buffer
 */
static inline void put_int8_t_by_index(int8_t b, int8_t bindex, uint8_t* buffer)
{
	*(buffer + bindex) = (uint8_t)b;
}

/**
 * @brief Place two unsigned bytes into the buffer
 *
 * @param b the bytes to add
 * @param bindex the position in the packet
 * @param buffer the packet buffer
 */
static inline void put_uint16_t_by_index(uint16_t b, const uint8_t bindex, uint8_t* buffer)
{
#if MAVLINK_NEED_BYTE_SWAP
	buffer[bindex]   = (b>>8)&0xff;
	buffer[bindex+1] = (b & 0xff);
#else
	*(uint16_t *)(buffer+bindex) = b;
#endif
}

/**
 * @brief Place two signed bytes into the buffer
 *
 * @param b the bytes to add
 * @param bindex the position in the packet
 * @param buffer the packet buffer
 */
static inline void put_int16_t_by_index(int16_t b, uint8_t bindex, uint8_t* buffer)
{
	put_uint16_t_by_index(b, bindex, buffer);
}

/**
 * @brief Place four unsigned bytes into the buffer
 *
 * @param b the bytes to add
 * @param bindex the position in the packet
 * @param buffer the packet buffer
 * @return the new position of the last used byte in the buffer
 */
static inline void put_uint32_t_by_index(uint32_t b, const uint8_t bindex, uint8_t* buffer)
{
#if MAVLINK_NEED_BYTE_SWAP
	buffer[bindex]   = (b>>24)&0xff;
	buffer[bindex+1] = (b>>16)&0xff;
	buffer[bindex+2] = (b>>8)&0xff;
	buffer[bindex+3] = (b & 0xff);
#else
	*(uint32_t *)(buffer+bindex) = b;
#endif
}

/**
 * @brief Place four signed bytes into the buffer
 *
 * @param b the bytes to add
 * @param bindex the position in the packet
 * @param buffer the packet buffer
 * @return the new position of the last used byte in the buffer
 */
static inline void put_int32_t_by_index(int32_t b, uint8_t bindex, uint8_t* buffer)
{
#if MAVLINK_NEED_BYTE_SWAP
	buffer[bindex]   = (b>>24)&0xff;
	buffer[bindex+1] = (b>>16)&0xff;
	buffer[bindex+2] = (b>>8)&0xff;
	buffer[bindex+3] = (b & 0xff);
#else
	*(int32_t *)(buffer+bindex) = b;
#endif
}

/**
 * @brief Place four unsigned bytes into the buffer
 *
 * @param b the bytes to add
 * @param bindex the position in the packet
 * @param buffer the packet buffer
 * @return the new position of the last used byte in the buffer
 */
static inline void put_uint64_t_by_index(uint64_t b, const uint8_t bindex, uint8_t* buffer)
{
#if MAVLINK_NEED_BYTE_SWAP
	buffer[bindex]   = (b>>56)&0xff;
	buffer[bindex+1] = (b>>48)&0xff;
	buffer[bindex+2] = (b>>40)&0xff;
	buffer[bindex+3] = (b>>32)&0xff;
	buffer[bindex+4] = (b>>24)&0xff;
	buffer[bindex+5] = (b>>16)&0xff;
	buffer[bindex+6] = (b>>8)&0xff;
	buffer[bindex+7] = (b & 0xff);
#else
	*(uint64_t *)(buffer+bindex) = b;
#endif
}

/**
 * @brief Place four signed bytes into the buffer
 *
 * @param b the bytes to add
 * @param bindex the position in the packet
 * @param buffer the packet buffer
 * @return the new position of the last used byte in the buffer
 */
static inline void put_int64_t_by_index(int64_t b, uint8_t bindex, uint8_t* buffer)
{
	put_uint64_t_by_index(b, bindex, buffer);
}

/**
 * @brief Place a float into the buffer
 *
 * @param b the float to add
 * @param bindex the position in the packet
 * @param buffer the packet buffer
 * @return the new position of the last used byte in the buffer
 */
static inline void put_float_by_index(float b, uint8_t bindex, uint8_t* buffer)
{
#if MAVLINK_NEED_BYTE_SWAP
	generic_32bit g;
	g.f = b;
	put_int32_t_by_index(g.i, bindex, buffer);
#else
	*(float *)(buffer+bindex) = b;
#endif
}

/*
 * Place a char array into a buffer
 */
static inline void put_char_array_by_index(const char *b, uint8_t wire_offset, uint8_t array_length, uint8_t *buffer)
{
	memcpy(buffer+wire_offset, b, array_length);
}

/*
 * Place a uint8_t array into a buffer
 */
static inline void put_uint8_t_array_by_index(const uint8_t *b, uint8_t wire_offset, uint8_t array_length, uint8_t *buffer)
{
	memcpy(buffer+wire_offset, b, array_length);
}

/*
 * Place a int8_t array into a buffer
 */
static inline void put_int8_t_array_by_index(const int8_t *b, uint8_t wire_offset, uint8_t array_length, uint8_t *buffer)
{
	memcpy(buffer+wire_offset, b, array_length);
}

#define MAVLINK_MSG_RETURN_int8_t(msg, wire_offset) (uint8_t)msg->payload[wire_offset]
#define MAVLINK_MSG_RETURN_uint8_t(msg, wire_offset) (uint8_t)msg->payload[wire_offset]

#ifndef MAVLINK_MSG_RETURN_uint16_t
static inline uint16_t MAVLINK_MSG_RETURN_uint16_t(const mavlink_message_t *msg, uint8_t wire_offset)
{
#if MAVLINK_NEED_BYTE_SWAP
	generic_16bit r;
	r.b[1] = msg->payload[wire_offset+0];
	r.b[0] = msg->payload[wire_offset+1];
	return (uint16_t)r.s;
#else
	return *(uint16_t *)(&msg->payload[wire_offset]);
#endif
}
#endif

#ifndef MAVLINK_MSG_RETURN_int16_t
#define MAVLINK_MSG_RETURN_int16_t(msg, wire_offset) (int16_t)MAVLINK_MSG_RETURN_uint16_t(msg, wire_offset)
#endif

#ifndef MAVLINK_MSG_RETURN_uint32_t
static inline uint32_t MAVLINK_MSG_RETURN_uint32_t(const mavlink_message_t *msg, uint8_t wire_offset)
{
#if MAVLINK_NEED_BYTE_SWAP
	generic_32bit r;
	r.b[3] = msg->payload[wire_offset+0];
	r.b[2] = msg->payload[wire_offset+1];
	r.b[1] = msg->payload[wire_offset+2];
	r.b[0] = msg->payload[wire_offset+3];
	return (uint32_t)r.i;
#else
	return *(uint32_t *)(&msg->payload[wire_offset]);
#endif
}
#endif

#ifndef MAVLINK_MSG_RETURN_int32_t
#define MAVLINK_MSG_RETURN_int32_t(msg, wire_offset) (int32_t)MAVLINK_MSG_RETURN_uint32_t(msg, wire_offset)
#endif

#ifndef MAVLINK_MSG_RETURN_uint64_t
static inline uint64_t MAVLINK_MSG_RETURN_uint64_t(const mavlink_message_t *msg, uint8_t wire_offset)
{
#if MAVLINK_NEED_BYTE_SWAP
	generic_64bit r;
	r.b[7] = msg->payload[wire_offset+0];
	r.b[6] = msg->payload[wire_offset+1];
	r.b[5] = msg->payload[wire_offset+2];
	r.b[4] = msg->payload[wire_offset+3];
	r.b[3] = msg->payload[wire_offset+4];
	r.b[2] = msg->payload[wire_offset+5];
	r.b[1] = msg->payload[wire_offset+6];
	r.b[0] = msg->payload[wire_offset+7];
	return (uint64_t)r.ll;
#else
	return *(uint64_t *)(&msg->payload[wire_offset]);
#endif
}
#endif

#ifndef MAVLINK_MSG_RETURN_int64_t
#define MAVLINK_MSG_RETURN_int64_t(msg, wire_offset) (int64_t)MAVLINK_MSG_RETURN_uint64_t(msg, wire_offset)
#endif

#ifndef MAVLINK_MSG_RETURN_float
static inline float MAVLINK_MSG_RETURN_float(const mavlink_message_t *msg, uint8_t wire_offset)
{
#if MAVLINK_NEED_BYTE_SWAP
	generic_32bit r;
	r.b[3] = msg->payload[wire_offset+0];
	r.b[2] = msg->payload[wire_offset+1];
	r.b[1] = msg->payload[wire_offset+2];
	r.b[0] = msg->payload[wire_offset+3];
	return r.f;
#else
	return *(float *)(&msg->payload[wire_offset]);
#endif
}
#endif

static inline uint16_t MAVLINK_MSG_RETURN_char_array(const mavlink_message_t *msg, char *value, 
						     uint8_t array_length, uint8_t wire_offset)
{
	memcpy(value, &msg->payload[wire_offset], array_length);
	return array_length;
}

static inline uint16_t MAVLINK_MSG_RETURN_uint8_t_array(const mavlink_message_t *msg, uint8_t *value, 
							uint8_t array_length, uint8_t wire_offset)
{
	memcpy(value, &msg->payload[wire_offset], array_length);
	return array_length;
}

static inline uint16_t MAVLINK_MSG_RETURN_int8_t_array(const mavlink_message_t *msg, int8_t *value, 
						       uint8_t array_length, uint8_t wire_offset)
{
	memcpy(value, &msg->payload[wire_offset], array_length);
	return array_length;
}

/*
  this rather strange macro creates a mavlink_message_t structure on
  the stack, with the payload aligned on whatever boundary this
  machine needs for accessing a uint64_t.
 */
#ifndef MAVLINK_ALIGNED_MESSAGE
#define MAVLINK_ALIGNMENT_OFFSET (8-MAVLINK_NUM_HEADER_BYTES)
#define MAVLINK_ALIGNED_MESSAGE(msg, length) \
union { \
	uint64_t _uint64; \
	uint8_t  _buf[MAVLINK_ALIGNMENT_OFFSET+MAVLINK_NUM_NON_PAYLOAD_BYTES+(length)]; \
} _buffer; \
mavlink_message_t *msg = (mavlink_message_t *)&_buffer._buf[MAVLINK_ALIGNMENT_OFFSET]
#endif // MAVLINK_ALIGNED_MESSAGE

#undef MAVLINK_HELPER
#endif /* _MAVLINK_PROTOCOL_H_ */
