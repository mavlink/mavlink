#pragma once
// MESSAGE MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG PACKING

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG 503

MAVPACKED(
typedef struct __mavlink_mav_shift_radiodog_ranging_request_data_msg_t {
 uint64_t t1; /*< Server internal receive(1) timestamp (Set on reception of the RANGING_REQUEST_MSG)*/
 uint64_t t2; /*< Server internal transmit timestamp (Set on transmision completed of the RANGING_REQUEST_ACK)*/
 uint64_t t5; /*< Server internal receive(2) timestamp (Set on reception of the RANGING_REQUEST_ACK)*/
 uint32_t moduleId; /*< RadioDog Module ID (Sending module)*/
 uint16_t radioId; /*< RadioDog Radio ID (Sending module)*/
 uint8_t moduleType; /*< Module Type (Sending module)*/
}) mavlink_mav_shift_radiodog_ranging_request_data_msg_t;

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN 31
#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN 31
#define MAVLINK_MSG_ID_503_LEN 31
#define MAVLINK_MSG_ID_503_MIN_LEN 31

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_CRC 94
#define MAVLINK_MSG_ID_503_CRC 94



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG { \
    503, \
    "MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG", \
    6, \
    {  { "t1", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, t1) }, \
         { "t2", NULL, MAVLINK_TYPE_UINT64_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, t2) }, \
         { "t5", NULL, MAVLINK_TYPE_UINT64_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, t5) }, \
         { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 24, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, moduleId) }, \
         { "radioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 28, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, radioId) }, \
         { "moduleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 30, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, moduleType) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG { \
    "MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG", \
    6, \
    {  { "t1", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, t1) }, \
         { "t2", NULL, MAVLINK_TYPE_UINT64_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, t2) }, \
         { "t5", NULL, MAVLINK_TYPE_UINT64_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, t5) }, \
         { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 24, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, moduleId) }, \
         { "radioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 28, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, radioId) }, \
         { "moduleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 30, offsetof(mavlink_mav_shift_radiodog_ranging_request_data_msg_t, moduleType) }, \
         } \
}
#endif

