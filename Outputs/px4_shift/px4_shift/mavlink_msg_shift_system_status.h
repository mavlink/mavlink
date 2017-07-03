#pragma once
// MESSAGE SHIFT_SYSTEM_STATUS PACKING

#define MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS 565

MAVPACKED(
typedef struct __mavlink_shift_system_status_t {
 uint16_t currentPathIndex; /*< The current PathIndex traveling*/
 uint16_t currentPointIndex; /*< The current PointIndex traveling*/
 uint8_t batteryRemaining; /*< Remaining percentage of the battery*/
 uint8_t navigatorMode; /*< Current mode of the navigator*/
 uint8_t navigatorSubMode; /*< Current sub mode of the navigator*/
 uint8_t objectMode; /*< Current mode of the object*/
 uint8_t detailField; /*< Detail field*/
}) mavlink_shift_system_status_t;

#define MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN 9
#define MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN 9
#define MAVLINK_MSG_ID_565_LEN 9
#define MAVLINK_MSG_ID_565_MIN_LEN 9

#define MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC 4
#define MAVLINK_MSG_ID_565_CRC 4



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SHIFT_SYSTEM_STATUS { \
    565, \
    "SHIFT_SYSTEM_STATUS", \
    7, \
    {  { "currentPathIndex", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_shift_system_status_t, currentPathIndex) }, \
         { "currentPointIndex", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_shift_system_status_t, currentPointIndex) }, \
         { "batteryRemaining", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_shift_system_status_t, batteryRemaining) }, \
         { "navigatorMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_shift_system_status_t, navigatorMode) }, \
         { "navigatorSubMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_shift_system_status_t, navigatorSubMode) }, \
         { "objectMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_shift_system_status_t, objectMode) }, \
         { "detailField", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_shift_system_status_t, detailField) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SHIFT_SYSTEM_STATUS { \
    "SHIFT_SYSTEM_STATUS", \
    7, \
    {  { "currentPathIndex", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_shift_system_status_t, currentPathIndex) }, \
         { "currentPointIndex", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_shift_system_status_t, currentPointIndex) }, \
         { "batteryRemaining", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_shift_system_status_t, batteryRemaining) }, \
         { "navigatorMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_shift_system_status_t, navigatorMode) }, \
         { "navigatorSubMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_shift_system_status_t, navigatorSubMode) }, \
         { "objectMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_shift_system_status_t, objectMode) }, \
         { "detailField", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_shift_system_status_t, detailField) }, \
         } \
}
#endif

