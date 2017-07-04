#pragma once
// MESSAGE SHIFT_NAVIGATOR_SIM_POINT PACKING

#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT 561

MAVPACKED(
typedef struct __mavlink_shift_navigator_sim_point_t {
 uint64_t time_usec; /*< Timestamp from the FlightController*/
 float positionX; /*< positionX value*/
 float positionY; /*< positionY value*/
 float positionZ; /*< positionZ value*/
 float positionRoll; /*< positionRoll value*/
 float positionPitch; /*< positionPitch value*/
 float positionYaw; /*< positionYaw value*/
}) mavlink_shift_navigator_sim_point_t;

#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN 32
#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN 32
#define MAVLINK_MSG_ID_561_LEN 32
#define MAVLINK_MSG_ID_561_MIN_LEN 32

#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_CRC 202
#define MAVLINK_MSG_ID_561_CRC 202



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SHIFT_NAVIGATOR_SIM_POINT { \
    561, \
    "SHIFT_NAVIGATOR_SIM_POINT", \
    7, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_shift_navigator_sim_point_t, time_usec) }, \
         { "positionX", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_shift_navigator_sim_point_t, positionX) }, \
         { "positionY", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_shift_navigator_sim_point_t, positionY) }, \
         { "positionZ", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_shift_navigator_sim_point_t, positionZ) }, \
         { "positionRoll", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_shift_navigator_sim_point_t, positionRoll) }, \
         { "positionPitch", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_shift_navigator_sim_point_t, positionPitch) }, \
         { "positionYaw", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_shift_navigator_sim_point_t, positionYaw) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SHIFT_NAVIGATOR_SIM_POINT { \
    "SHIFT_NAVIGATOR_SIM_POINT", \
    7, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_shift_navigator_sim_point_t, time_usec) }, \
         { "positionX", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_shift_navigator_sim_point_t, positionX) }, \
         { "positionY", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_shift_navigator_sim_point_t, positionY) }, \
         { "positionZ", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_shift_navigator_sim_point_t, positionZ) }, \
         { "positionRoll", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_shift_navigator_sim_point_t, positionRoll) }, \
         { "positionPitch", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_shift_navigator_sim_point_t, positionPitch) }, \
         { "positionYaw", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_shift_navigator_sim_point_t, positionYaw) }, \
         } \
}
#endif

/**
 * @brief Pack a shift_navigator_sim_point message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_usec Timestamp from the FlightController
 * @param positionX positionX value
 * @param positionY positionY value
 * @param positionZ positionZ value
 * @param positionRoll positionRoll value
 * @param positionPitch positionPitch value
 * @param positionYaw positionYaw value
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_navigator_sim_point_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint64_t time_usec, float positionX, float positionY, float positionZ, float positionRoll, float positionPitch, float positionYaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 8, positionX);
    _mav_put_float(buf, 12, positionY);
    _mav_put_float(buf, 16, positionZ);
    _mav_put_float(buf, 20, positionRoll);
    _mav_put_float(buf, 24, positionPitch);
    _mav_put_float(buf, 28, positionYaw);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN);
#else
    mavlink_shift_navigator_sim_point_t packet;
    packet.time_usec = time_usec;
    packet.positionX = positionX;
    packet.positionY = positionY;
    packet.positionZ = positionZ;
    packet.positionRoll = positionRoll;
    packet.positionPitch = positionPitch;
    packet.positionYaw = positionYaw;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_CRC);
}

/**
 * @brief Pack a shift_navigator_sim_point message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_usec Timestamp from the FlightController
 * @param positionX positionX value
 * @param positionY positionY value
 * @param positionZ positionZ value
 * @param positionRoll positionRoll value
 * @param positionPitch positionPitch value
 * @param positionYaw positionYaw value
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_navigator_sim_point_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint64_t time_usec,float positionX,float positionY,float positionZ,float positionRoll,float positionPitch,float positionYaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 8, positionX);
    _mav_put_float(buf, 12, positionY);
    _mav_put_float(buf, 16, positionZ);
    _mav_put_float(buf, 20, positionRoll);
    _mav_put_float(buf, 24, positionPitch);
    _mav_put_float(buf, 28, positionYaw);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN);
#else
    mavlink_shift_navigator_sim_point_t packet;
    packet.time_usec = time_usec;
    packet.positionX = positionX;
    packet.positionY = positionY;
    packet.positionZ = positionZ;
    packet.positionRoll = positionRoll;
    packet.positionPitch = positionPitch;
    packet.positionYaw = positionYaw;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_CRC);
}

/**
 * @brief Encode a shift_navigator_sim_point struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param shift_navigator_sim_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_navigator_sim_point_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_shift_navigator_sim_point_t* shift_navigator_sim_point)
{
    return mavlink_msg_shift_navigator_sim_point_pack(system_id, component_id, msg, shift_navigator_sim_point->time_usec, shift_navigator_sim_point->positionX, shift_navigator_sim_point->positionY, shift_navigator_sim_point->positionZ, shift_navigator_sim_point->positionRoll, shift_navigator_sim_point->positionPitch, shift_navigator_sim_point->positionYaw);
}

/**
 * @brief Encode a shift_navigator_sim_point struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param shift_navigator_sim_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_navigator_sim_point_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_shift_navigator_sim_point_t* shift_navigator_sim_point)
{
    return mavlink_msg_shift_navigator_sim_point_pack_chan(system_id, component_id, chan, msg, shift_navigator_sim_point->time_usec, shift_navigator_sim_point->positionX, shift_navigator_sim_point->positionY, shift_navigator_sim_point->positionZ, shift_navigator_sim_point->positionRoll, shift_navigator_sim_point->positionPitch, shift_navigator_sim_point->positionYaw);
}

/**
 * @brief Send a shift_navigator_sim_point message
 * @param chan MAVLink channel to send the message
 *
 * @param time_usec Timestamp from the FlightController
 * @param positionX positionX value
 * @param positionY positionY value
 * @param positionZ positionZ value
 * @param positionRoll positionRoll value
 * @param positionPitch positionPitch value
 * @param positionYaw positionYaw value
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_shift_navigator_sim_point_send(mavlink_channel_t chan, uint64_t time_usec, float positionX, float positionY, float positionZ, float positionRoll, float positionPitch, float positionYaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 8, positionX);
    _mav_put_float(buf, 12, positionY);
    _mav_put_float(buf, 16, positionZ);
    _mav_put_float(buf, 20, positionRoll);
    _mav_put_float(buf, 24, positionPitch);
    _mav_put_float(buf, 28, positionYaw);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT, buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_CRC);
#else
    mavlink_shift_navigator_sim_point_t packet;
    packet.time_usec = time_usec;
    packet.positionX = positionX;
    packet.positionY = positionY;
    packet.positionZ = positionZ;
    packet.positionRoll = positionRoll;
    packet.positionPitch = positionPitch;
    packet.positionYaw = positionYaw;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT, (const char *)&packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_CRC);
#endif
}

/**
 * @brief Send a shift_navigator_sim_point message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_shift_navigator_sim_point_send_struct(mavlink_channel_t chan, const mavlink_shift_navigator_sim_point_t* shift_navigator_sim_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_shift_navigator_sim_point_send(chan, shift_navigator_sim_point->time_usec, shift_navigator_sim_point->positionX, shift_navigator_sim_point->positionY, shift_navigator_sim_point->positionZ, shift_navigator_sim_point->positionRoll, shift_navigator_sim_point->positionPitch, shift_navigator_sim_point->positionYaw);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT, (const char *)shift_navigator_sim_point, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_CRC);
#endif
}

#if MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_shift_navigator_sim_point_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint64_t time_usec, float positionX, float positionY, float positionZ, float positionRoll, float positionPitch, float positionYaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 8, positionX);
    _mav_put_float(buf, 12, positionY);
    _mav_put_float(buf, 16, positionZ);
    _mav_put_float(buf, 20, positionRoll);
    _mav_put_float(buf, 24, positionPitch);
    _mav_put_float(buf, 28, positionYaw);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT, buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_CRC);
#else
    mavlink_shift_navigator_sim_point_t *packet = (mavlink_shift_navigator_sim_point_t *)msgbuf;
    packet->time_usec = time_usec;
    packet->positionX = positionX;
    packet->positionY = positionY;
    packet->positionZ = positionZ;
    packet->positionRoll = positionRoll;
    packet->positionPitch = positionPitch;
    packet->positionYaw = positionYaw;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT, (const char *)packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_CRC);
#endif
}
#endif

#endif

// MESSAGE SHIFT_NAVIGATOR_SIM_POINT UNPACKING


/**
 * @brief Get field time_usec from shift_navigator_sim_point message
 *
 * @return Timestamp from the FlightController
 */