/**
 * @brief Pack a mav_shift_radiodog_ranging_request_data_msg message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param radioId RadioDog Radio ID (Sending module)
 * @param moduleType Module Type (Sending module)
 * @param t1 Server internal receive(1) timestamp (Set on reception of the RANGING_REQUEST_MSG)
 * @param t2 Server internal transmit timestamp (Set on transmision completed of the RANGING_REQUEST_ACK)
 * @param t5 Server internal receive(2) timestamp (Set on reception of the RANGING_REQUEST_ACK)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t moduleId, uint16_t radioId, uint8_t moduleType, uint64_t t1, uint64_t t2, uint64_t t5)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN];
    _mav_put_uint64_t(buf, 0, t1);
    _mav_put_uint64_t(buf, 8, t2);
    _mav_put_uint64_t(buf, 16, t5);
    _mav_put_uint32_t(buf, 24, moduleId);
    _mav_put_uint16_t(buf, 28, radioId);
    _mav_put_uint8_t(buf, 30, moduleType);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN);
#else
    mavlink_mav_shift_radiodog_ranging_request_data_msg_t packet;
    packet.t1 = t1;
    packet.t2 = t2;
    packet.t5 = t5;
    packet.moduleId = moduleId;
    packet.radioId = radioId;
    packet.moduleType = moduleType;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_CRC);
}

/**
 * @brief Pack a mav_shift_radiodog_ranging_request_data_msg message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param moduleId RadioDog Module ID (Sending module)
 * @param radioId RadioDog Radio ID (Sending module)
 * @param moduleType Module Type (Sending module)
 * @param t1 Server internal receive(1) timestamp (Set on reception of the RANGING_REQUEST_MSG)
 * @param t2 Server internal transmit timestamp (Set on transmision completed of the RANGING_REQUEST_ACK)
 * @param t5 Server internal receive(2) timestamp (Set on reception of the RANGING_REQUEST_ACK)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t moduleId,uint16_t radioId,uint8_t moduleType,uint64_t t1,uint64_t t2,uint64_t t5)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN];
    _mav_put_uint64_t(buf, 0, t1);
    _mav_put_uint64_t(buf, 8, t2);
    _mav_put_uint64_t(buf, 16, t5);
    _mav_put_uint32_t(buf, 24, moduleId);
    _mav_put_uint16_t(buf, 28, radioId);
    _mav_put_uint8_t(buf, 30, moduleType);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN);
#else
    mavlink_mav_shift_radiodog_ranging_request_data_msg_t packet;
    packet.t1 = t1;
    packet.t2 = t2;
    packet.t5 = t5;
    packet.moduleId = moduleId;
    packet.radioId = radioId;
    packet.moduleType = moduleType;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_CRC);
}

/**
 * @brief Encode a mav_shift_radiodog_ranging_request_data_msg struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_ranging_request_data_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_ranging_request_data_msg_t* mav_shift_radiodog_ranging_request_data_msg)
{
    return mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_pack(system_id, component_id, msg, mav_shift_radiodog_ranging_request_data_msg->moduleId, mav_shift_radiodog_ranging_request_data_msg->radioId, mav_shift_radiodog_ranging_request_data_msg->moduleType, mav_shift_radiodog_ranging_request_data_msg->t1, mav_shift_radiodog_ranging_request_data_msg->t2, mav_shift_radiodog_ranging_request_data_msg->t5);
}

/**
 * @brief Encode a mav_shift_radiodog_ranging_request_data_msg struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_ranging_request_data_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_ranging_request_data_msg_t* mav_shift_radiodog_ranging_request_data_msg)
{
    return mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_pack_chan(system_id, component_id, chan, msg, mav_shift_radiodog_ranging_request_data_msg->moduleId, mav_shift_radiodog_ranging_request_data_msg->radioId, mav_shift_radiodog_ranging_request_data_msg->moduleType, mav_shift_radiodog_ranging_request_data_msg->t1, mav_shift_radiodog_ranging_request_data_msg->t2, mav_shift_radiodog_ranging_request_data_msg->t5);
}

/**
 * @brief Send a mav_shift_radiodog_ranging_request_data_msg message
 * @param chan MAVLink channel to send the message
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param radioId RadioDog Radio ID (Sending module)
 * @param moduleType Module Type (Sending module)
 * @param t1 Server internal receive(1) timestamp (Set on reception of the RANGING_REQUEST_MSG)
 * @param t2 Server internal transmit timestamp (Set on transmision completed of the RANGING_REQUEST_ACK)
 * @param t5 Server internal receive(2) timestamp (Set on reception of the RANGING_REQUEST_ACK)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_send(mavlink_channel_t chan, uint32_t moduleId, uint16_t radioId, uint8_t moduleType, uint64_t t1, uint64_t t2, uint64_t t5)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN];
    _mav_put_uint64_t(buf, 0, t1);
    _mav_put_uint64_t(buf, 8, t2);
    _mav_put_uint64_t(buf, 16, t5);
    _mav_put_uint32_t(buf, 24, moduleId);
    _mav_put_uint16_t(buf, 28, radioId);
    _mav_put_uint8_t(buf, 30, moduleType);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_CRC);
#else
    mavlink_mav_shift_radiodog_ranging_request_data_msg_t packet;
    packet.t1 = t1;
    packet.t2 = t2;
    packet.t5 = t5;
    packet.moduleId = moduleId;
    packet.radioId = radioId;
    packet.moduleType = moduleType;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG, (const char *)&packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_CRC);
#endif
}

/**
 * @brief Send a mav_shift_radiodog_ranging_request_data_msg message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_send_struct(mavlink_channel_t chan, const mavlink_mav_shift_radiodog_ranging_request_data_msg_t* mav_shift_radiodog_ranging_request_data_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_send(chan, mav_shift_radiodog_ranging_request_data_msg->moduleId, mav_shift_radiodog_ranging_request_data_msg->radioId, mav_shift_radiodog_ranging_request_data_msg->moduleType, mav_shift_radiodog_ranging_request_data_msg->t1, mav_shift_radiodog_ranging_request_data_msg->t2, mav_shift_radiodog_ranging_request_data_msg->t5);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG, (const char *)mav_shift_radiodog_ranging_request_data_msg, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_CRC);
#endif
}

#if MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t moduleId, uint16_t radioId, uint8_t moduleType, uint64_t t1, uint64_t t2, uint64_t t5)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, t1);
    _mav_put_uint64_t(buf, 8, t2);
    _mav_put_uint64_t(buf, 16, t5);
    _mav_put_uint32_t(buf, 24, moduleId);
    _mav_put_uint16_t(buf, 28, radioId);
    _mav_put_uint8_t(buf, 30, moduleType);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_CRC);
#else
    mavlink_mav_shift_radiodog_ranging_request_data_msg_t *packet = (mavlink_mav_shift_radiodog_ranging_request_data_msg_t *)msgbuf;
    packet->t1 = t1;
    packet->t2 = t2;
    packet->t5 = t5;
    packet->moduleId = moduleId;
    packet->radioId = radioId;
    packet->moduleType = moduleType;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG, (const char *)packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_CRC);
#endif
}
#endif

#endif

// MESSAGE MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG UNPACKING


/**
 * @brief Get field moduleId from mav_shift_radiodog_ranging_request_data_msg message
 *
 * @return RadioDog Module ID (Sending module)
 */
