#pragma once
// MESSAGE MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL PACKING

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL 520

MAVPACKED(
typedef struct __mavlink_mav_shift_radiodog_px4_position_small_t {
 uint32_t moduleId; /*< RadioDog Module ID (Sending module)*/
 uint32_t positionTimeStamp; /*< RadioDog Timestamp when the position was calculated*/
 int32_t Xposition; /*< Position X in mm*/
 int32_t Yposition; /*< Position Y in mm*/
 int32_t Zposition; /*< Position Z in mm*/
 uint16_t newRadioId; /*< The new RadioDog Radio ID (Sending module)*/
 uint8_t newModuleType; /*< The new Module Type*/
}) mavlink_mav_shift_radiodog_px4_position_small_t;

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN 23
#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN 23
#define MAVLINK_MSG_ID_520_LEN 23
#define MAVLINK_MSG_ID_520_MIN_LEN 23

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_CRC 43
#define MAVLINK_MSG_ID_520_CRC 43



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL { \
    520, \
    "MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL", \
    7, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, moduleId) }, \
         { "positionTimeStamp", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, positionTimeStamp) }, \
         { "Xposition", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, Xposition) }, \
         { "Yposition", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, Yposition) }, \
         { "Zposition", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, Zposition) }, \
         { "newRadioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 20, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, newRadioId) }, \
         { "newModuleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, newModuleType) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL { \
    "MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL", \
    7, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, moduleId) }, \
         { "positionTimeStamp", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, positionTimeStamp) }, \
         { "Xposition", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, Xposition) }, \
         { "Yposition", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, Yposition) }, \
         { "Zposition", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, Zposition) }, \
         { "newRadioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 20, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, newRadioId) }, \
         { "newModuleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_mav_shift_radiodog_px4_position_small_t, newModuleType) }, \
         } \
}
#endif

