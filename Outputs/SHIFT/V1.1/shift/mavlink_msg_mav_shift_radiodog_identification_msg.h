#pragma once
// MESSAGE MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG PACKING

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG 500

MAVPACKED(
typedef struct __mavlink_mav_shift_radiodog_identification_msg_t {
 uint32_t moduleId; /*< RadioDog Module ID*/
 int32_t currentXposition; /*< Module position X in mm*/
 int32_t currentYposition; /*< Module position Y in mm*/
 int32_t currentZposition; /*< Module position Z in mm*/
 uint16_t radioId; /*< RadioDog Radio ID*/
 uint8_t moduleType; /*< Module Type*/
 uint8_t moduleIsActive; /*< Module Active*/
}) mavlink_mav_shift_radiodog_identification_msg_t;

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN 20
#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN 20
#define MAVLINK_MSG_ID_500_LEN 20
#define MAVLINK_MSG_ID_500_MIN_LEN 20

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_CRC 203
#define MAVLINK_MSG_ID_500_CRC 203



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG { \
    500, \
    "MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG", \
    7, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, moduleId) }, \
         { "currentXposition", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, currentXposition) }, \
         { "currentYposition", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, currentYposition) }, \
         { "currentZposition", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, currentZposition) }, \
         { "radioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, radioId) }, \
         { "moduleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, moduleType) }, \
         { "moduleIsActive", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, moduleIsActive) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG { \
    "MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG", \
    7, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, moduleId) }, \
         { "currentXposition", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, currentXposition) }, \
         { "currentYposition", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, currentYposition) }, \
         { "currentZposition", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, currentZposition) }, \
         { "radioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, radioId) }, \
         { "moduleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, moduleType) }, \
         { "moduleIsActive", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_mav_shift_radiodog_identification_msg_t, moduleIsActive) }, \
         } \
}
#endif

