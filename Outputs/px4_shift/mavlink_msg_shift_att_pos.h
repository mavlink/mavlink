#pragma once
// MESSAGE SHIFT_ATT_POS PACKING

#define MAVLINK_MSG_ID_SHIFT_ATT_POS 555

MAVPACKED(
typedef struct __mavlink_shift_att_pos_t {
 uint64_t time_usec; /*< Timestamp (micros since boot or Unix epoch)*/
 float q[4]; /*< Attitude quaternion (w, x, y, z order, zero-rotation is 1, 0, 0, 0)*/
 float x; /*< X position in meters (NED)*/
 float y; /*< Y position in meters (NED)*/
 float z; /*< Z position in meters (NED)*/
 uint8_t target_system; /*< System ID*/
}) mavlink_shift_att_pos_t;

#define MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN 37
#define MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN 37
#define MAVLINK_MSG_ID_555_LEN 37
#define MAVLINK_MSG_ID_555_MIN_LEN 37

#define MAVLINK_MSG_ID_SHIFT_ATT_POS_CRC 103
#define MAVLINK_MSG_ID_555_CRC 103

#define MAVLINK_MSG_SHIFT_ATT_POS_FIELD_Q_LEN 4

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SHIFT_ATT_POS { \
    555, \
    "SHIFT_ATT_POS", \
    6, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_shift_att_pos_t, time_usec) }, \
         { "q", NULL, MAVLINK_TYPE_FLOAT, 4, 8, offsetof(mavlink_shift_att_pos_t, q) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_shift_att_pos_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_shift_att_pos_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_shift_att_pos_t, z) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 36, offsetof(mavlink_shift_att_pos_t, target_system) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SHIFT_ATT_POS { \
    "SHIFT_ATT_POS", \
    6, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_shift_att_pos_t, time_usec) }, \
         { "q", NULL, MAVLINK_TYPE_FLOAT, 4, 8, offsetof(mavlink_shift_att_pos_t, q) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_shift_att_pos_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_shift_att_pos_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_shift_att_pos_t, z) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 36, offsetof(mavlink_shift_att_pos_t, target_system) }, \
         } \
}
#endif

