#pragma once
// MESSAGE MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO PACKING

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO 550

MAVPACKED(
typedef struct __mavlink_mav_shift_radiodog_debug_tag_info_t {
 uint32_t moduleId; /*< RadioDog Module ID (Sending module)*/
 uint32_t positionTimeStamp; /*< RadioDog Timestamp when the position was calculated*/
 int32_t Anchor_A_Ranging_Raw; /*< Raw anchor A ranging value in mm*/
 int32_t Anchor_A_Ranging_Filtered; /*< Filtered anchor A ranging value in mm*/
 int32_t Anchor_B_Ranging_Raw; /*< Raw anchor B ranging value in mm*/
 int32_t Anchor_B_Ranging_Filtered; /*< Filtered anchor B ranging value in mm*/
 int32_t Anchor_C_Ranging_Raw; /*< Raw anchor C ranging value in mm*/
 int32_t Anchor_C_Ranging_Filtered; /*< Filtered anchor C ranging value in mm*/
 uint16_t radioId; /*< The RadioDog Radio ID (Sending module)*/
 uint8_t moduleType; /*< The Module Type*/
}) mavlink_mav_shift_radiodog_debug_tag_info_t;

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN 35
#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN 35
#define MAVLINK_MSG_ID_550_LEN 35
#define MAVLINK_MSG_ID_550_MIN_LEN 35

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_CRC 215
#define MAVLINK_MSG_ID_550_CRC 215



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO { \
    550, \
    "MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO", \
    10, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, moduleId) }, \
         { "positionTimeStamp", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, positionTimeStamp) }, \
         { "Anchor_A_Ranging_Raw", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_A_Ranging_Raw) }, \
         { "Anchor_A_Ranging_Filtered", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_A_Ranging_Filtered) }, \
         { "Anchor_B_Ranging_Raw", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_B_Ranging_Raw) }, \
         { "Anchor_B_Ranging_Filtered", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_B_Ranging_Filtered) }, \
         { "Anchor_C_Ranging_Raw", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_C_Ranging_Raw) }, \
         { "Anchor_C_Ranging_Filtered", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_C_Ranging_Filtered) }, \
         { "radioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 32, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, radioId) }, \
         { "moduleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 34, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, moduleType) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO { \
    "MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO", \
    10, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, moduleId) }, \
         { "positionTimeStamp", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, positionTimeStamp) }, \
         { "Anchor_A_Ranging_Raw", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_A_Ranging_Raw) }, \
         { "Anchor_A_Ranging_Filtered", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_A_Ranging_Filtered) }, \
         { "Anchor_B_Ranging_Raw", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_B_Ranging_Raw) }, \
         { "Anchor_B_Ranging_Filtered", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_B_Ranging_Filtered) }, \
         { "Anchor_C_Ranging_Raw", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_C_Ranging_Raw) }, \
         { "Anchor_C_Ranging_Filtered", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, Anchor_C_Ranging_Filtered) }, \
         { "radioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 32, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, radioId) }, \
         { "moduleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 34, offsetof(mavlink_mav_shift_radiodog_debug_tag_info_t, moduleType) }, \
         } \
}
#endif

