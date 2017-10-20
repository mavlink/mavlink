#pragma once
// MESSAGE SHIFTW_SYSTEM_STATUS PACKING

#define MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS 575

MAVPACKED(
typedef struct __mavlink_shiftw_system_status_t {
 uint8_t status; /*< Status*/
 uint8_t serialNumber; /*< SerialNumber*/
 uint8_t estopStatus; /*< Status of the E-STOP connected to the SHIFT-W Box*/
}) mavlink_shiftw_system_status_t;

#define MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN 3
#define MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN 3
#define MAVLINK_MSG_ID_575_LEN 3
#define MAVLINK_MSG_ID_575_MIN_LEN 3

#define MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_CRC 78
#define MAVLINK_MSG_ID_575_CRC 78



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SHIFTW_SYSTEM_STATUS { \
    575, \
    "SHIFTW_SYSTEM_STATUS", \
    3, \
    {  { "status", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_shiftw_system_status_t, status) }, \
         { "serialNumber", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_shiftw_system_status_t, serialNumber) }, \
         { "estopStatus", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_shiftw_system_status_t, estopStatus) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SHIFTW_SYSTEM_STATUS { \
    "SHIFTW_SYSTEM_STATUS", \
    3, \
    {  { "status", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_shiftw_system_status_t, status) }, \
         { "serialNumber", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_shiftw_system_status_t, serialNumber) }, \
         { "estopStatus", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_shiftw_system_status_t, estopStatus) }, \
         } \
}
#endif

/**
 * @brief Pack a shiftw_system_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param status Status
 * @param serialNumber SerialNumber
 * @param estopStatus Status of the E-STOP connected to the SHIFT-W Box
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shiftw_system_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t status, uint8_t serialNumber, uint8_t estopStatus)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN];
    _mav_put_uint8_t(buf, 0, status);
    _mav_put_uint8_t(buf, 1, serialNumber);
    _mav_put_uint8_t(buf, 2, estopStatus);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN);
#else
    mavlink_shiftw_system_status_t packet;
    packet.status = status;
    packet.serialNumber = serialNumber;
    packet.estopStatus = estopStatus;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_CRC);
}

/**
 * @brief Pack a shiftw_system_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param status Status
 * @param serialNumber SerialNumber
 * @param estopStatus Status of the E-STOP connected to the SHIFT-W Box
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shiftw_system_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t status,uint8_t serialNumber,uint8_t estopStatus)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN];
    _mav_put_uint8_t(buf, 0, status);
    _mav_put_uint8_t(buf, 1, serialNumber);
    _mav_put_uint8_t(buf, 2, estopStatus);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN);
#else
    mavlink_shiftw_system_status_t packet;
    packet.status = status;
    packet.serialNumber = serialNumber;
    packet.estopStatus = estopStatus;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_CRC);
}

/**
 * @brief Encode a shiftw_system_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param shiftw_system_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shiftw_system_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_shiftw_system_status_t* shiftw_system_status)
{
    return mavlink_msg_shiftw_system_status_pack(system_id, component_id, msg, shiftw_system_status->status, shiftw_system_status->serialNumber, shiftw_system_status->estopStatus);
}

/**
 * @brief Encode a shiftw_system_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param shiftw_system_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shiftw_system_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_shiftw_system_status_t* shiftw_system_status)
{
    return mavlink_msg_shiftw_system_status_pack_chan(system_id, component_id, chan, msg, shiftw_system_status->status, shiftw_system_status->serialNumber, shiftw_system_status->estopStatus);
}

/**
 * @brief Send a shiftw_system_status message
 * @param chan MAVLink channel to send the message
 *
 * @param status Status
 * @param serialNumber SerialNumber
 * @param estopStatus Status of the E-STOP connected to the SHIFT-W Box
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_shiftw_system_status_send(mavlink_channel_t chan, uint8_t status, uint8_t serialNumber, uint8_t estopStatus)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN];
    _mav_put_uint8_t(buf, 0, status);
    _mav_put_uint8_t(buf, 1, serialNumber);
    _mav_put_uint8_t(buf, 2, estopStatus);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS, buf, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_CRC);
#else
    mavlink_shiftw_system_status_t packet;
    packet.status = status;
    packet.serialNumber = serialNumber;
    packet.estopStatus = estopStatus;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS, (const char *)&packet, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_CRC);
#endif
}

/**
 * @brief Send a shiftw_system_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_shiftw_system_status_send_struct(mavlink_channel_t chan, const mavlink_shiftw_system_status_t* shiftw_system_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_shiftw_system_status_send(chan, shiftw_system_status->status, shiftw_system_status->serialNumber, shiftw_system_status->estopStatus);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS, (const char *)shiftw_system_status, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_shiftw_system_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t status, uint8_t serialNumber, uint8_t estopStatus)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, status);
    _mav_put_uint8_t(buf, 1, serialNumber);
    _mav_put_uint8_t(buf, 2, estopStatus);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS, buf, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_CRC);
#else
    mavlink_shiftw_system_status_t *packet = (mavlink_shiftw_system_status_t *)msgbuf;
    packet->status = status;
    packet->serialNumber = serialNumber;
    packet->estopStatus = estopStatus;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS, (const char *)packet, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE SHIFTW_SYSTEM_STATUS UNPACKING


/**
 * @brief Get field status from shiftw_system_status message
 *
 * @return Status
 */
static inline uint8_t mavlink_msg_shiftw_system_status_get_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field serialNumber from shiftw_system_status message
 *
 * @return SerialNumber
 */
static inline uint8_t mavlink_msg_shiftw_system_status_get_serialNumber(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field estopStatus from shiftw_system_status message
 *
 * @return Status of the E-STOP connected to the SHIFT-W Box
 */
static inline uint8_t mavlink_msg_shiftw_system_status_get_estopStatus(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Decode a shiftw_system_status message into a struct
 *
 * @param msg The message to decode
 * @param shiftw_system_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_shiftw_system_status_decode(const mavlink_message_t* msg, mavlink_shiftw_system_status_t* shiftw_system_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    shiftw_system_status->status = mavlink_msg_shiftw_system_status_get_status(msg);
    shiftw_system_status->serialNumber = mavlink_msg_shiftw_system_status_get_serialNumber(msg);
    shiftw_system_status->estopStatus = mavlink_msg_shiftw_system_status_get_estopStatus(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN? msg->len : MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN;
        memset(shiftw_system_status, 0, MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_LEN);
    memcpy(shiftw_system_status, _MAV_PAYLOAD(msg), len);
#endif
}
