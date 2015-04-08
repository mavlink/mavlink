/**
 * @file protocol_c2000.h
 * @author Aaron Bamberger (abamberger@aesaustin.com)
 * @date December, 2014
 * @brief Data packing, unpacking, and CRC functions for the TI C2000 architecture
 *
 * The C2000 architecture doesn't have byte-level addressing, only (16-bit) word level addressing,
 * which means, in addition to other things, the char and uint8_t types are actually 16-bits wide.
 * This necessitates special handling of all data packing and unpacking into and out of mavlink_message_t
 * payloads, as well as special consideration when calculating CRCs.  This file contains support functions
 * that replace some of the normal MAVLink library support functions when compiling for the C2000 architecture
 * which do the necessary work to unpack and repack message payloads while handing the architectural differences
 */

#ifndef PROTOCOL_C2000_H_
#define PROTOCOL_C2000_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "f2806x_int8.h"
#include <stdint.h>

// This is necessary because the uint64_t and int64_t definitions are only enabled in stdint.h if __TMS320C28X__ is defined
// This must only be defined by the compiler under specific circumstances (which I haven't figured out yet), because sometimes the
// stdint.h definitions are enabled and sometimes they aren't.  To ensure this file will always compile, add the typedefs ourselves
// if the stdint.h ones don't come through
#ifndef uint64_t
typedef unsigned long long uint64_t;
#endif
#ifndef int64_t
typedef long long int64_t;
#endif

#define X25_INIT_CRC_C2000 0xffff

/**
 * @brief Insert a 32-bit floating point number into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_float_c2000(void* buf, int wire_offset, float b)
{
    float* dest_ptr = (float*)((uint8_t*)buf + (wire_offset / 2));
    *dest_ptr = b;
}

/**
 * @brief Insert an unsigned 64-bit integer into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_uint64_t_c2000(void* buf, int wire_offset, uint64_t b)
{
    uint64_t* dest_ptr = (uint64_t*)((uint8_t*)buf + (wire_offset / 2));
    *dest_ptr = b;
}

/**
 * @brief Insert a signed 64-bit integer into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_int64_t_c2000(void* buf, int wire_offset, int64_t b)
{
    mav_put_uint64_t_c2000(buf, wire_offset, (uint64_t)b);
}

/**
 * @brief Insert an unsigned 32-bit integer into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_uint32_t_c2000(void* buf, int wire_offset, uint32_t b)
{
    uint32_t* dest_ptr = (uint32_t*)((uint8_t*)buf + (wire_offset / 2));
    *dest_ptr = b;
}

/**
 * @brief Insert a signed 32-bit integer into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_int32_t_c2000(void* buf, int wire_offset, int32_t b)
{
    mav_put_uint32_t_c2000(buf, wire_offset, (uint32_t)b);
}

/**
 * @brief Insert an unsigned 16-bit integer into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_uint16_t_c2000(void* buf, int wire_offset, uint16_t b)
{
    uint16_t* dest_ptr = (uint16_t*)((uint8_t*)buf + (wire_offset / 2));
    *dest_ptr = b;
}

/**
 * @brief Insert a signed 16-bit integer into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_int16_t_c2000(void* buf, int wire_offset, int16_t b)
{
    mav_put_uint16_t_c2000(buf, wire_offset, (uint16_t)b);
}

/**
 * @brief Insert an unsigned 8-bit integer into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_uint8_t_c2000(void* buf, int wire_offset, uint8_t b)
{
    uint8_t* dest_ptr = (uint8_t*)buf;
    if ((wire_offset % 2) == 0) {
        dest_ptr += (wire_offset / 2);
        *dest_ptr &= 0xFF00;
        *dest_ptr |= (b & 0x00FF);
    } else {
        dest_ptr += ((wire_offset - 1) / 2);
        *dest_ptr &= 0x00FF;
        *dest_ptr |= ((b << 8) & 0xFF00);
    }
}

/**
 * @brief Insert a signed 8-bit integer into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_int8_t_c2000(void* buf, int wire_offset, int8_t b)
{
    mav_put_uint8_t_c2000(buf, wire_offset, (uint8_t)b);
}

/**
 * @brief Insert a character into the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be inserted
 * @param b Value to be inserted into the payload
 *
 */
static inline void mav_put_char_t_c2000(void* buf, int wire_offset, char b)
{
    mav_put_uint8_t_c2000(buf, wire_offset, (uint8_t)b);
}

