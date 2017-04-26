#pragma once
// MESSAGE SHIFT_SYSTEM_CMD PACKING

#define MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD 570

MAVPACKED(
typedef struct __mavlink_shift_system_cmd_t {
 uint16_t cmd1; /*< cmd1 field*/
 uint16_t cmd2; /*< cmd2 field*/
 uint16_t cmd3; /*< cmd3 field*/
 uint16_t cmd4; /*< cmd4 field*/
 uint16_t cmd5; /*< cmd5 field*/
 uint16_t cmd6; /*< cmd6 field*/
 uint8_t type; /*< type field*/
}) mavlink_shift_system_cmd_t;

#define MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN 13
#define MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN 13
#define MAVLINK_MSG_ID_570_LEN 13
#define MAVLINK_MSG_ID_570_MIN_LEN 13

#define MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_CRC 32
#define MAVLINK_MSG_ID_570_CRC 32



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SHIFT_SYSTEM_CMD { \
    570, \
    "SHIFT_SYSTEM_CMD", \
    7, \
    {  { "cmd1", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_shift_system_cmd_t, cmd1) }, \
         { "cmd2", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_shift_system_cmd_t, cmd2) }, \
         { "cmd3", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_shift_system_cmd_t, cmd3) }, \
         { "cmd4", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_shift_system_cmd_t, cmd4) }, \
         { "cmd5", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_shift_system_cmd_t, cmd5) }, \
         { "cmd6", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_shift_system_cmd_t, cmd6) }, \
         { "type", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_shift_system_cmd_t, type) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SHIFT_SYSTEM_CMD { \
    "SHIFT_SYSTEM_CMD", \
    7, \
    {  { "cmd1", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_shift_system_cmd_t, cmd1) }, \
         { "cmd2", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_shift_system_cmd_t, cmd2) }, \
         { "cmd3", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_shift_system_cmd_t, cmd3) }, \
         { "cmd4", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_shift_system_cmd_t, cmd4) }, \
         { "cmd5", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_shift_system_cmd_t, cmd5) }, \
         { "cmd6", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_shift_system_cmd_t, cmd6) }, \
         { "type", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_shift_system_cmd_t, type) }, \
         } \
}
#endif

/**
 * @brief Pack a shift_system_cmd message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param cmd1 cmd1 field
 * @param cmd2 cmd2 field
 * @param cmd3 cmd3 field
 * @param cmd4 cmd4 field
 * @param cmd5 cmd5 field
 * @param cmd6 cmd6 field
 * @param type type field
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_system_cmd_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint16_t cmd1, uint16_t cmd2, uint16_t cmd3, uint16_t cmd4, uint16_t cmd5, uint16_t cmd6, uint8_t type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN];
    _mav_put_uint16_t(buf, 0, cmd1);
    _mav_put_uint16_t(buf, 2, cmd2);
    _mav_put_uint16_t(buf, 4, cmd3);
    _mav_put_uint16_t(buf, 6, cmd4);
    _mav_put_uint16_t(buf, 8, cmd5);
    _mav_put_uint16_t(buf, 10, cmd6);
    _mav_put_uint8_t(buf, 12, type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN);
#else
    mavlink_shift_system_cmd_t packet;
    packet.cmd1 = cmd1;
    packet.cmd2 = cmd2;
    packet.cmd3 = cmd3;
    packet.cmd4 = cmd4;
    packet.cmd5 = cmd5;
    packet.cmd6 = cmd6;
    packet.type = type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_CRC);
}

/**
 * @brief Pack a shift_system_cmd message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cmd1 cmd1 field
 * @param cmd2 cmd2 field
 * @param cmd3 cmd3 field
 * @param cmd4 cmd4 field
 * @param cmd5 cmd5 field
 * @param cmd6 cmd6 field
 * @param type type field
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_system_cmd_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint16_t cmd1,uint16_t cmd2,uint16_t cmd3,uint16_t cmd4,uint16_t cmd5,uint16_t cmd6,uint8_t type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN];
    _mav_put_uint16_t(buf, 0, cmd1);
    _mav_put_uint16_t(buf, 2, cmd2);
    _mav_put_uint16_t(buf, 4, cmd3);
    _mav_put_uint16_t(buf, 6, cmd4);
    _mav_put_uint16_t(buf, 8, cmd5);
    _mav_put_uint16_t(buf, 10, cmd6);
    _mav_put_uint8_t(buf, 12, type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN);
#else
    mavlink_shift_system_cmd_t packet;
    packet.cmd1 = cmd1;
    packet.cmd2 = cmd2;
    packet.cmd3 = cmd3;
    packet.cmd4 = cmd4;
    packet.cmd5 = cmd5;
    packet.cmd6 = cmd6;
    packet.type = type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_CRC);
}

/**
 * @brief Encode a shift_system_cmd struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param shift_system_cmd C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_system_cmd_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_shift_system_cmd_t* shift_system_cmd)
{
    return mavlink_msg_shift_system_cmd_pack(system_id, component_id, msg, shift_system_cmd->cmd1, shift_system_cmd->cmd2, shift_system_cmd->cmd3, shift_system_cmd->cmd4, shift_system_cmd->cmd5, shift_system_cmd->cmd6, shift_system_cmd->type);
}

/**
 * @brief Encode a shift_system_cmd struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param shift_system_cmd C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_system_cmd_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_shift_system_cmd_t* shift_system_cmd)
{
    return mavlink_msg_shift_system_cmd_pack_chan(system_id, component_id, chan, msg, shift_system_cmd->cmd1, shift_system_cmd->cmd2, shift_system_cmd->cmd3, shift_system_cmd->cmd4, shift_system_cmd->cmd5, shift_system_cmd->cmd6, shift_system_cmd->type);
}

/**
 * @brief Send a shift_system_cmd message
 * @param chan MAVLink channel to send the message
 *
 * @param cmd1 cmd1 field
 * @param cmd2 cmd2 field
 * @param cmd3 cmd3 field
 * @param cmd4 cmd4 field
 * @param cmd5 cmd5 field
 * @param cmd6 cmd6 field
 * @param type type field
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_shift_system_cmd_send(mavlink_channel_t chan, uint16_t cmd1, uint16_t cmd2, uint16_t cmd3, uint16_t cmd4, uint16_t cmd5, uint16_t cmd6, uint8_t type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN];
    _mav_put_uint16_t(buf, 0, cmd1);
    _mav_put_uint16_t(buf, 2, cmd2);
    _mav_put_uint16_t(buf, 4, cmd3);
    _mav_put_uint16_t(buf, 6, cmd4);
    _mav_put_uint16_t(buf, 8, cmd5);
    _mav_put_uint16_t(buf, 10, cmd6);
    _mav_put_uint8_t(buf, 12, type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD, buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_CRC);
#else
    mavlink_shift_system_cmd_t packet;
    packet.cmd1 = cmd1;
    packet.cmd2 = cmd2;
    packet.cmd3 = cmd3;
    packet.cmd4 = cmd4;
    packet.cmd5 = cmd5;
    packet.cmd6 = cmd6;
    packet.type = type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD, (const char *)&packet, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_CRC);
#endif
}

/**
 * @brief Send a shift_system_cmd message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_shift_system_cmd_send_struct(mavlink_channel_t chan, const mavlink_shift_system_cmd_t* shift_system_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_shift_system_cmd_send(chan, shift_system_cmd->cmd1, shift_system_cmd->cmd2, shift_system_cmd->cmd3, shift_system_cmd->cmd4, shift_system_cmd->cmd5, shift_system_cmd->cmd6, shift_system_cmd->type);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD, (const char *)shift_system_cmd, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_CRC);
#endif
}

#if MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_shift_system_cmd_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint16_t cmd1, uint16_t cmd2, uint16_t cmd3, uint16_t cmd4, uint16_t cmd5, uint16_t cmd6, uint8_t type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, cmd1);
    _mav_put_uint16_t(buf, 2, cmd2);
    _mav_put_uint16_t(buf, 4, cmd3);
    _mav_put_uint16_t(buf, 6, cmd4);
    _mav_put_uint16_t(buf, 8, cmd5);
    _mav_put_uint16_t(buf, 10, cmd6);
    _mav_put_uint8_t(buf, 12, type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD, buf, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_CRC);
#else
    mavlink_shift_system_cmd_t *packet = (mavlink_shift_system_cmd_t *)msgbuf;
    packet->cmd1 = cmd1;
    packet->cmd2 = cmd2;
    packet->cmd3 = cmd3;
    packet->cmd4 = cmd4;
    packet->cmd5 = cmd5;
    packet->cmd6 = cmd6;
    packet->type = type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD, (const char *)packet, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_CRC);
#endif
}
#endif

#endif

// MESSAGE SHIFT_SYSTEM_CMD UNPACKING


/**
 * @brief Get field cmd1 from shift_system_cmd message
 *
 * @return cmd1 field
 */