/**
 * @brief Pack a mav_shift_radiodog_debug_tag_info message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param radioId The RadioDog Radio ID (Sending module)
 * @param moduleType The Module Type
 * @param positionTimeStamp RadioDog Timestamp when the position was calculated
 * @param Anchor_A_Ranging_Raw Raw anchor A ranging value in mm
 * @param Anchor_A_Ranging_Filtered Filtered anchor A ranging value in mm
 * @param Anchor_B_Ranging_Raw Raw anchor B ranging value in mm
 * @param Anchor_B_Ranging_Filtered Filtered anchor B ranging value in mm
 * @param Anchor_C_Ranging_Raw Raw anchor C ranging value in mm
 * @param Anchor_C_Ranging_Filtered Filtered anchor C ranging value in mm
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_debug_tag_info_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t moduleId, uint16_t radioId, uint8_t moduleType, uint32_t positionTimeStamp, int32_t Anchor_A_Ranging_Raw, int32_t Anchor_A_Ranging_Filtered, int32_t Anchor_B_Ranging_Raw, int32_t Anchor_B_Ranging_Filtered, int32_t Anchor_C_Ranging_Raw, int32_t Anchor_C_Ranging_Filtered)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint32_t(buf, 4, positionTimeStamp);
    _mav_put_int32_t(buf, 8, Anchor_A_Ranging_Raw);
    _mav_put_int32_t(buf, 12, Anchor_A_Ranging_Filtered);
    _mav_put_int32_t(buf, 16, Anchor_B_Ranging_Raw);
    _mav_put_int32_t(buf, 20, Anchor_B_Ranging_Filtered);
    _mav_put_int32_t(buf, 24, Anchor_C_Ranging_Raw);
    _mav_put_int32_t(buf, 28, Anchor_C_Ranging_Filtered);
    _mav_put_uint16_t(buf, 32, radioId);
    _mav_put_uint8_t(buf, 34, moduleType);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN);
#else
    mavlink_mav_shift_radiodog_debug_tag_info_t packet;
    packet.moduleId = moduleId;
    packet.positionTimeStamp = positionTimeStamp;
    packet.Anchor_A_Ranging_Raw = Anchor_A_Ranging_Raw;
    packet.Anchor_A_Ranging_Filtered = Anchor_A_Ranging_Filtered;
    packet.Anchor_B_Ranging_Raw = Anchor_B_Ranging_Raw;
    packet.Anchor_B_Ranging_Filtered = Anchor_B_Ranging_Filtered;
    packet.Anchor_C_Ranging_Raw = Anchor_C_Ranging_Raw;
    packet.Anchor_C_Ranging_Filtered = Anchor_C_Ranging_Filtered;
    packet.radioId = radioId;
    packet.moduleType = moduleType;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_CRC);
}

/**
 * @brief Pack a mav_shift_radiodog_debug_tag_info message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param moduleId RadioDog Module ID (Sending module)
 * @param radioId The RadioDog Radio ID (Sending module)
 * @param moduleType The Module Type
 * @param positionTimeStamp RadioDog Timestamp when the position was calculated
 * @param Anchor_A_Ranging_Raw Raw anchor A ranging value in mm
 * @param Anchor_A_Ranging_Filtered Filtered anchor A ranging value in mm
 * @param Anchor_B_Ranging_Raw Raw anchor B ranging value in mm
 * @param Anchor_B_Ranging_Filtered Filtered anchor B ranging value in mm
 * @param Anchor_C_Ranging_Raw Raw anchor C ranging value in mm
 * @param Anchor_C_Ranging_Filtered Filtered anchor C ranging value in mm
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_debug_tag_info_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t moduleId,uint16_t radioId,uint8_t moduleType,uint32_t positionTimeStamp,int32_t Anchor_A_Ranging_Raw,int32_t Anchor_A_Ranging_Filtered,int32_t Anchor_B_Ranging_Raw,int32_t Anchor_B_Ranging_Filtered,int32_t Anchor_C_Ranging_Raw,int32_t Anchor_C_Ranging_Filtered)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint32_t(buf, 4, positionTimeStamp);
    _mav_put_int32_t(buf, 8, Anchor_A_Ranging_Raw);
    _mav_put_int32_t(buf, 12, Anchor_A_Ranging_Filtered);
    _mav_put_int32_t(buf, 16, Anchor_B_Ranging_Raw);
    _mav_put_int32_t(buf, 20, Anchor_B_Ranging_Filtered);
    _mav_put_int32_t(buf, 24, Anchor_C_Ranging_Raw);
    _mav_put_int32_t(buf, 28, Anchor_C_Ranging_Filtered);
    _mav_put_uint16_t(buf, 32, radioId);
    _mav_put_uint8_t(buf, 34, moduleType);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN);
#else
    mavlink_mav_shift_radiodog_debug_tag_info_t packet;
    packet.moduleId = moduleId;
    packet.positionTimeStamp = positionTimeStamp;
    packet.Anchor_A_Ranging_Raw = Anchor_A_Ranging_Raw;
    packet.Anchor_A_Ranging_Filtered = Anchor_A_Ranging_Filtered;
    packet.Anchor_B_Ranging_Raw = Anchor_B_Ranging_Raw;
    packet.Anchor_B_Ranging_Filtered = Anchor_B_Ranging_Filtered;
    packet.Anchor_C_Ranging_Raw = Anchor_C_Ranging_Raw;
    packet.Anchor_C_Ranging_Filtered = Anchor_C_Ranging_Filtered;
    packet.radioId = radioId;
    packet.moduleType = moduleType;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_CRC);
}

/**
 * @brief Encode a mav_shift_radiodog_debug_tag_info struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_debug_tag_info C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_debug_tag_info_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_debug_tag_info_t* mav_shift_radiodog_debug_tag_info)
{
    return mavlink_msg_mav_shift_radiodog_debug_tag_info_pack(system_id, component_id, msg, mav_shift_radiodog_debug_tag_info->moduleId, mav_shift_radiodog_debug_tag_info->radioId, mav_shift_radiodog_debug_tag_info->moduleType, mav_shift_radiodog_debug_tag_info->positionTimeStamp, mav_shift_radiodog_debug_tag_info->Anchor_A_Ranging_Raw, mav_shift_radiodog_debug_tag_info->Anchor_A_Ranging_Filtered, mav_shift_radiodog_debug_tag_info->Anchor_B_Ranging_Raw, mav_shift_radiodog_debug_tag_info->Anchor_B_Ranging_Filtered, mav_shift_radiodog_debug_tag_info->Anchor_C_Ranging_Raw, mav_shift_radiodog_debug_tag_info->Anchor_C_Ranging_Filtered);
}

/**
 * @brief Encode a mav_shift_radiodog_debug_tag_info struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_debug_tag_info C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_debug_tag_info_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_debug_tag_info_t* mav_shift_radiodog_debug_tag_info)
{
    return mavlink_msg_mav_shift_radiodog_debug_tag_info_pack_chan(system_id, component_id, chan, msg, mav_shift_radiodog_debug_tag_info->moduleId, mav_shift_radiodog_debug_tag_info->radioId, mav_shift_radiodog_debug_tag_info->moduleType, mav_shift_radiodog_debug_tag_info->positionTimeStamp, mav_shift_radiodog_debug_tag_info->Anchor_A_Ranging_Raw, mav_shift_radiodog_debug_tag_info->Anchor_A_Ranging_Filtered, mav_shift_radiodog_debug_tag_info->Anchor_B_Ranging_Raw, mav_shift_radiodog_debug_tag_info->Anchor_B_Ranging_Filtered, mav_shift_radiodog_debug_tag_info->Anchor_C_Ranging_Raw, mav_shift_radiodog_debug_tag_info->Anchor_C_Ranging_Filtered);
}

/**
 * @brief Send a mav_shift_radiodog_debug_tag_info message
 * @param chan MAVLink channel to send the message
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param radioId The RadioDog Radio ID (Sending module)
 * @param moduleType The Module Type
 * @param positionTimeStamp RadioDog Timestamp when the position was calculated
 * @param Anchor_A_Ranging_Raw Raw anchor A ranging value in mm
 * @param Anchor_A_Ranging_Filtered Filtered anchor A ranging value in mm
 * @param Anchor_B_Ranging_Raw Raw anchor B ranging value in mm
 * @param Anchor_B_Ranging_Filtered Filtered anchor B ranging value in mm
 * @param Anchor_C_Ranging_Raw Raw anchor C ranging value in mm
 * @param Anchor_C_Ranging_Filtered Filtered anchor C ranging value in mm
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mav_shift_radiodog_debug_tag_info_send(mavlink_channel_t chan, uint32_t moduleId, uint16_t radioId, uint8_t moduleType, uint32_t positionTimeStamp, int32_t Anchor_A_Ranging_Raw, int32_t Anchor_A_Ranging_Filtered, int32_t Anchor_B_Ranging_Raw, int32_t Anchor_B_Ranging_Filtered, int32_t Anchor_C_Ranging_Raw, int32_t Anchor_C_Ranging_Filtered)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint32_t(buf, 4, positionTimeStamp);
    _mav_put_int32_t(buf, 8, Anchor_A_Ranging_Raw);
    _mav_put_int32_t(buf, 12, Anchor_A_Ranging_Filtered);
    _mav_put_int32_t(buf, 16, Anchor_B_Ranging_Raw);
    _mav_put_int32_t(buf, 20, Anchor_B_Ranging_Filtered);
    _mav_put_int32_t(buf, 24, Anchor_C_Ranging_Raw);
    _mav_put_int32_t(buf, 28, Anchor_C_Ranging_Filtered);
    _mav_put_uint16_t(buf, 32, radioId);
    _mav_put_uint8_t(buf, 34, moduleType);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_CRC);
#else
    mavlink_mav_shift_radiodog_debug_tag_info_t packet;
    packet.moduleId = moduleId;
    packet.positionTimeStamp = positionTimeStamp;
    packet.Anchor_A_Ranging_Raw = Anchor_A_Ranging_Raw;
    packet.Anchor_A_Ranging_Filtered = Anchor_A_Ranging_Filtered;
    packet.Anchor_B_Ranging_Raw = Anchor_B_Ranging_Raw;
    packet.Anchor_B_Ranging_Filtered = Anchor_B_Ranging_Filtered;
    packet.Anchor_C_Ranging_Raw = Anchor_C_Ranging_Raw;
    packet.Anchor_C_Ranging_Filtered = Anchor_C_Ranging_Filtered;
    packet.radioId = radioId;
    packet.moduleType = moduleType;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO, (const char *)&packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_CRC);
#endif
}

/**
 * @brief Send a mav_shift_radiodog_debug_tag_info message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_mav_shift_radiodog_debug_tag_info_send_struct(mavlink_channel_t chan, const mavlink_mav_shift_radiodog_debug_tag_info_t* mav_shift_radiodog_debug_tag_info)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_mav_shift_radiodog_debug_tag_info_send(chan, mav_shift_radiodog_debug_tag_info->moduleId, mav_shift_radiodog_debug_tag_info->radioId, mav_shift_radiodog_debug_tag_info->moduleType, mav_shift_radiodog_debug_tag_info->positionTimeStamp, mav_shift_radiodog_debug_tag_info->Anchor_A_Ranging_Raw, mav_shift_radiodog_debug_tag_info->Anchor_A_Ranging_Filtered, mav_shift_radiodog_debug_tag_info->Anchor_B_Ranging_Raw, mav_shift_radiodog_debug_tag_info->Anchor_B_Ranging_Filtered, mav_shift_radiodog_debug_tag_info->Anchor_C_Ranging_Raw, mav_shift_radiodog_debug_tag_info->Anchor_C_Ranging_Filtered);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO, (const char *)mav_shift_radiodog_debug_tag_info, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_CRC);
#endif
}

#if MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mav_shift_radiodog_debug_tag_info_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t moduleId, uint16_t radioId, uint8_t moduleType, uint32_t positionTimeStamp, int32_t Anchor_A_Ranging_Raw, int32_t Anchor_A_Ranging_Filtered, int32_t Anchor_B_Ranging_Raw, int32_t Anchor_B_Ranging_Filtered, int32_t Anchor_C_Ranging_Raw, int32_t Anchor_C_Ranging_Filtered)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_uint32_t(buf, 4, positionTimeStamp);
    _mav_put_int32_t(buf, 8, Anchor_A_Ranging_Raw);
    _mav_put_int32_t(buf, 12, Anchor_A_Ranging_Filtered);
    _mav_put_int32_t(buf, 16, Anchor_B_Ranging_Raw);
    _mav_put_int32_t(buf, 20, Anchor_B_Ranging_Filtered);
    _mav_put_int32_t(buf, 24, Anchor_C_Ranging_Raw);
    _mav_put_int32_t(buf, 28, Anchor_C_Ranging_Filtered);
    _mav_put_uint16_t(buf, 32, radioId);
    _mav_put_uint8_t(buf, 34, moduleType);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_CRC);
#else
    mavlink_mav_shift_radiodog_debug_tag_info_t *packet = (mavlink_mav_shift_radiodog_debug_tag_info_t *)msgbuf;
    packet->moduleId = moduleId;
    packet->positionTimeStamp = positionTimeStamp;
    packet->Anchor_A_Ranging_Raw = Anchor_A_Ranging_Raw;
    packet->Anchor_A_Ranging_Filtered = Anchor_A_Ranging_Filtered;
    packet->Anchor_B_Ranging_Raw = Anchor_B_Ranging_Raw;
    packet->Anchor_B_Ranging_Filtered = Anchor_B_Ranging_Filtered;
    packet->Anchor_C_Ranging_Raw = Anchor_C_Ranging_Raw;
    packet->Anchor_C_Ranging_Filtered = Anchor_C_Ranging_Filtered;
    packet->radioId = radioId;
    packet->moduleType = moduleType;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO, (const char *)packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_CRC);
#endif
}
#endif

#endif

// MESSAGE MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO UNPACKING


/**
 * @brief Get field moduleId from mav_shift_radiodog_debug_tag_info message
 *
 * @return RadioDog Module ID (Sending module)
 */
