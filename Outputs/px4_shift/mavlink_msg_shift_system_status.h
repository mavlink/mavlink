#pragma once
// MESSAGE SHIFT_SYSTEM_STATUS PACKING

#define MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS 565

MAVPACKED(
typedef struct __mavlink_shift_system_status_t {
 float statusValue1; /*< This value represents what the mask is indicating*/
 float statusValue2; /*< This value represents what the mask is indicating*/
 uint8_t statusValueIndicators; /*< 128 =to= 128*/
 uint8_t objectModeObjectStatus; /*< Current mode and status of the object uint_4 and uint_4*/
 uint8_t navigatorMode; /*< Current mode of the navigator*/
 uint8_t navigatorSubMode; /*< Current sub mode of the navigator*/
}) mavlink_shift_system_status_t;

#define MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN 12
#define MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN 12
#define MAVLINK_MSG_ID_565_LEN 12
#define MAVLINK_MSG_ID_565_MIN_LEN 12

#define MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC 66
#define MAVLINK_MSG_ID_565_CRC 66



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SHIFT_SYSTEM_STATUS { \
    565, \
    "SHIFT_SYSTEM_STATUS", \
    6, \
    {  { "statusValue1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_shift_system_status_t, statusValue1) }, \
         { "statusValue2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_shift_system_status_t, statusValue2) }, \
         { "statusValueIndicators", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_shift_system_status_t, statusValueIndicators) }, \
         { "objectModeObjectStatus", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_shift_system_status_t, objectModeObjectStatus) }, \
         { "navigatorMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_shift_system_status_t, navigatorMode) }, \
         { "navigatorSubMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_shift_system_status_t, navigatorSubMode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SHIFT_SYSTEM_STATUS { \
    "SHIFT_SYSTEM_STATUS", \
    6, \
    {  { "statusValue1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_shift_system_status_t, statusValue1) }, \
         { "statusValue2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_shift_system_status_t, statusValue2) }, \
         { "statusValueIndicators", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_shift_system_status_t, statusValueIndicators) }, \
         { "objectModeObjectStatus", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_shift_system_status_t, objectModeObjectStatus) }, \
         { "navigatorMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_shift_system_status_t, navigatorMode) }, \
         { "navigatorSubMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_shift_system_status_t, navigatorSubMode) }, \
         } \
}
#endif