/**
 * @brief Pack a shift_system_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param currentPathIndex The current PathIndex traveling
 * @param currentPointIndex The current PointIndex traveling
 * @param batteryRemaining Remaining percentage of the battery
 * @param navigatorMode Current mode of the navigator
 * @param navigatorSubMode Current sub mode of the navigator
 * @param objectMode Current mode of the object
 * @param detailField Detail field
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_system_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint16_t currentPathIndex, uint16_t currentPointIndex, uint8_t batteryRemaining, uint8_t navigatorMode, uint8_t navigatorSubMode, uint8_t objectMode, uint8_t detailField)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN];
    _mav_put_uint16_t(buf, 0, currentPathIndex);
    _mav_put_uint16_t(buf, 2, currentPointIndex);
    _mav_put_uint8_t(buf, 4, batteryRemaining);
    _mav_put_uint8_t(buf, 5, navigatorMode);
    _mav_put_uint8_t(buf, 6, navigatorSubMode);
    _mav_put_uint8_t(buf, 7, objectMode);
    _mav_put_uint8_t(buf, 8, detailField);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN);
#else
    mavlink_shift_system_status_t packet;
    packet.currentPathIndex = currentPathIndex;
    packet.currentPointIndex = currentPointIndex;
    packet.batteryRemaining = batteryRemaining;
    packet.navigatorMode = navigatorMode;
    packet.navigatorSubMode = navigatorSubMode;
    packet.objectMode = objectMode;
    packet.detailField = detailField;

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
 * @param currentPathIndex The current PathIndex traveling
 * @param currentPointIndex The current PointIndex traveling
 * @param batteryRemaining Remaining percentage of the battery
 * @param navigatorMode Current mode of the navigator
 * @param navigatorSubMode Current sub mode of the navigator
 * @param objectMode Current mode of the object
 * @param detailField Detail field
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_system_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint16_t currentPathIndex,uint16_t currentPointIndex,uint8_t batteryRemaining,uint8_t navigatorMode,uint8_t navigatorSubMode,uint8_t objectMode,uint8_t detailField)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN];
    _mav_put_uint16_t(buf, 0, currentPathIndex);
    _mav_put_uint16_t(buf, 2, currentPointIndex);
    _mav_put_uint8_t(buf, 4, batteryRemaining);
    _mav_put_uint8_t(buf, 5, navigatorMode);
    _mav_put_uint8_t(buf, 6, navigatorSubMode);
    _mav_put_uint8_t(buf, 7, objectMode);
    _mav_put_uint8_t(buf, 8, detailField);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN);
#else
    mavlink_shift_system_status_t packet;
    packet.currentPathIndex = currentPathIndex;
    packet.currentPointIndex = currentPointIndex;
    packet.batteryRemaining = batteryRemaining;
    packet.navigatorMode = navigatorMode;
    packet.navigatorSubMode = navigatorSubMode;
    packet.objectMode = objectMode;
    packet.detailField = detailField;

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
    return mavlink_msg_shift_system_status_pack(system_id, component_id, msg, shift_system_status->currentPathIndex, shift_system_status->currentPointIndex, shift_system_status->batteryRemaining, shift_system_status->navigatorMode, shift_system_status->navigatorSubMode, shift_system_status->objectMode, shift_system_status->detailField);
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
    return mavlink_msg_shift_system_status_pack_chan(system_id, component_id, chan, msg, shift_system_status->currentPathIndex, shift_system_status->currentPointIndex, shift_system_status->batteryRemaining, shift_system_status->navigatorMode, shift_system_status->navigatorSubMode, shift_system_status->objectMode, shift_system_status->detailField);
}

/**
 * @brief Send a shift_system_status message
 * @param chan MAVLink channel to send the message
 *
 * @param currentPathIndex The current PathIndex traveling
 * @param currentPointIndex The current PointIndex traveling
 * @param batteryRemaining Remaining percentage of the battery
 * @param navigatorMode Current mode of the navigator
 * @param navigatorSubMode Current sub mode of the navigator
 * @param objectMode Current mode of the object
 * @param detailField Detail field
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_shift_system_status_send(mavlink_channel_t chan, uint16_t currentPathIndex, uint16_t currentPointIndex, uint8_t batteryRemaining, uint8_t navigatorMode, uint8_t navigatorSubMode, uint8_t objectMode, uint8_t detailField)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN];
    _mav_put_uint16_t(buf, 0, currentPathIndex);
    _mav_put_uint16_t(buf, 2, currentPointIndex);
    _mav_put_uint8_t(buf, 4, batteryRemaining);
    _mav_put_uint8_t(buf, 5, navigatorMode);
    _mav_put_uint8_t(buf, 6, navigatorSubMode);
    _mav_put_uint8_t(buf, 7, objectMode);
    _mav_put_uint8_t(buf, 8, detailField);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS, buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
#else
    mavlink_shift_system_status_t packet;
    packet.currentPathIndex = currentPathIndex;
    packet.currentPointIndex = currentPointIndex;
    packet.batteryRemaining = batteryRemaining;
    packet.navigatorMode = navigatorMode;
    packet.navigatorSubMode = navigatorSubMode;
    packet.objectMode = objectMode;
    packet.detailField = detailField;

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
    mavlink_msg_shift_system_status_send(chan, shift_system_status->currentPathIndex, shift_system_status->currentPointIndex, shift_system_status->batteryRemaining, shift_system_status->navigatorMode, shift_system_status->navigatorSubMode, shift_system_status->objectMode, shift_system_status->detailField);
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
static inline void mavlink_msg_shift_system_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint16_t currentPathIndex, uint16_t currentPointIndex, uint8_t batteryRemaining, uint8_t navigatorMode, uint8_t navigatorSubMode, uint8_t objectMode, uint8_t detailField)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, currentPathIndex);
    _mav_put_uint16_t(buf, 2, currentPointIndex);
    _mav_put_uint8_t(buf, 4, batteryRemaining);
    _mav_put_uint8_t(buf, 5, navigatorMode);
    _mav_put_uint8_t(buf, 6, navigatorSubMode);
    _mav_put_uint8_t(buf, 7, objectMode);
    _mav_put_uint8_t(buf, 8, detailField);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS, buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
#else
    mavlink_shift_system_status_t *packet = (mavlink_shift_system_status_t *)msgbuf;
    packet->currentPathIndex = currentPathIndex;
    packet->currentPointIndex = currentPointIndex;
    packet->batteryRemaining = batteryRemaining;
    packet->navigatorMode = navigatorMode;
    packet->navigatorSubMode = navigatorSubMode;
    packet->objectMode = objectMode;
    packet->detailField = detailField;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS, (const char *)packet, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE SHIFT_SYSTEM_STATUS UNPACKING


/**
 * @brief Get field currentPathIndex from shift_system_status message
 *
 * @return The current PathIndex traveling
 */
static inline uint16_t mavlink_msg_shift_system_status_get_currentPathIndex(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field currentPointIndex from shift_system_status message
 *
 * @return The current PointIndex traveling
 */
static inline uint16_t mavlink_msg_shift_system_status_get_currentPointIndex(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Get field batteryRemaining from shift_system_status message
 *
 * @return Remaining percentage of the battery
 */
static inline uint8_t mavlink_msg_shift_system_status_get_batteryRemaining(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field navigatorMode from shift_system_status message
 *
 * @return Current mode of the navigator
 */
static inline uint8_t mavlink_msg_shift_system_status_get_navigatorMode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field navigatorSubMode from shift_system_status message
 *
 * @return Current sub mode of the navigator
 */
static inline uint8_t mavlink_msg_shift_system_status_get_navigatorSubMode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field objectMode from shift_system_status message
 *
 * @return Current mode of the object
 */
static inline uint8_t mavlink_msg_shift_system_status_get_objectMode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  7);
}

/**
 * @brief Get field detailField from shift_system_status message
 *
 * @return Detail field
 */
static inline uint8_t mavlink_msg_shift_system_status_get_detailField(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
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
    shift_system_status->currentPathIndex = mavlink_msg_shift_system_status_get_currentPathIndex(msg);
    shift_system_status->currentPointIndex = mavlink_msg_shift_system_status_get_currentPointIndex(msg);
    shift_system_status->batteryRemaining = mavlink_msg_shift_system_status_get_batteryRemaining(msg);
    shift_system_status->navigatorMode = mavlink_msg_shift_system_status_get_navigatorMode(msg);
    shift_system_status->navigatorSubMode = mavlink_msg_shift_system_status_get_navigatorSubMode(msg);
    shift_system_status->objectMode = mavlink_msg_shift_system_status_get_objectMode(msg);
    shift_system_status->detailField = mavlink_msg_shift_system_status_get_detailField(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN? msg->len : MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN;
        memset(shift_system_status, 0, MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_LEN);
    memcpy(shift_system_status, _MAV_PAYLOAD(msg), len);
#endif
}