/**
 * @brief Pack a mav_shift_radiodog_px4_position_small message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param newRadioId The new RadioDog Radio ID (Sending module)
 * @param newModuleType The new Module Type
 * @param positionTimeStamp RadioDog Timestamp when the position was calculated
 * @param Xposition Position X in mm
 * @param Yposition Position Y in mm
 * @param Zposition Position Z in mm
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_px4_position_small_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t moduleId, uint16_t newRadioId, uint8_t newModuleType, uint32_t positionTimeStamp, int32_t Xposition, int32_t Yposition, int32_t Zposition)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint32_t(buf, 4, positionTimeStamp);
    _mav_put_int32_t(buf, 8, Xposition);
    _mav_put_int32_t(buf, 12, Yposition);
    _mav_put_int32_t(buf, 16, Zposition);
    _mav_put_uint16_t(buf, 20, newRadioId);
    _mav_put_uint8_t(buf, 22, newModuleType);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN);
#else
    mavlink_mav_shift_radiodog_px4_position_small_t packet;
    packet.moduleId = moduleId;
    packet.positionTimeStamp = positionTimeStamp;
    packet.Xposition = Xposition;
    packet.Yposition = Yposition;
    packet.Zposition = Zposition;
    packet.newRadioId = newRadioId;
    packet.newModuleType = newModuleType;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_CRC);
}

/**
 * @brief Pack a mav_shift_radiodog_px4_position_small message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param moduleId RadioDog Module ID (Sending module)
 * @param newRadioId The new RadioDog Radio ID (Sending module)
 * @param newModuleType The new Module Type
 * @param positionTimeStamp RadioDog Timestamp when the position was calculated
 * @param Xposition Position X in mm
 * @param Yposition Position Y in mm
 * @param Zposition Position Z in mm
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_px4_position_small_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t moduleId,uint16_t newRadioId,uint8_t newModuleType,uint32_t positionTimeStamp,int32_t Xposition,int32_t Yposition,int32_t Zposition)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint32_t(buf, 4, positionTimeStamp);
    _mav_put_int32_t(buf, 8, Xposition);
    _mav_put_int32_t(buf, 12, Yposition);
    _mav_put_int32_t(buf, 16, Zposition);
    _mav_put_uint16_t(buf, 20, newRadioId);
    _mav_put_uint8_t(buf, 22, newModuleType);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN);
#else
    mavlink_mav_shift_radiodog_px4_position_small_t packet;
    packet.moduleId = moduleId;
    packet.positionTimeStamp = positionTimeStamp;
    packet.Xposition = Xposition;
    packet.Yposition = Yposition;
    packet.Zposition = Zposition;
    packet.newRadioId = newRadioId;
    packet.newModuleType = newModuleType;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_CRC);
}

/**
 * @brief Encode a mav_shift_radiodog_px4_position_small struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_px4_position_small C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_px4_position_small_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_px4_position_small_t* mav_shift_radiodog_px4_position_small)
{
    return mavlink_msg_mav_shift_radiodog_px4_position_small_pack(system_id, component_id, msg, mav_shift_radiodog_px4_position_small->moduleId, mav_shift_radiodog_px4_position_small->newRadioId, mav_shift_radiodog_px4_position_small->newModuleType, mav_shift_radiodog_px4_position_small->positionTimeStamp, mav_shift_radiodog_px4_position_small->Xposition, mav_shift_radiodog_px4_position_small->Yposition, mav_shift_radiodog_px4_position_small->Zposition);
}

/**
 * @brief Encode a mav_shift_radiodog_px4_position_small struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_px4_position_small C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_px4_position_small_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_px4_position_small_t* mav_shift_radiodog_px4_position_small)
{
    return mavlink_msg_mav_shift_radiodog_px4_position_small_pack_chan(system_id, component_id, chan, msg, mav_shift_radiodog_px4_position_small->moduleId, mav_shift_radiodog_px4_position_small->newRadioId, mav_shift_radiodog_px4_position_small->newModuleType, mav_shift_radiodog_px4_position_small->positionTimeStamp, mav_shift_radiodog_px4_position_small->Xposition, mav_shift_radiodog_px4_position_small->Yposition, mav_shift_radiodog_px4_position_small->Zposition);
}

/**
 * @brief Send a mav_shift_radiodog_px4_position_small message
 * @param chan MAVLink channel to send the message
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param newRadioId The new RadioDog Radio ID (Sending module)
 * @param newModuleType The new Module Type
 * @param positionTimeStamp RadioDog Timestamp when the position was calculated
 * @param Xposition Position X in mm
 * @param Yposition Position Y in mm
 * @param Zposition Position Z in mm
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mav_shift_radiodog_px4_position_small_send(mavlink_channel_t chan, uint32_t moduleId, uint16_t newRadioId, uint8_t newModuleType, uint32_t positionTimeStamp, int32_t Xposition, int32_t Yposition, int32_t Zposition)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint32_t(buf, 4, positionTimeStamp);
    _mav_put_int32_t(buf, 8, Xposition);
    _mav_put_int32_t(buf, 12, Yposition);
    _mav_put_int32_t(buf, 16, Zposition);
    _mav_put_uint16_t(buf, 20, newRadioId);
    _mav_put_uint8_t(buf, 22, newModuleType);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_CRC);
#else
    mavlink_mav_shift_radiodog_px4_position_small_t packet;
    packet.moduleId = moduleId;
    packet.positionTimeStamp = positionTimeStamp;
    packet.Xposition = Xposition;
    packet.Yposition = Yposition;
    packet.Zposition = Zposition;
    packet.newRadioId = newRadioId;
    packet.newModuleType = newModuleType;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL, (const char *)&packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_CRC);
#endif
}

/**
 * @brief Send a mav_shift_radiodog_px4_position_small message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_mav_shift_radiodog_px4_position_small_send_struct(mavlink_channel_t chan, const mavlink_mav_shift_radiodog_px4_position_small_t* mav_shift_radiodog_px4_position_small)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_mav_shift_radiodog_px4_position_small_send(chan, mav_shift_radiodog_px4_position_small->moduleId, mav_shift_radiodog_px4_position_small->newRadioId, mav_shift_radiodog_px4_position_small->newModuleType, mav_shift_radiodog_px4_position_small->positionTimeStamp, mav_shift_radiodog_px4_position_small->Xposition, mav_shift_radiodog_px4_position_small->Yposition, mav_shift_radiodog_px4_position_small->Zposition);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL, (const char *)mav_shift_radiodog_px4_position_small, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_CRC);
#endif
}

#if MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mav_shift_radiodog_px4_position_small_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t moduleId, uint16_t newRadioId, uint8_t newModuleType, uint32_t positionTimeStamp, int32_t Xposition, int32_t Yposition, int32_t Zposition)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint32_t(buf, 4, positionTimeStamp);
    _mav_put_int32_t(buf, 8, Xposition);
    _mav_put_int32_t(buf, 12, Yposition);
    _mav_put_int32_t(buf, 16, Zposition);
    _mav_put_uint16_t(buf, 20, newRadioId);
    _mav_put_uint8_t(buf, 22, newModuleType);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_CRC);
#else
    mavlink_mav_shift_radiodog_px4_position_small_t *packet = (mavlink_mav_shift_radiodog_px4_position_small_t *)msgbuf;
    packet->moduleId = moduleId;
    packet->positionTimeStamp = positionTimeStamp;
    packet->Xposition = Xposition;
    packet->Yposition = Yposition;
    packet->Zposition = Zposition;
    packet->newRadioId = newRadioId;
    packet->newModuleType = newModuleType;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL, (const char *)packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_CRC);
#endif
}
#endif

#endif

// MESSAGE MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL UNPACKING


/**
 * @brief Get field moduleId from mav_shift_radiodog_px4_position_small message
 *
 * @return RadioDog Module ID (Sending module)
 */