/**
 * @brief Copy an array of floats into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_float_array_c2000(void* buf_dest, const float* buf_src, int wire_offset, int len)
{
    int i;
    if (buf_src == NULL) {
        for (i = 0; i < len; i++) {
            mav_put_float_c2000(buf_dest, wire_offset + i, 0.0);
        }
    } else {
        for (i = 0; i < len; i++) {
            mav_put_float_c2000(buf_dest, wire_offset + i, buf_src[i]);
        }
    }
}

/**
 * @brief Copy an array of unsigned 64-bit integers into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_uint64_t_array_c2000(void* buf_dest, const uint64_t* buf_src, int wire_offset, int len)
{
    int i;
    if (buf_src == NULL) {
        for (i = 0; i < len; i++) {
            mav_put_uint64_t_c2000(buf_dest, wire_offset + i, 0x0000000000000000);
        }
    } else {
        for (i = 0; i < len; i++) {
            mav_put_uint64_t_c2000(buf_dest, wire_offset + i, buf_src[i]);
        }
    }
}

/**
 * @brief Copy an array of signed 64-bit integers into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_int64_t_array_c2000(void* buf_dest, const int64_t* buf_src, int wire_offset, int len)
{
    mav_put_uint64_t_array_c2000(buf_dest, (const uint64_t*)buf_src, wire_offset, len);
}

/**
 * @brief Copy an array of unsigned 32-bit integers into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_uint32_t_array_c2000(void* buf_dest, const uint32_t* buf_src, int wire_offset, int len)
{
    int i;
    if (buf_src == NULL) {
        for (i = 0; i < len; i++) {
            mav_put_uint32_t_c2000(buf_dest, wire_offset + i, 0x00000000);
        }
    } else {
        for (i = 0; i < len; i++) {
            mav_put_uint32_t_c2000(buf_dest, wire_offset + i, buf_src[i]);
        }
    }
}

/**
 * @brief Copy an array of signed 32-bit integers into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_int32_t_array_c2000(void* buf_dest, const int32_t* buf_src, int wire_offset, int len)
{
    mav_put_uint32_t_array_c2000(buf_dest, (const uint32_t*)buf_src, wire_offset, len);
}


/**
 * @brief Copy an array of unsigned 16-bit integers into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_uint16_t_array_c2000(void* buf_dest, const uint16_t* buf_src, int wire_offset, int len)
{
    int i;
    if (buf_src == NULL) {
        for (i = 0; i < len; i++) {
            mav_put_uint16_t_c2000(buf_dest, wire_offset + i, 0x0000);
        }
    } else {
        for (i = 0; i < len; i++) {
            mav_put_uint16_t_c2000(buf_dest, wire_offset + i, buf_src[i]);
        }
    }
}

/**
 * @brief Copy an array of signed 16-bit integers into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_int16_t_array_c2000(void* buf_dest, const int16_t* buf_src, int wire_offset, int len)
{
    mav_put_uint16_t_array_c2000(buf_dest, (const uint16_t*)buf_src, wire_offset, len);
}

/**
 * @brief Copy an array of characters into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_char_array_c2000(void* buf_dest, const char* buf_src, int wire_offset, int len)
{
    int i;
    if (buf_src == NULL) {
        for (i = 0; i < len; i++) {
            mav_put_uint8_t_c2000(buf_dest, wire_offset + i, 0x0000);
        }
    } else {
        for (i = 0; i < len; i++) {
            mav_put_uint8_t_c2000(buf_dest, wire_offset + i, buf_src[i]);
        }
    }
}

/**
 * @brief Copy an array of unsigned 8-bit integers into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_uint8_t_array_c2000(void* buf_dest, const uint8_t* buf_src, int wire_offset, int len)
{
    mav_put_char_array_c2000(buf_dest, (const char*)buf_src, wire_offset, len);
}

/**
 * @brief Copy an array of signed 8-bit integers into the payload of a message for the C2000 architecture
 *
 * @param buf_dest Pointer to beginning of MAVLink message payload
 * @param buf_src Pointer to beginning of array to be copied
 * @param wire_offset Offset in bytes into the payload where the array should be copied
 * @param len Length of the array to be copied in array elements (not bytes)
 *
 */
static inline void mav_put_int8_t_array_c2000(void* buf_dest, const int8_t* buf_src, int wire_offset, int len)
{
    mav_put_char_array_c2000(buf_dest, (const char*)buf_src, wire_offset, len);
}