/**
 * @brief Pack a shift_att_pos message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param time_usec Timestamp (micros since boot or Unix epoch)
 * @param q Attitude quaternion (w, x, y, z order, zero-rotation is 1, 0, 0, 0)
 * @param x X position in meters (NED)
 * @param y Y position in meters (NED)
 * @param z Z position in meters (NED)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_att_pos_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint64_t time_usec, const float *q, float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 24, x);
    _mav_put_float(buf, 28, y);
    _mav_put_float(buf, 32, z);
    _mav_put_uint8_t(buf, 36, target_system);
    _mav_put_float_array(buf, 8, q, 4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN);
#else
    mavlink_shift_att_pos_t packet;
    packet.time_usec = time_usec;
    packet.x = x;
    packet.y = y;
    packet.z = z;
    packet.target_system = target_system;
    mav_array_memcpy(packet.q, q, sizeof(float)*4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_ATT_POS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_CRC);
}

/**
 * @brief Pack a shift_att_pos message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param time_usec Timestamp (micros since boot or Unix epoch)
 * @param q Attitude quaternion (w, x, y, z order, zero-rotation is 1, 0, 0, 0)
 * @param x X position in meters (NED)
 * @param y Y position in meters (NED)
 * @param z Z position in meters (NED)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_att_pos_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint64_t time_usec,const float *q,float x,float y,float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 24, x);
    _mav_put_float(buf, 28, y);
    _mav_put_float(buf, 32, z);
    _mav_put_uint8_t(buf, 36, target_system);
    _mav_put_float_array(buf, 8, q, 4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN);
#else
    mavlink_shift_att_pos_t packet;
    packet.time_usec = time_usec;
    packet.x = x;
    packet.y = y;
    packet.z = z;
    packet.target_system = target_system;
    mav_array_memcpy(packet.q, q, sizeof(float)*4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_ATT_POS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_CRC);
}

/**
 * @brief Encode a shift_att_pos struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param shift_att_pos C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_att_pos_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_shift_att_pos_t* shift_att_pos)
{
    return mavlink_msg_shift_att_pos_pack(system_id, component_id, msg, shift_att_pos->target_system, shift_att_pos->time_usec, shift_att_pos->q, shift_att_pos->x, shift_att_pos->y, shift_att_pos->z);
}

/**
 * @brief Encode a shift_att_pos struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param shift_att_pos C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_att_pos_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_shift_att_pos_t* shift_att_pos)
{
    return mavlink_msg_shift_att_pos_pack_chan(system_id, component_id, chan, msg, shift_att_pos->target_system, shift_att_pos->time_usec, shift_att_pos->q, shift_att_pos->x, shift_att_pos->y, shift_att_pos->z);
}

/**
 * @brief Send a shift_att_pos message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param time_usec Timestamp (micros since boot or Unix epoch)
 * @param q Attitude quaternion (w, x, y, z order, zero-rotation is 1, 0, 0, 0)
 * @param x X position in meters (NED)
 * @param y Y position in meters (NED)
 * @param z Z position in meters (NED)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_shift_att_pos_send(mavlink_channel_t chan, uint8_t target_system, uint64_t time_usec, const float *q, float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 24, x);
    _mav_put_float(buf, 28, y);
    _mav_put_float(buf, 32, z);
    _mav_put_uint8_t(buf, 36, target_system);
    _mav_put_float_array(buf, 8, q, 4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_ATT_POS, buf, MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_CRC);
#else
    mavlink_shift_att_pos_t packet;
    packet.time_usec = time_usec;
    packet.x = x;
    packet.y = y;
    packet.z = z;
    packet.target_system = target_system;
    mav_array_memcpy(packet.q, q, sizeof(float)*4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_ATT_POS, (const char *)&packet, MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_CRC);
#endif
}

/**
 * @brief Send a shift_att_pos message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_shift_att_pos_send_struct(mavlink_channel_t chan, const mavlink_shift_att_pos_t* shift_att_pos)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_shift_att_pos_send(chan, shift_att_pos->target_system, shift_att_pos->time_usec, shift_att_pos->q, shift_att_pos->x, shift_att_pos->y, shift_att_pos->z);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_ATT_POS, (const char *)shift_att_pos, MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_CRC);
#endif
}

#if MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_shift_att_pos_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint64_t time_usec, const float *q, float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 24, x);
    _mav_put_float(buf, 28, y);
    _mav_put_float(buf, 32, z);
    _mav_put_uint8_t(buf, 36, target_system);
    _mav_put_float_array(buf, 8, q, 4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_ATT_POS, buf, MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_CRC);
#else
    mavlink_shift_att_pos_t *packet = (mavlink_shift_att_pos_t *)msgbuf;
    packet->time_usec = time_usec;
    packet->x = x;
    packet->y = y;
    packet->z = z;
    packet->target_system = target_system;
    mav_array_memcpy(packet->q, q, sizeof(float)*4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_ATT_POS, (const char *)packet, MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN, MAVLINK_MSG_ID_SHIFT_ATT_POS_CRC);
#endif
}
#endif

#endif

// MESSAGE SHIFT_ATT_POS UNPACKING


/**
 * @brief Get field target_system from shift_att_pos message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_shift_att_pos_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  36);
}

/**
 * @brief Get field time_usec from shift_att_pos message
 *
 * @return Timestamp (micros since boot or Unix epoch)
 */
static inline uint64_t mavlink_msg_shift_att_pos_get_time_usec(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field q from shift_att_pos message
 *
 * @return Attitude quaternion (w, x, y, z order, zero-rotation is 1, 0, 0, 0)
 */
static inline uint16_t mavlink_msg_shift_att_pos_get_q(const mavlink_message_t* msg, float *q)
{
    return _MAV_RETURN_float_array(msg, q, 4,  8);
}

/**
 * @brief Get field x from shift_att_pos message
 *
 * @return X position in meters (NED)
 */
static inline float mavlink_msg_shift_att_pos_get_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field y from shift_att_pos message
 *
 * @return Y position in meters (NED)
 */
static inline float mavlink_msg_shift_att_pos_get_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field z from shift_att_pos message
 *
 * @return Z position in meters (NED)
 */
static inline float mavlink_msg_shift_att_pos_get_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Decode a shift_att_pos message into a struct
 *
 * @param msg The message to decode
 * @param shift_att_pos C-struct to decode the message contents into
 */
static inline void mavlink_msg_shift_att_pos_decode(const mavlink_message_t* msg, mavlink_shift_att_pos_t* shift_att_pos)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    shift_att_pos->time_usec = mavlink_msg_shift_att_pos_get_time_usec(msg);
    mavlink_msg_shift_att_pos_get_q(msg, shift_att_pos->q);
    shift_att_pos->x = mavlink_msg_shift_att_pos_get_x(msg);
    shift_att_pos->y = mavlink_msg_shift_att_pos_get_y(msg);
    shift_att_pos->z = mavlink_msg_shift_att_pos_get_z(msg);
    shift_att_pos->target_system = mavlink_msg_shift_att_pos_get_target_system(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN? msg->len : MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN;
        memset(shift_att_pos, 0, MAVLINK_MSG_ID_SHIFT_ATT_POS_LEN);
    memcpy(shift_att_pos, _MAV_PAYLOAD(msg), len);
#endif
}