/**
 * @brief Pack a mav_shift_radiodog_identification_msg message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param moduleId RadioDog Module ID
 * @param radioId RadioDog Radio ID
 * @param moduleType Module Type
 * @param moduleIsActive Module Active
 * @param currentXposition Module position X in mm
 * @param currentYposition Module position Y in mm
 * @param currentZposition Module position Z in mm
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_identification_msg_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t moduleId, uint16_t radioId, uint8_t moduleType, uint8_t moduleIsActive, int32_t currentXposition, int32_t currentYposition, int32_t currentZposition)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_int32_t(buf, 4, currentXposition);
    _mav_put_int32_t(buf, 8, currentYposition);
    _mav_put_int32_t(buf, 12, currentZposition);
    _mav_put_uint16_t(buf, 16, radioId);
    _mav_put_uint8_t(buf, 18, moduleType);
    _mav_put_uint8_t(buf, 19, moduleIsActive);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN);
#else
    mavlink_mav_shift_radiodog_identification_msg_t packet;
    packet.moduleId = moduleId;
    packet.currentXposition = currentXposition;
    packet.currentYposition = currentYposition;
    packet.currentZposition = currentZposition;
    packet.radioId = radioId;
    packet.moduleType = moduleType;
    packet.moduleIsActive = moduleIsActive;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_CRC);
}

/**
 * @brief Pack a mav_shift_radiodog_identification_msg message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param moduleId RadioDog Module ID
 * @param radioId RadioDog Radio ID
 * @param moduleType Module Type
 * @param moduleIsActive Module Active
 * @param currentXposition Module position X in mm
 * @param currentYposition Module position Y in mm
 * @param currentZposition Module position Z in mm
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_identification_msg_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t moduleId,uint16_t radioId,uint8_t moduleType,uint8_t moduleIsActive,int32_t currentXposition,int32_t currentYposition,int32_t currentZposition)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_int32_t(buf, 4, currentXposition);
    _mav_put_int32_t(buf, 8, currentYposition);
    _mav_put_int32_t(buf, 12, currentZposition);
    _mav_put_uint16_t(buf, 16, radioId);
    _mav_put_uint8_t(buf, 18, moduleType);
    _mav_put_uint8_t(buf, 19, moduleIsActive);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN);
#else
    mavlink_mav_shift_radiodog_identification_msg_t packet;
    packet.moduleId = moduleId;
    packet.currentXposition = currentXposition;
    packet.currentYposition = currentYposition;
    packet.currentZposition = currentZposition;
    packet.radioId = radioId;
    packet.moduleType = moduleType;
    packet.moduleIsActive = moduleIsActive;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_CRC);
}

/**
 * @brief Encode a mav_shift_radiodog_identification_msg struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_identification_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_identification_msg_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_identification_msg_t* mav_shift_radiodog_identification_msg)
{
    return mavlink_msg_mav_shift_radiodog_identification_msg_pack(system_id, component_id, msg, mav_shift_radiodog_identification_msg->moduleId, mav_shift_radiodog_identification_msg->radioId, mav_shift_radiodog_identification_msg->moduleType, mav_shift_radiodog_identification_msg->moduleIsActive, mav_shift_radiodog_identification_msg->currentXposition, mav_shift_radiodog_identification_msg->currentYposition, mav_shift_radiodog_identification_msg->currentZposition);
}

/**
 * @brief Encode a mav_shift_radiodog_identification_msg struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_identification_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_identification_msg_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_identification_msg_t* mav_shift_radiodog_identification_msg)
{
    return mavlink_msg_mav_shift_radiodog_identification_msg_pack_chan(system_id, component_id, chan, msg, mav_shift_radiodog_identification_msg->moduleId, mav_shift_radiodog_identification_msg->radioId, mav_shift_radiodog_identification_msg->moduleType, mav_shift_radiodog_identification_msg->moduleIsActive, mav_shift_radiodog_identification_msg->currentXposition, mav_shift_radiodog_identification_msg->currentYposition, mav_shift_radiodog_identification_msg->currentZposition);
}

/**
 * @brief Send a mav_shift_radiodog_identification_msg message
 * @param chan MAVLink channel to send the message
 *
 * @param moduleId RadioDog Module ID
 * @param radioId RadioDog Radio ID
 * @param moduleType Module Type
 * @param moduleIsActive Module Active
 * @param currentXposition Module position X in mm
 * @param currentYposition Module position Y in mm
 * @param currentZposition Module position Z in mm
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mav_shift_radiodog_identification_msg_send(mavlink_channel_t chan, uint32_t moduleId, uint16_t radioId, uint8_t moduleType, uint8_t moduleIsActive, int32_t currentXposition, int32_t currentYposition, int32_t currentZposition)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_int32_t(buf, 4, currentXposition);
    _mav_put_int32_t(buf, 8, currentYposition);
    _mav_put_int32_t(buf, 12, currentZposition);
    _mav_put_uint16_t(buf, 16, radioId);
    _mav_put_uint8_t(buf, 18, moduleType);
    _mav_put_uint8_t(buf, 19, moduleIsActive);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_CRC);
#else
    mavlink_mav_shift_radiodog_identification_msg_t packet;
    packet.moduleId = moduleId;
    packet.currentXposition = currentXposition;
    packet.currentYposition = currentYposition;
    packet.currentZposition = currentZposition;
    packet.radioId = radioId;
    packet.moduleType = moduleType;
    packet.moduleIsActive = moduleIsActive;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG, (const char *)&packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_CRC);
#endif
}

/**
 * @brief Send a mav_shift_radiodog_identification_msg message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_mav_shift_radiodog_identification_msg_send_struct(mavlink_channel_t chan, const mavlink_mav_shift_radiodog_identification_msg_t* mav_shift_radiodog_identification_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_mav_shift_radiodog_identification_msg_send(chan, mav_shift_radiodog_identification_msg->moduleId, mav_shift_radiodog_identification_msg->radioId, mav_shift_radiodog_identification_msg->moduleType, mav_shift_radiodog_identification_msg->moduleIsActive, mav_shift_radiodog_identification_msg->currentXposition, mav_shift_radiodog_identification_msg->currentYposition, mav_shift_radiodog_identification_msg->currentZposition);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG, (const char *)mav_shift_radiodog_identification_msg, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_CRC);
#endif
}

#if MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mav_shift_radiodog_identification_msg_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t moduleId, uint16_t radioId, uint8_t moduleType, uint8_t moduleIsActive, int32_t currentXposition, int32_t currentYposition, int32_t currentZposition)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_int32_t(buf, 4, currentXposition);
    _mav_put_int32_t(buf, 8, currentYposition);
    _mav_put_int32_t(buf, 12, currentZposition);
    _mav_put_uint16_t(buf, 16, radioId);
    _mav_put_uint8_t(buf, 18, moduleType);
    _mav_put_uint8_t(buf, 19, moduleIsActive);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_CRC);
#else
    mavlink_mav_shift_radiodog_identification_msg_t *packet = (mavlink_mav_shift_radiodog_identification_msg_t *)msgbuf;
    packet->moduleId = moduleId;
    packet->currentXposition = currentXposition;
    packet->currentYposition = currentYposition;
    packet->currentZposition = currentZposition;
    packet->radioId = radioId;
    packet->moduleType = moduleType;
    packet->moduleIsActive = moduleIsActive;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG, (const char *)packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_CRC);
#endif
}
#endif

#endif

// MESSAGE MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG UNPACKING


/**
 * @brief Get field moduleId from mav_shift_radiodog_identification_msg message
 *
 * @return RadioDog Module ID
 */