/**
 * @brief Retrieve a 32-bit floating point number from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline float mav_get_float_c2000(const void* buf, int wire_offset)
{
    float* dest_ptr = (float*)((uint8_t*)buf + (wire_offset / 2));
    return *dest_ptr;
}

/**
 * @brief Retrieve an unsigned 64-bit integer from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline uint64_t mav_get_uint64_t_c2000(const void* buf, int wire_offset)
{
    uint64_t* dest_ptr = (uint64_t*)((uint8_t*)buf + (wire_offset / 2));
    return *dest_ptr;
}

/**
 * @brief Retrieve a signed 64-bit integer from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline int64_t mav_get_int64_t_c2000(const void* buf, int wire_offset)
{
    return (int64_t)mav_get_uint64_t_c2000(buf, wire_offset);
}

/**
 * @brief Retrieve an unsigned 32-bit integer from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline uint32_t mav_get_uint32_t_c2000(const void* buf, int wire_offset)
{
    uint32_t* dest_ptr = (uint32_t*)((uint8_t*)buf + (wire_offset / 2));
    return *dest_ptr;
}

/**
 * @brief Retrieve a signed 32-bit integer from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline int32_t mav_get_int32_t_c2000(const void* buf, int wire_offset)
{
    return (int32_t)mav_get_uint32_t_c2000(buf, wire_offset);
}

/**
 * @brief Retrieve an unsigned 16-bit integer from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline uint16_t mav_get_uint16_t_c2000(const void* buf, int wire_offset)
{
    uint16_t* dest_ptr = (uint16_t*)((uint8_t*)buf + (wire_offset / 2));
    return *dest_ptr;
}

/**
 * @brief Retrieve a signed 16-bit integer from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline int16_t mav_get_int16_t_c2000(const void* buf, int wire_offset)
{
    return (int16_t)mav_get_uint16_t_c2000(buf, wire_offset);
}

/**
 * @brief Retrieve an unsigned 8-bit integer from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline uint8_t mav_get_uint8_t_c2000(const void* buf, int wire_offset)
{
    uint8_t* dest_ptr = (uint8_t*)buf;
    if ((wire_offset % 2) == 0) {
        dest_ptr += (wire_offset / 2);
        return (*dest_ptr & 0x00FF);
    } else {
        dest_ptr += ((wire_offset - 1) / 2);
        return ((*dest_ptr >> 8) & 0x00FF);
    }
}

/**
 * @brief Retrieve a signed 8-bit integer from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline int8_t mav_get_int8_t_c2000(const void* buf, int wire_offset)
{
    return (int8_t)mav_get_uint8_t_c2000(buf, wire_offset);
}

/**
 * @brief Retrieve a character from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param wire_offset Offset in bytes into the payload where the value should be fetched from
 * @return Value retrieved from MAVLink message payload
 *
 */
static inline int8_t mav_get_char_c2000(const void* buf, int wire_offset)
{
    return (char)mav_get_uint8_t_c2000(buf, wire_offset);
}

/**
 * @brief Retrieve an array of floating point numbers from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_float_array_c2000(const void* buf, float* value, int array_length, int wire_offset)
{
    int i;
    for (i = 0; i < array_length; i++) {
        value[i] = mav_get_float_c2000(buf, wire_offset + i);
    }

    return array_length;
}

/**
 * @brief Retrieve an array of unsigned 64-bit integers from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_uint64_t_array_c2000(const void* buf, uint64_t* value, int array_length, int wire_offset)
{
    int i;
    for (i = 0; i < array_length; i++) {
        value[i] = mav_get_uint64_t_c2000(buf, wire_offset + i);
    }

    return array_length;
}

/**
 * @brief Retrieve an array of signed 64-bit integers from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_int64_t_array_c2000(const void* buf, int64_t* value, int array_length, int wire_offset)
{
    return mav_get_uint64_t_array_c2000(buf, (uint64_t*)value, array_length, wire_offset);
}

/**
 * @brief Retrieve an array of unsigned 32-bit integers from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_uint32_t_array_c2000(const void* buf, uint32_t* value, int array_length, int wire_offset)
{
    int i;
    for (i = 0; i < array_length; i++) {
        value[i] = mav_get_uint32_t_c2000(buf, wire_offset + i);
    }

    return array_length;
}

/**
 * @brief Retrieve an array of signed 32-bit integers from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_int32_t_array_c2000(const void* buf, int32_t* value, int array_length, int wire_offset)
{
    return mav_get_uint32_t_array_c2000(buf, (uint32_t*)value, array_length, wire_offset);
}

/**
 * @brief Retrieve an array of unsigned 16-bit integers from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_uint16_t_array_c2000(const void* buf, uint16_t* value, int array_length, int wire_offset)
{
    int i;
    for (i = 0; i < array_length; i++) {
        value[i] = mav_get_uint16_t_c2000(buf, wire_offset + i);
    }

    return array_length;
}

/**
 * @brief Retrieve an array of signed 16-bit integers from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_int16_t_array_c2000(const void* buf, int16_t* value, int array_length, int wire_offset)
{
    return mav_get_uint16_t_array_c2000(buf, (uint16_t*)value, array_length, wire_offset);
}

/**
 * @brief Retrieve an array of characters from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_char_array_c2000(const void* buf, char* value, int array_length, int wire_offset)
{
    int i;
    for (i = 0; i < array_length; i++) {
        value[i] = mav_get_uint8_t_c2000(buf, wire_offset + i);
    }

    return array_length;
}

/**
 * @brief Retrieve an array of unsigned 8-bit integers from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_uint8_t_array_c2000(const void* buf, uint8_t* value, int array_length, int wire_offset)
{
    return mav_get_char_array_c2000(buf, (char*)value, array_length, wire_offset);
}

/**
 * @brief Retrieve an array of signed 8-bit integers from the payload of a message for the C2000 architecture
 *
 * @param buf Pointer to beginning of MAVLink message payload
 * @param value Pointer to buffer where array should be extracted to
 * @param array_length Length of array to fetch in array elements (not bytes)
 * @param wire_offset Offset in bytes into the payload where the array should be fetched from
 * @return Length of the array retrieved (in array elements, not bytes)
 *
 */