static inline uint32_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_moduleId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  24);
}

/**
 * @brief Get field radioId from mav_shift_radiodog_ranging_request_data_msg message
 *
 * @return RadioDog Radio ID (Sending module)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_radioId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  28);
}

/**
 * @brief Get field moduleType from mav_shift_radiodog_ranging_request_data_msg message
 *
 * @return Module Type (Sending module)
 */
static inline uint8_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_moduleType(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  30);
}

/**
 * @brief Get field t1 from mav_shift_radiodog_ranging_request_data_msg message
 *
 * @return Server internal receive(1) timestamp (Set on reception of the RANGING_REQUEST_MSG)
 */
static inline uint64_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_t1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field t2 from mav_shift_radiodog_ranging_request_data_msg message
 *
 * @return Server internal transmit timestamp (Set on transmision completed of the RANGING_REQUEST_ACK)
 */
static inline uint64_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_t2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  8);
}

/**
 * @brief Get field t5 from mav_shift_radiodog_ranging_request_data_msg message
 *
 * @return Server internal receive(2) timestamp (Set on reception of the RANGING_REQUEST_ACK)
 */
static inline uint64_t mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_t5(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  16);
}

/**
 * @brief Decode a mav_shift_radiodog_ranging_request_data_msg message into a struct
 *
 * @param msg The message to decode
 * @param mav_shift_radiodog_ranging_request_data_msg C-struct to decode the message contents into
 */
static inline void mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_decode(const mavlink_message_t* msg, mavlink_mav_shift_radiodog_ranging_request_data_msg_t* mav_shift_radiodog_ranging_request_data_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mav_shift_radiodog_ranging_request_data_msg->t1 = mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_t1(msg);
    mav_shift_radiodog_ranging_request_data_msg->t2 = mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_t2(msg);
    mav_shift_radiodog_ranging_request_data_msg->t5 = mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_t5(msg);
    mav_shift_radiodog_ranging_request_data_msg->moduleId = mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_moduleId(msg);
    mav_shift_radiodog_ranging_request_data_msg->radioId = mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_radioId(msg);
    mav_shift_radiodog_ranging_request_data_msg->moduleType = mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_get_moduleType(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN? msg->len : MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN;
        memset(mav_shift_radiodog_ranging_request_data_msg, 0, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_LEN);
    memcpy(mav_shift_radiodog_ranging_request_data_msg, _MAV_PAYLOAD(msg), len);
#endif
}