static inline uint16_t mavlink_msg_shift_system_cmd_get_cmd1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field cmd2 from shift_system_cmd message
 *
 * @return cmd2 field
 */
static inline uint16_t mavlink_msg_shift_system_cmd_get_cmd2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Get field cmd3 from shift_system_cmd message
 *
 * @return cmd3 field
 */
static inline uint16_t mavlink_msg_shift_system_cmd_get_cmd3(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  4);
}

/**
 * @brief Get field cmd4 from shift_system_cmd message
 *
 * @return cmd4 field
 */
static inline uint16_t mavlink_msg_shift_system_cmd_get_cmd4(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  6);
}

/**
 * @brief Get field cmd5 from shift_system_cmd message
 *
 * @return cmd5 field
 */
static inline uint16_t mavlink_msg_shift_system_cmd_get_cmd5(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  8);
}

/**
 * @brief Get field cmd6 from shift_system_cmd message
 *
 * @return cmd6 field
 */
static inline uint16_t mavlink_msg_shift_system_cmd_get_cmd6(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  10);
}

/**
 * @brief Get field type from shift_system_cmd message
 *
 * @return type field
 */
static inline uint8_t mavlink_msg_shift_system_cmd_get_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Decode a shift_system_cmd message into a struct
 *
 * @param msg The message to decode
 * @param shift_system_cmd C-struct to decode the message contents into
 */
static inline void mavlink_msg_shift_system_cmd_decode(const mavlink_message_t* msg, mavlink_shift_system_cmd_t* shift_system_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    shift_system_cmd->cmd1 = mavlink_msg_shift_system_cmd_get_cmd1(msg);
    shift_system_cmd->cmd2 = mavlink_msg_shift_system_cmd_get_cmd2(msg);
    shift_system_cmd->cmd3 = mavlink_msg_shift_system_cmd_get_cmd3(msg);
    shift_system_cmd->cmd4 = mavlink_msg_shift_system_cmd_get_cmd4(msg);
    shift_system_cmd->cmd5 = mavlink_msg_shift_system_cmd_get_cmd5(msg);
    shift_system_cmd->cmd6 = mavlink_msg_shift_system_cmd_get_cmd6(msg);
    shift_system_cmd->type = mavlink_msg_shift_system_cmd_get_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN? msg->len : MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN;
        memset(shift_system_cmd, 0, MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_LEN);
    memcpy(shift_system_cmd, _MAV_PAYLOAD(msg), len);
#endif
}