/**
 * @brief Pack a shift_system_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param statusValueIndicators 128 =to= 128
 * @param statusValue1 This value represents what the mask is indicating
 * @param statusValue2 This value represents what the mask is indicating
 * @param objectModeObjectStatus Current mode and status of the object uint_4 and uint_4
 * @param navigatorMode Current mode of the navigator
 * @param navigatorSubMode Current sub mode of the navigator
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_system_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t statusValueIndicators, float statusValue1, float statusValue2, uint8_t objectModeObjectStatus, uint8_t navigatorMode, uint8_t navigatorSubMode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN];
    _mav_put_float(buf, 0, statusValue1);
    _mav_put_float(buf, 4, statusValue2);
    _mav_put_uint8_t(buf, 8, statusValueIndicators);
    _mav_put_uint8_t(buf, 9, objectModeObjectStatus);
    _mav_put_uint8_t(buf, 10, navigatorMode);
    _mav_put_uint8_t(buf, 11, navigatorSubMode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN);
#else
    mavlink_shift_system_status_t packet;
    packet.statusValue1 = statusValue1;
    packet.statusValue2 = statusValue2;
    packet.statusValueIndicators = statusValueIndicators;
    packet.objectModeObjectStatus = objectModeObjectStatus;
    packet.navigatorMode = navigatorMode;
    packet.navigatorSubMode = navigatorSubMode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
}

/**
 * @brief Pack a shift_system_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param statusValueIndicators 128 =to= 128
 * @param statusValue1 This value represents what the mask is indicating
 * @param statusValue2 This value represents what the mask is indicating
 * @param objectModeObjectStatus Current mode and status of the object uint_4 and uint_4
 * @param navigatorMode Current mode of the navigator
 * @param navigatorSubMode Current sub mode of the navigator
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_system_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t statusValueIndicators,float statusValue1,float statusValue2,uint8_t objectModeObjectStatus,uint8_t navigatorMode,uint8_t navigatorSubMode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN];
    _mav_put_float(buf, 0, statusValue1);
    _mav_put_float(buf, 4, statusValue2);
    _mav_put_uint8_t(buf, 8, statusValueIndicators);
    _mav_put_uint8_t(buf, 9, objectModeObjectStatus);
    _mav_put_uint8_t(buf, 10, navigatorMode);
    _mav_put_uint8_t(buf, 11, navigatorSubMode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN);
#else
    mavlink_shift_system_status_t packet;
    packet.statusValue1 = statusValue1;
    packet.statusValue2 = statusValue2;
    packet.statusValueIndicators = statusValueIndicators;
    packet.objectModeObjectStatus = objectModeObjectStatus;
    packet.navigatorMode = navigatorMode;
    packet.navigatorSubMode = navigatorSubMode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
}

/**
 * @brief Encode a shift_system_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param shift_system_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_system_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_shift_system_status_t* shift_system_status)
{
    return mavlink_msg_shift_system_status_pack(system_id, component_id, msg, shift_system_status->statusValueIndicators, shift_system_status->statusValue1, shift_system_status->statusValue2, shift_system_status->objectModeObjectStatus, shift_system_status->navigatorMode, shift_system_status->navigatorSubMode);
}

/**
 * @brief Encode a shift_system_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param shift_system_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_system_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_shift_system_status_t* shift_system_status)
{
    return mavlink_msg_shift_system_status_pack_chan(system_id, component_id, chan, msg, shift_system_status->statusValueIndicators, shift_system_status->statusValue1, shift_system_status->statusValue2, shift_system_status->objectModeObjectStatus, shift_system_status->navigatorMode, shift_system_status->navigatorSubMode);
}

/**
 * @brief Send a shift_system_status message
 * @param chan MAVLink channel to send the message
 *
 * @param statusValueIndicators 128 =to= 128
 * @param statusValue1 This value represents what the mask is indicating
 * @param statusValue2 This value represents what the mask is indicating
 * @param objectModeObjectStatus Current mode and status of the object uint_4 and uint_4
 * @param navigatorMode Current mode of the navigator
 * @param navigatorSubMode Current sub mode of the navigator
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_shift_system_status_send(mavlink_channel_t chan, uint8_t statusValueIndicators, float statusValue1, float statusValue2, uint8_t objectModeObjectStatus, uint8_t navigatorMode, uint8_t navigatorSubMode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN];
    _mav_put_float(buf, 0, statusValue1);
    _mav_put_float(buf, 4, statusValue2);
    _mav_put_uint8_t(buf, 8, statusValueIndicators);
    _mav_put_uint8_t(buf, 9, objectModeObjectStatus);
    _mav_put_uint8_t(buf, 10, navigatorMode);
    _mav_put_uint8_t(buf, 11, navigatorSubMode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS, buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
#else
    mavlink_shift_system_status_t packet;
    packet.statusValue1 = statusValue1;
    packet.statusValue2 = statusValue2;
    packet.statusValueIndicators = statusValueIndicators;
    packet.objectModeObjectStatus = objectModeObjectStatus;
    packet.navigatorMode = navigatorMode;
    packet.navigatorSubMode = navigatorSubMode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS, (const char *)&packet, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
#endif
}

/**
 * @brief Send a shift_system_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_shift_system_status_send_struct(mavlink_channel_t chan, const mavlink_shift_system_status_t* shift_system_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_shift_system_status_send(chan, shift_system_status->statusValueIndicators, shift_system_status->statusValue1, shift_system_status->statusValue2, shift_system_status->objectModeObjectStatus, shift_system_status->navigatorMode, shift_system_status->navigatorSubMode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS, (const char *)shift_system_status, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_shift_system_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t statusValueIndicators, float statusValue1, float statusValue2, uint8_t objectModeObjectStatus, uint8_t navigatorMode, uint8_t navigatorSubMode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, statusValue1);
    _mav_put_float(buf, 4, statusValue2);
    _mav_put_uint8_t(buf, 8, statusValueIndicators);
    _mav_put_uint8_t(buf, 9, objectModeObjectStatus);
    _mav_put_uint8_t(buf, 10, navigatorMode);
    _mav_put_uint8_t(buf, 11, navigatorSubMode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS, buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
#else
    mavlink_shift_system_status_t *packet = (mavlink_shift_system_status_t *)msgbuf;
    packet->statusValue1 = statusValue1;
    packet->statusValue2 = statusValue2;
    packet->statusValueIndicators = statusValueIndicators;
    packet->objectModeObjectStatus = objectModeObjectStatus;
    packet->navigatorMode = navigatorMode;
    packet->navigatorSubMode = navigatorSubMode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS, (const char *)packet, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE SHIFT_SYSTEM_STATUS UNPACKING


/**
 * @brief Get field statusValueIndicators from shift_system_status message
 *
 * @return 128 =to= 128
 */
static inline uint8_t mavlink_msg_shift_system_status_get_statusValueIndicators(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field statusValue1 from shift_system_status message
 *
 * @return This value represents what the mask is indicating
 */
static inline float mavlink_msg_shift_system_status_get_statusValue1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field statusValue2 from shift_system_status message
 *
 * @return This value represents what the mask is indicating
 */
static inline float mavlink_msg_shift_system_status_get_statusValue2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field objectModeObjectStatus from shift_system_status message
 *
 * @return Current mode and status of the object uint_4 and uint_4
 */
static inline uint8_t mavlink_msg_shift_system_status_get_objectModeObjectStatus(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field navigatorMode from shift_system_status message
 *
 * @return Current mode of the navigator
 */
static inline uint8_t mavlink_msg_shift_system_status_get_navigatorMode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field navigatorSubMode from shift_system_status message
 *
 * @return Current sub mode of the navigator
 */
static inline uint8_t mavlink_msg_shift_system_status_get_navigatorSubMode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  11);
}

/**
 * @brief Decode a shift_system_status message into a struct
 *
 * @param msg The message to decode
 * @param shift_system_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_shift_system_status_decode(const mavlink_message_t* msg, mavlink_shift_system_status_t* shift_system_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    shift_system_status->statusValue1 = mavlink_msg_shift_system_status_get_statusValue1(msg);
    shift_system_status->statusValue2 = mavlink_msg_shift_system_status_get_statusValue2(msg);
    shift_system_status->statusValueIndicators = mavlink_msg_shift_system_status_get_statusValueIndicators(msg);
    shift_system_status->objectModeObjectStatus = mavlink_msg_shift_system_status_get_objectModeObjectStatus(msg);
    shift_system_status->navigatorMode = mavlink_msg_shift_system_status_get_navigatorMode(msg);
    shift_system_status->navigatorSubMode = mavlink_msg_shift_system_status_get_navigatorSubMode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN? msg->len : MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN;
        memset(shift_system_status, 0, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN);
    memcpy(shift_system_status, _MAV_PAYLOAD(msg), len);
#endif
}