static inline uint32_t mavlink_msg_mav_shift_radiodog_identification_msg_get_moduleId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field radioId from mav_shift_radiodog_identification_msg message
 *
 * @return RadioDog Radio ID
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_identification_msg_get_radioId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  16);
}

/**
 * @brief Get field moduleType from mav_shift_radiodog_identification_msg message
 *
 * @return Module Type
 */
static inline uint8_t mavlink_msg_mav_shift_radiodog_identification_msg_get_moduleType(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field moduleIsActive from mav_shift_radiodog_identification_msg message
 *
 * @return Module Active
 */
static inline uint8_t mavlink_msg_mav_shift_radiodog_identification_msg_get_moduleIsActive(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Get field currentXposition from mav_shift_radiodog_identification_msg message
 *
 * @return Module position X in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_identification_msg_get_currentXposition(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field currentYposition from mav_shift_radiodog_identification_msg message
 *
 * @return Module position Y in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_identification_msg_get_currentYposition(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field currentZposition from mav_shift_radiodog_identification_msg message
 *
 * @return Module position Z in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_identification_msg_get_currentZposition(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Decode a mav_shift_radiodog_identification_msg message into a struct
 *
 * @param msg The message to decode
 * @param mav_shift_radiodog_identification_msg C-struct to decode the message contents into
 */
static inline void mavlink_msg_mav_shift_radiodog_identification_msg_decode(const mavlink_message_t* msg, mavlink_mav_shift_radiodog_identification_msg_t* mav_shift_radiodog_identification_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mav_shift_radiodog_identification_msg->moduleId = mavlink_msg_mav_shift_radiodog_identification_msg_get_moduleId(msg);
    mav_shift_radiodog_identification_msg->currentXposition = mavlink_msg_mav_shift_radiodog_identification_msg_get_currentXposition(msg);
    mav_shift_radiodog_identification_msg->currentYposition = mavlink_msg_mav_shift_radiodog_identification_msg_get_currentYposition(msg);
    mav_shift_radiodog_identification_msg->currentZposition = mavlink_msg_mav_shift_radiodog_identification_msg_get_currentZposition(msg);
    mav_shift_radiodog_identification_msg->radioId = mavlink_msg_mav_shift_radiodog_identification_msg_get_radioId(msg);
    mav_shift_radiodog_identification_msg->moduleType = mavlink_msg_mav_shift_radiodog_identification_msg_get_moduleType(msg);
    mav_shift_radiodog_identification_msg->moduleIsActive = mavlink_msg_mav_shift_radiodog_identification_msg_get_moduleIsActive(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN? msg->len : MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN;
        memset(mav_shift_radiodog_identification_msg, 0, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_LEN);
    memcpy(mav_shift_radiodog_identification_msg, _MAV_PAYLOAD(msg), len);
#endif
}