static inline uint64_t mavlink_msg_shift_navigator_sim_point_get_time_usec(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field positionX from shift_navigator_sim_point message
 *
 * @return positionX value
 */
static inline float mavlink_msg_shift_navigator_sim_point_get_positionX(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field positionY from shift_navigator_sim_point message
 *
 * @return positionY value
 */
static inline float mavlink_msg_shift_navigator_sim_point_get_positionY(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field positionZ from shift_navigator_sim_point message
 *
 * @return positionZ value
 */
static inline float mavlink_msg_shift_navigator_sim_point_get_positionZ(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field positionRoll from shift_navigator_sim_point message
 *
 * @return positionRoll value
 */
static inline float mavlink_msg_shift_navigator_sim_point_get_positionRoll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field positionPitch from shift_navigator_sim_point message
 *
 * @return positionPitch value
 */
static inline float mavlink_msg_shift_navigator_sim_point_get_positionPitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field positionYaw from shift_navigator_sim_point message
 *
 * @return positionYaw value
 */
static inline float mavlink_msg_shift_navigator_sim_point_get_positionYaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Decode a shift_navigator_sim_point message into a struct
 *
 * @param msg The message to decode
 * @param shift_navigator_sim_point C-struct to decode the message contents into
 */
static inline void mavlink_msg_shift_navigator_sim_point_decode(const mavlink_message_t* msg, mavlink_shift_navigator_sim_point_t* shift_navigator_sim_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    shift_navigator_sim_point->time_usec = mavlink_msg_shift_navigator_sim_point_get_time_usec(msg);
    shift_navigator_sim_point->positionX = mavlink_msg_shift_navigator_sim_point_get_positionX(msg);
    shift_navigator_sim_point->positionY = mavlink_msg_shift_navigator_sim_point_get_positionY(msg);
    shift_navigator_sim_point->positionZ = mavlink_msg_shift_navigator_sim_point_get_positionZ(msg);
    shift_navigator_sim_point->positionRoll = mavlink_msg_shift_navigator_sim_point_get_positionRoll(msg);
    shift_navigator_sim_point->positionPitch = mavlink_msg_shift_navigator_sim_point_get_positionPitch(msg);
    shift_navigator_sim_point->positionYaw = mavlink_msg_shift_navigator_sim_point_get_positionYaw(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN? msg->len : MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN;
        memset(shift_navigator_sim_point, 0, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_LEN);
    memcpy(shift_navigator_sim_point, _MAV_PAYLOAD(msg), len);
#endif
}
