#pragma once
// MESSAGE MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG PACKING

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG 501

MAVPACKED(
typedef struct __mavlink_mav_shift_radiodog_ranging_request_msg_t {
 uint32_t moduleId; /*< RadioDog Module ID (Sending module)*/
 uint16_t radioId; /*< RadioDog Radio ID (Sending module)*/
 uint8_t moduleType; /*< Module Type (Sending module)*/
}) mavlink_mav_shift_radiodog_ranging_request_msg_t;

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN 7
#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN 7
#define MAVLINK_MSG_ID_501_LEN 7
#define MAVLINK_MSG_ID_501_MIN_LEN 7

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_CRC 81
#define MAVLINK_MSG_ID_501_CRC 81



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG { \
    501, \
    "MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG", \
    3, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_ranging_request_msg_t, moduleId) }, \
         { "radioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_ranging_request_msg_t, radioId) }, \
         { "moduleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_mav_shift_radiodog_ranging_request_msg_t, moduleType) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG { \
    "MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG", \
    3, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_ranging_request_msg_t, moduleId) }, \
         { "radioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_ranging_request_msg_t, radioId) }, \
         { "moduleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_mav_shift_radiodog_ranging_request_msg_t, moduleType) }, \
         } \
}
#endif

/**
 * @brief Pack a mav_shift_radiodog_ranging_request_msg message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param radioId RadioDog Radio ID (Sending module)
 * @param moduleType Module Type (Sending module)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_msg_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t moduleId, uint16_t radioId, uint8_t moduleType)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint16_t(buf, 4, radioId);
    _mav_put_uint8_t(buf, 6, moduleType);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN);
#else
    mavlink_mav_shift_radiodog_ranging_request_msg_t packet;
    packet.moduleId = moduleId;
    packet.radioId = radioId;
    packet.moduleType = moduleType;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_CRC);
}

/**
 * @brief Pack a mav_shift_radiodog_ranging_request_msg message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param moduleId RadioDog Module ID (Sending module)
 * @param radioId RadioDog Radio ID (Sending module)
 * @param moduleType Module Type (Sending module)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_msg_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t moduleId,uint16_t radioId,uint8_t moduleType)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint16_t(buf, 4, radioId);
    _mav_put_uint8_t(buf, 6, moduleType);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN);
#else
    mavlink_mav_shift_radiodog_ranging_request_msg_t packet;
    packet.moduleId = moduleId;
    packet.radioId = radioId;
    packet.moduleType = moduleType;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_CRC);
}

/**
 * @brief Encode a mav_shift_radiodog_ranging_request_msg struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_ranging_request_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_msg_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_ranging_request_msg_t* mav_shift_radiodog_ranging_request_msg)
{
    return mavlink_msg_mav_shift_radiodog_ranging_request_msg_pack(system_id, component_id, msg, mav_shift_radiodog_ranging_request_msg->moduleId, mav_shift_radiodog_ranging_request_msg->radioId, mav_shift_radiodog_ranging_request_msg->moduleType);
}

/**
 * @brief Encode a mav_shift_radiodog_ranging_request_msg struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_ranging_request_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_msg_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_ranging_request_msg_t* mav_shift_radiodog_ranging_request_msg)
{
    return mavlink_msg_mav_shift_radiodog_ranging_request_msg_pack_chan(system_id, component_id, chan, msg, mav_shift_radiodog_ranging_request_msg->moduleId, mav_shift_radiodog_ranging_request_msg->radioId, mav_shift_radiodog_ranging_request_msg->moduleType);
}

/**
 * @brief Send a mav_shift_radiodog_ranging_request_msg message
 * @param chan MAVLink channel to send the message
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param radioId RadioDog Radio ID (Sending module)
 * @param moduleType Module Type (Sending module)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mav_shift_radiodog_ranging_request_msg_send(mavlink_channel_t chan, uint32_t moduleId, uint16_t radioId, uint8_t moduleType)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint16_t(buf, 4, radioId);
    _mav_put_uint8_t(buf, 6, moduleType);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_CRC);
#else
    mavlink_mav_shift_radiodog_ranging_request_msg_t packet;
    packet.moduleId = moduleId;
    packet.radioId = radioId;
    packet.moduleType = moduleType;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG, (const char *)&packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_CRC);
#endif
}

/**
 * @brief Send a mav_shift_radiodog_ranging_request_msg message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_mav_shift_radiodog_ranging_request_msg_send_struct(mavlink_channel_t chan, const mavlink_mav_shift_radiodog_ranging_request_msg_t* mav_shift_radiodog_ranging_request_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_send(chan, mav_shift_radiodog_ranging_request_msg->moduleId, mav_shift_radiodog_ranging_request_msg->radioId, mav_shift_radiodog_ranging_request_msg->moduleType);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG, (const char *)mav_shift_radiodog_ranging_request_msg, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_CRC);
#endif
}

#if MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mav_shift_radiodog_ranging_request_msg_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t moduleId, uint16_t radioId, uint8_t moduleType)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint16_t(buf, 4, radioId);
    _mav_put_uint8_t(buf, 6, moduleType);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_CRC);
#else
    mavlink_mav_shift_radiodog_ranging_request_msg_t *packet = (mavlink_mav_shift_radiodog_ranging_request_msg_t *)msgbuf;
    packet->moduleId = moduleId;
    packet->radioId = radioId;
    packet->moduleType = moduleType;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG, (const char *)packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_CRC);
#endif
}
#endif

#endif

// MESSAGE MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG UNPACKING


/**
 * @brief Get field moduleId from mav_shift_radiodog_ranging_request_msg message
 *
 * @return RadioDog Module ID (Sending module)
 */
static inline uint32_t mavlink_msg_mav_shift_radiodog_ranging_request_msg_get_moduleId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field radioId from mav_shift_radiodog_ranging_request_msg message
 *
 * @return RadioDog Radio ID (Sending module)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_msg_get_radioId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  4);
}

/**
 * @brief Get field moduleType from mav_shift_radiodog_ranging_request_msg message
 *
 * @return Module Type (Sending module)
 */
static inline uint8_t mavlink_msg_mav_shift_radiodog_ranging_request_msg_get_moduleType(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Decode a mav_shift_radiodog_ranging_request_msg message into a struct
 *
 * @param msg The message to decode
 * @param mav_shift_radiodog_ranging_request_msg C-struct to decode the message contents into
 */
static inline void mavlink_msg_mav_shift_radiodog_ranging_request_msg_decode(const mavlink_message_t* msg, mavlink_mav_shift_radiodog_ranging_request_msg_t* mav_shift_radiodog_ranging_request_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mav_shift_radiodog_ranging_request_msg->moduleId = mavlink_msg_mav_shift_radiodog_ranging_request_msg_get_moduleId(msg);
    mav_shift_radiodog_ranging_request_msg->radioId = mavlink_msg_mav_shift_radiodog_ranging_request_msg_get_radioId(msg);
    mav_shift_radiodog_ranging_request_msg->moduleType = mavlink_msg_mav_shift_radiodog_ranging_request_msg_get_moduleType(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN? msg->len : MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN;
        memset(mav_shift_radiodog_ranging_request_msg, 0, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_LEN);
    memcpy(mav_shift_radiodog_ranging_request_msg, _MAV_PAYLOAD(msg), len);
#endif
}