static inline uint32_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_moduleId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field radioId from mav_shift_radiodog_debug_tag_info message
 *
 * @return The RadioDog Radio ID (Sending module)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_radioId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  32);
}

/**
 * @brief Get field moduleType from mav_shift_radiodog_debug_tag_info message
 *
 * @return The Module Type
 */
static inline uint8_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_moduleType(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  34);
}

/**
 * @brief Get field positionTimeStamp from mav_shift_radiodog_debug_tag_info message
 *
 * @return RadioDog Timestamp when the position was calculated
 */
static inline uint32_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_positionTimeStamp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field Anchor_A_Ranging_Raw from mav_shift_radiodog_debug_tag_info message
 *
 * @return Raw anchor A ranging value in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_A_Ranging_Raw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field Anchor_A_Ranging_Filtered from mav_shift_radiodog_debug_tag_info message
 *
 * @return Filtered anchor A ranging value in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_A_Ranging_Filtered(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Get field Anchor_B_Ranging_Raw from mav_shift_radiodog_debug_tag_info message
 *
 * @return Raw anchor B ranging value in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_B_Ranging_Raw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  16);
}

/**
 * @brief Get field Anchor_B_Ranging_Filtered from mav_shift_radiodog_debug_tag_info message
 *
 * @return Filtered anchor B ranging value in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_B_Ranging_Filtered(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  20);
}

/**
 * @brief Get field Anchor_C_Ranging_Raw from mav_shift_radiodog_debug_tag_info message
 *
 * @return Raw anchor C ranging value in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_C_Ranging_Raw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  24);
}

/**
 * @brief Get field Anchor_C_Ranging_Filtered from mav_shift_radiodog_debug_tag_info message
 *
 * @return Filtered anchor C ranging value in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_C_Ranging_Filtered(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  28);
}

/**
 * @brief Decode a mav_shift_radiodog_debug_tag_info message into a struct
 *
 * @param msg The message to decode
 * @param mav_shift_radiodog_debug_tag_info C-struct to decode the message contents into
 */