static inline uint16_t mav_get_int8_t_array_c2000(const void* buf, int8_t* value, int array_length, int wire_offset)
{
    return mav_get_char_array_c2000(buf, (char*)value, array_length, wire_offset);
}

/**
 * @brief Initialize the buffer for the X.25 CRC
 *
 * This is identical to the non-C2000 version of this function (the constant used is also identical)
 * It is simply replicated here to remove a circular dependency issue between this header and checksum.h
 *
 * @param crcAccum the 16 bit X.25 CRC
 */
static inline void crc_init_c2000(uint16_t* crcAccum)
{
        *crcAccum = X25_INIT_CRC_C2000;
}

/**
 * @brief Accumulate the X.25 CRC by adding one char at a time.
 *
 * The checksum function adds the hash of one char at a time to the
 * 16 bit checksum (uint16_t).
 *
 * The C2000 version is unique in that it needs to do extra masking of the incoming data byte
 * and temporary accumulator byte, since uint8_t on C2000 is actually 16-bits.  Without doing
 * the extra masking rollover into the upper byte of the word causes the CRC to be calculated
 * incorrectly
 *
 * @param data new char to hash
 * @param crcAccum the already accumulated checksum
 */
static inline void crc_accumulate_c2000(uint8_t data, uint16_t *crcAccum)
{
    /*Accumulate one byte of data into the CRC*/
    uint8_t tmp = 0x0000;

    // For C2000, since bytes are actually stored in the lower half of a 16-bit word,
    // need to make sure we mask off the upper half of the word so we don't accidentally accumulate
    // unwanted garbage data into the CRC
    tmp = (data & 0x00FF) ^ (uint8_t)(*crcAccum & 0x00ff);
    tmp ^= (tmp << 4);
    tmp &= 0x00FF; // Need to zero-out the upper byte of tmp, since it's intended to be an 8-bit value (but is actually 16-bits),
                   // and rolling it back into the 16-bit crcAccum with garbage in the upper byte will corrupt the CRC
    *crcAccum = (*crcAccum >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4);
}

/**
 * @brief Calculates the X.25 checksum on a byte buffer
 *
 * The only difference between this and the non-C2000 version is that this calls the C2000 specific
 * version of crc_accumulate to make sure that 16-bit "bytes" are handled correctly
 *
 * @param  pBuffer buffer containing the byte array to hash
 * @param  length  length of the byte array
 * @return the checksum over the buffer bytes
 */
static inline uint16_t crc_calculate_c2000(const uint8_t* pBuffer, uint16_t length)
{
    // The only difference between this and the regular version is that this calls the C2000 specific
    // version of crc_accumulate to make sure that 16-bit "bytes" are handled correctly

    uint16_t crcTmp;
    crc_init_c2000(&crcTmp);
    while (length--) {
        crc_accumulate_c2000(*pBuffer++, &crcTmp);
    }
    return crcTmp;
}

/**
 * @brief Accumulate the X.25 CRC by adding an array of bytes
 *
 * The checksum function adds the hash of one char at a time to the
 * 16 bit checksum (uint16_t).
 *
 * The C2000 version is specific to message payloads (where the normal version works for any byte buffer),
 * but in the library this is the only way the normal version is used.  The C2000 version uses the C2000-specific
 * byte retrieval function to handle extracting bytes out of a message payload on the C2000 architecture
 *
 * @param crcAccum Pointer to CRC accumulator to accumulate message payload CRC into
 * @param payload Pointer to beginning of MAVLink message payload
 * @param length Length of message payload (in bytes)
 */
static inline void crc_accumulate_msg_payload_c2000(uint16_t *crcAccum, void* payload, uint16_t length)
{
    int crc_bytes_accumulated = 0;
    while (crc_bytes_accumulated < length) {
        crc_accumulate_c2000(mav_get_uint8_t_c2000(payload, crc_bytes_accumulated++), crcAccum);
    }
}

#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_C2000_H_ */