static inline uint32_t mavlink_msg_mav_shift_radiodog_px4_position_small_get_moduleId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field newRadioId from mav_shift_radiodog_px4_position_small message
 *
 * @return The new RadioDog Radio ID (Sending module)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_px4_position_small_get_newRadioId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  20);
}

/**
 * @brief Get field newModuleType from mav_shift_radiodog_px4_position_small message
 *
 * @return The new Module Type
 */
static inline uint8_t mavlink_msg_mav_shift_radiodog_px4_position_small_get_newModuleType(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  22);
}

/**
 * @brief Get field positionTimeStamp from mav_shift_radiodog_px4_position_small message
 *
 * @return RadioDog Timestamp when the position was calculated
 */
static inline uint32_t mavlink_msg_mav_shift_radiodog_px4_position_small_get_positionTimeStamp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field Xposition from mav_shift_radiodog_px4_position_small message
 *
 * @return Position X in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_px4_position_small_get_Xposition(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field Yposition from mav_shift_radiodog_px4_position_small message
 *
 * @return Position Y in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_px4_position_small_get_Yposition(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Get field Zposition from mav_shift_radiodog_px4_position_small message
 *
 * @return Position Z in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_px4_position_small_get_Zposition(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  16);
}

/**
 * @brief Decode a mav_shift_radiodog_px4_position_small message into a struct
 *
 * @param msg The message to decode
 * @param mav_shift_radiodog_px4_position_small C-struct to decode the message contents into
 */
static inline void mavlink_msg_mav_shift_radiodog_px4_position_small_decode(const mavlink_message_t* msg, mavlink_mav_shift_radiodog_px4_position_small_t* mav_shift_radiodog_px4_position_small)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mav_shift_radiodog_px4_position_small->moduleId = mavlink_msg_mav_shift_radiodog_px4_position_small_get_moduleId(msg);
    mav_shift_radiodog_px4_position_small->positionTimeStamp = mavlink_msg_mav_shift_radiodog_px4_position_small_get_positionTimeStamp(msg);
    mav_shift_radiodog_px4_position_small->Xposition = mavlink_msg_mav_shift_radiodog_px4_position_small_get_Xposition(msg);
    mav_shift_radiodog_px4_position_small->Yposition = mavlink_msg_mav_shift_radiodog_px4_position_small_get_Yposition(msg);
    mav_shift_radiodog_px4_position_small->Zposition = mavlink_msg_mav_shift_radiodog_px4_position_small_get_Zposition(msg);
    mav_shift_radiodog_px4_position_small->newRadioId = mavlink_msg_mav_shift_radiodog_px4_position_small_get_newRadioId(msg);
    mav_shift_radiodog_px4_position_small->newModuleType = mavlink_msg_mav_shift_radiodog_px4_position_small_get_newModuleType(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN? msg->len : MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN;
        memset(mav_shift_radiodog_px4_position_small, 0, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_LEN);
    memcpy(mav_shift_radiodog_px4_position_small, _MAV_PAYLOAD(msg), len);
#endif
}