static inline void mavlink_msg_mav_shift_radiodog_debug_tag_info_decode(const mavlink_message_t* msg, mavlink_mav_shift_radiodog_debug_tag_info_t* mav_shift_radiodog_debug_tag_info)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mav_shift_radiodog_debug_tag_info->moduleId = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_moduleId(msg);
    mav_shift_radiodog_debug_tag_info->positionTimeStamp = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_positionTimeStamp(msg);
    mav_shift_radiodog_debug_tag_info->Anchor_A_Ranging_Raw = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_A_Ranging_Raw(msg);
    mav_shift_radiodog_debug_tag_info->Anchor_A_Ranging_Filtered = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_A_Ranging_Filtered(msg);
    mav_shift_radiodog_debug_tag_info->Anchor_B_Ranging_Raw = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_B_Ranging_Raw(msg);
    mav_shift_radiodog_debug_tag_info->Anchor_B_Ranging_Filtered = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_B_Ranging_Filtered(msg);
    mav_shift_radiodog_debug_tag_info->Anchor_C_Ranging_Raw = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_C_Ranging_Raw(msg);
    mav_shift_radiodog_debug_tag_info->Anchor_C_Ranging_Filtered = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_Anchor_C_Ranging_Filtered(msg);
    mav_shift_radiodog_debug_tag_info->radioId = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_radioId(msg);
    mav_shift_radiodog_debug_tag_info->moduleType = mavlink_msg_mav_shift_radiodog_debug_tag_info_get_moduleType(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN? msg->len : MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN;
        memset(mav_shift_radiodog_debug_tag_info, 0, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_LEN);
    memcpy(mav_shift_radiodog_debug_tag_info, _MAV_PAYLOAD(msg), len);
#endif
}
