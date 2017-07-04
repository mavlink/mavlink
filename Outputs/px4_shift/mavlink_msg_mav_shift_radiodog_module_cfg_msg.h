#pragma once
// MESSAGE MAV_SHIFT_RADIODOG_MODULE_CFG_MSG PACKING

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG 510

MAVPACKED(
typedef struct __mavlink_mav_shift_radiodog_module_cfg_msg_t {
 uint32_t moduleId; /*< RadioDog Module ID (Sending module)*/
 int32_t newXposition; /*< new position X in mm*/
 int32_t newYposition; /*< new position Y in mm*/
 int32_t newZposition; /*< new position Z in mm*/
 uint16_t newRadioId; /*< The new RadioDog Radio ID (Sending module)*/
 uint8_t newModuleType; /*< The new Module Type*/
 uint8_t storeInEeprom; /*< if 0 do not store in eeprom, if > 1 do store in eeprom*/
}) mavlink_mav_shift_radiodog_module_cfg_msg_t;

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN 20
#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN 20
#define MAVLINK_MSG_ID_510_LEN 20
#define MAVLINK_MSG_ID_510_MIN_LEN 20

#define MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_CRC 81
#define MAVLINK_MSG_ID_510_CRC 81



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG { \
    510, \
    "MAV_SHIFT_RADIODOG_MODULE_CFG_MSG", \
    7, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, moduleId) }, \
         { "newXposition", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newXposition) }, \
         { "newYposition", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newYposition) }, \
         { "newZposition", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newZposition) }, \
         { "newRadioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newRadioId) }, \
         { "newModuleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newModuleType) }, \
         { "storeInEeprom", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, storeInEeprom) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG { \
    "MAV_SHIFT_RADIODOG_MODULE_CFG_MSG", \
    7, \
    {  { "moduleId", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, moduleId) }, \
         { "newXposition", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newXposition) }, \
         { "newYposition", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newYposition) }, \
         { "newZposition", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newZposition) }, \
         { "newRadioId", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newRadioId) }, \
         { "newModuleType", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, newModuleType) }, \
         { "storeInEeprom", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_mav_shift_radiodog_module_cfg_msg_t, storeInEeprom) }, \
         } \
}
#endif

/**
 * @brief Pack a mav_shift_radiodog_module_cfg_msg message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param newRadioId The new RadioDog Radio ID (Sending module)
 * @param newModuleType The new Module Type
 * @param newXposition new position X in mm
 * @param newYposition new position Y in mm
 * @param newZposition new position Z in mm
 * @param storeInEeprom if 0 do not store in eeprom, if > 1 do store in eeprom
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t moduleId, uint16_t newRadioId, uint8_t newModuleType, int32_t newXposition, int32_t newYposition, int32_t newZposition, uint8_t storeInEeprom)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_int32_t(buf, 4, newXposition);
    _mav_put_int32_t(buf, 8, newYposition);
    _mav_put_int32_t(buf, 12, newZposition);
    _mav_put_uint16_t(buf, 16, newRadioId);
    _mav_put_uint8_t(buf, 18, newModuleType);
    _mav_put_uint8_t(buf, 19, storeInEeprom);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN);
#else
    mavlink_mav_shift_radiodog_module_cfg_msg_t packet;
    packet.moduleId = moduleId;
    packet.newXposition = newXposition;
    packet.newYposition = newYposition;
    packet.newZposition = newZposition;
    packet.newRadioId = newRadioId;
    packet.newModuleType = newModuleType;
    packet.storeInEeprom = storeInEeprom;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_CRC);
}

/**
 * @brief Pack a mav_shift_radiodog_module_cfg_msg message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param moduleId RadioDog Module ID (Sending module)
 * @param newRadioId The new RadioDog Radio ID (Sending module)
 * @param newModuleType The new Module Type
 * @param newXposition new position X in mm
 * @param newYposition new position Y in mm
 * @param newZposition new position Z in mm
 * @param storeInEeprom if 0 do not store in eeprom, if > 1 do store in eeprom
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t moduleId,uint16_t newRadioId,uint8_t newModuleType,int32_t newXposition,int32_t newYposition,int32_t newZposition,uint8_t storeInEeprom)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_int32_t(buf, 4, newXposition);
    _mav_put_int32_t(buf, 8, newYposition);
    _mav_put_int32_t(buf, 12, newZposition);
    _mav_put_uint16_t(buf, 16, newRadioId);
    _mav_put_uint8_t(buf, 18, newModuleType);
    _mav_put_uint8_t(buf, 19, storeInEeprom);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN);
#else
    mavlink_mav_shift_radiodog_module_cfg_msg_t packet;
    packet.moduleId = moduleId;
    packet.newXposition = newXposition;
    packet.newYposition = newYposition;
    packet.newZposition = newZposition;
    packet.newRadioId = newRadioId;
    packet.newModuleType = newModuleType;
    packet.storeInEeprom = storeInEeprom;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_CRC);
}

/**
 * @brief Encode a mav_shift_radiodog_module_cfg_msg struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_module_cfg_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_module_cfg_msg_t* mav_shift_radiodog_module_cfg_msg)
{
    return mavlink_msg_mav_shift_radiodog_module_cfg_msg_pack(system_id, component_id, msg, mav_shift_radiodog_module_cfg_msg->moduleId, mav_shift_radiodog_module_cfg_msg->newRadioId, mav_shift_radiodog_module_cfg_msg->newModuleType, mav_shift_radiodog_module_cfg_msg->newXposition, mav_shift_radiodog_module_cfg_msg->newYposition, mav_shift_radiodog_module_cfg_msg->newZposition, mav_shift_radiodog_module_cfg_msg->storeInEeprom);
}

/**
 * @brief Encode a mav_shift_radiodog_module_cfg_msg struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mav_shift_radiodog_module_cfg_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mav_shift_radiodog_module_cfg_msg_t* mav_shift_radiodog_module_cfg_msg)
{
    return mavlink_msg_mav_shift_radiodog_module_cfg_msg_pack_chan(system_id, component_id, chan, msg, mav_shift_radiodog_module_cfg_msg->moduleId, mav_shift_radiodog_module_cfg_msg->newRadioId, mav_shift_radiodog_module_cfg_msg->newModuleType, mav_shift_radiodog_module_cfg_msg->newXposition, mav_shift_radiodog_module_cfg_msg->newYposition, mav_shift_radiodog_module_cfg_msg->newZposition, mav_shift_radiodog_module_cfg_msg->storeInEeprom);
}

/**
 * @brief Send a mav_shift_radiodog_module_cfg_msg message
 * @param chan MAVLink channel to send the message
 *
 * @param moduleId RadioDog Module ID (Sending module)
 * @param newRadioId The new RadioDog Radio ID (Sending module)
 * @param newModuleType The new Module Type
 * @param newXposition new position X in mm
 * @param newYposition new position Y in mm
 * @param newZposition new position Z in mm
 * @param storeInEeprom if 0 do not store in eeprom, if > 1 do store in eeprom
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mav_shift_radiodog_module_cfg_msg_send(mavlink_channel_t chan, uint32_t moduleId, uint16_t newRadioId, uint8_t newModuleType, int32_t newXposition, int32_t newYposition, int32_t newZposition, uint8_t storeInEeprom)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN];
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_int32_t(buf, 4, newXposition);
    _mav_put_int32_t(buf, 8, newYposition);
    _mav_put_int32_t(buf, 12, newZposition);
    _mav_put_uint16_t(buf, 16, newRadioId);
    _mav_put_uint8_t(buf, 18, newModuleType);
    _mav_put_uint8_t(buf, 19, storeInEeprom);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_CRC);
#else
    mavlink_mav_shift_radiodog_module_cfg_msg_t packet;
    packet.moduleId = moduleId;
    packet.newXposition = newXposition;
    packet.newYposition = newYposition;
    packet.newZposition = newZposition;
    packet.newRadioId = newRadioId;
    packet.newModuleType = newModuleType;
    packet.storeInEeprom = storeInEeprom;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG, (const char *)&packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_CRC);
#endif
}

/**
 * @brief Send a mav_shift_radiodog_module_cfg_msg message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_mav_shift_radiodog_module_cfg_msg_send_struct(mavlink_channel_t chan, const mavlink_mav_shift_radiodog_module_cfg_msg_t* mav_shift_radiodog_module_cfg_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_send(chan, mav_shift_radiodog_module_cfg_msg->moduleId, mav_shift_radiodog_module_cfg_msg->newRadioId, mav_shift_radiodog_module_cfg_msg->newModuleType, mav_shift_radiodog_module_cfg_msg->newXposition, mav_shift_radiodog_module_cfg_msg->newYposition, mav_shift_radiodog_module_cfg_msg->newZposition, mav_shift_radiodog_module_cfg_msg->storeInEeprom);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG, (const char *)mav_shift_radiodog_module_cfg_msg, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_CRC);
#endif
}

#if MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mav_shift_radiodog_module_cfg_msg_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t moduleId, uint16_t newRadioId, uint8_t newModuleType, int32_t newXposition, int32_t newYposition, int32_t newZposition, uint8_t storeInEeprom)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, moduleId);
    _mav_put_int32_t(buf, 4, newXposition);
    _mav_put_int32_t(buf, 8, newYposition);
    _mav_put_int32_t(buf, 12, newZposition);
    _mav_put_uint16_t(buf, 16, newRadioId);
    _mav_put_uint8_t(buf, 18, newModuleType);
    _mav_put_uint8_t(buf, 19, storeInEeprom);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG, buf, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_CRC);
#else
    mavlink_mav_shift_radiodog_module_cfg_msg_t *packet = (mavlink_mav_shift_radiodog_module_cfg_msg_t *)msgbuf;
    packet->moduleId = moduleId;
    packet->newXposition = newXposition;
    packet->newYposition = newYposition;
    packet->newZposition = newZposition;
    packet->newRadioId = newRadioId;
    packet->newModuleType = newModuleType;
    packet->storeInEeprom = storeInEeprom;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG, (const char *)packet, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_CRC);
#endif
}
#endif

#endif

// MESSAGE MAV_SHIFT_RADIODOG_MODULE_CFG_MSG UNPACKING


/**
 * @brief Get field moduleId from mav_shift_radiodog_module_cfg_msg message
 *
 * @return RadioDog Module ID (Sending module)
 */
static inline uint32_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_moduleId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field newRadioId from mav_shift_radiodog_module_cfg_msg message
 *
 * @return The new RadioDog Radio ID (Sending module)
 */
static inline uint16_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newRadioId(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  16);
}

/**
 * @brief Get field newModuleType from mav_shift_radiodog_module_cfg_msg message
 *
 * @return The new Module Type
 */
static inline uint8_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newModuleType(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field newXposition from mav_shift_radiodog_module_cfg_msg message
 *
 * @return new position X in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newXposition(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field newYposition from mav_shift_radiodog_module_cfg_msg message
 *
 * @return new position Y in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newYposition(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field newZposition from mav_shift_radiodog_module_cfg_msg message
 *
 * @return new position Z in mm
 */
static inline int32_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newZposition(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Get field storeInEeprom from mav_shift_radiodog_module_cfg_msg message
 *
 * @return if 0 do not store in eeprom, if > 1 do store in eeprom
 */
static inline uint8_t mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_storeInEeprom(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Decode a mav_shift_radiodog_module_cfg_msg message into a struct
 *
 * @param msg The message to decode
 * @param mav_shift_radiodog_module_cfg_msg C-struct to decode the message contents into
 */
static inline void mavlink_msg_mav_shift_radiodog_module_cfg_msg_decode(const mavlink_message_t* msg, mavlink_mav_shift_radiodog_module_cfg_msg_t* mav_shift_radiodog_module_cfg_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mav_shift_radiodog_module_cfg_msg->moduleId = mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_moduleId(msg);
    mav_shift_radiodog_module_cfg_msg->newXposition = mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newXposition(msg);
    mav_shift_radiodog_module_cfg_msg->newYposition = mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newYposition(msg);
    mav_shift_radiodog_module_cfg_msg->newZposition = mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newZposition(msg);
    mav_shift_radiodog_module_cfg_msg->newRadioId = mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newRadioId(msg);
    mav_shift_radiodog_module_cfg_msg->newModuleType = mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_newModuleType(msg);
    mav_shift_radiodog_module_cfg_msg->storeInEeprom = mavlink_msg_mav_shift_radiodog_module_cfg_msg_get_storeInEeprom(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN? msg->len : MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN;
        memset(mav_shift_radiodog_module_cfg_msg, 0, MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_LEN);
    memcpy(mav_shift_radiodog_module_cfg_msg, _MAV_PAYLOAD(msg), len);
#endif
}
