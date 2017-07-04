#pragma once
// MESSAGE SHIFT_NAVIGATOR_CMD PACKING

#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD 551

MAVPACKED(
typedef struct __mavlink_shift_navigator_cmd_t {
 uint64_t time_usec; /*< Timestamp from the SHIFT PC*/
 uint32_t pathIndex; /*< Index of the path which is affected by this command*/
 uint8_t target_system; /*< System ID*/
 uint8_t cmd; /*< The command itself. Using ENUM: SHIFT_NAVIGATOR_CMD_ENUM*/
}) mavlink_shift_navigator_cmd_t;

#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN 14
#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN 14
#define MAVLINK_MSG_ID_551_LEN 14
#define MAVLINK_MSG_ID_551_MIN_LEN 14

#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_CRC 39
#define MAVLINK_MSG_ID_551_CRC 39



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SHIFT_NAVIGATOR_CMD { \
    551, \
    "SHIFT_NAVIGATOR_CMD", \
    4, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_shift_navigator_cmd_t, time_usec) }, \
         { "pathIndex", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_shift_navigator_cmd_t, pathIndex) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_shift_navigator_cmd_t, target_system) }, \
         { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_shift_navigator_cmd_t, cmd) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SHIFT_NAVIGATOR_CMD { \
    "SHIFT_NAVIGATOR_CMD", \
    4, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_shift_navigator_cmd_t, time_usec) }, \
         { "pathIndex", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_shift_navigator_cmd_t, pathIndex) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_shift_navigator_cmd_t, target_system) }, \
         { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_shift_navigator_cmd_t, cmd) }, \
         } \
}
#endif

/**
 * @brief Pack a shift_navigator_cmd message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param time_usec Timestamp from the SHIFT PC
 * @param pathIndex Index of the path which is affected by this command
 * @param cmd The command itself. Using ENUM: SHIFT_NAVIGATOR_CMD_ENUM
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_navigator_cmd_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint64_t time_usec, uint32_t pathIndex, uint8_t cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_uint32_t(buf, 8, pathIndex);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN);
#else
    mavlink_shift_navigator_cmd_t packet;
    packet.time_usec = time_usec;
    packet.pathIndex = pathIndex;
    packet.target_system = target_system;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_CRC);
}

/**
 * @brief Pack a shift_navigator_cmd message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param time_usec Timestamp from the SHIFT PC
 * @param pathIndex Index of the path which is affected by this command
 * @param cmd The command itself. Using ENUM: SHIFT_NAVIGATOR_CMD_ENUM
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_navigator_cmd_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint64_t time_usec,uint32_t pathIndex,uint8_t cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_uint32_t(buf, 8, pathIndex);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN);
#else
    mavlink_shift_navigator_cmd_t packet;
    packet.time_usec = time_usec;
    packet.pathIndex = pathIndex;
    packet.target_system = target_system;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_CRC);
}

/**
 * @brief Encode a shift_navigator_cmd struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param shift_navigator_cmd C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_navigator_cmd_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_shift_navigator_cmd_t* shift_navigator_cmd)
{
    return mavlink_msg_shift_navigator_cmd_pack(system_id, component_id, msg, shift_navigator_cmd->target_system, shift_navigator_cmd->time_usec, shift_navigator_cmd->pathIndex, shift_navigator_cmd->cmd);
}

/**
 * @brief Encode a shift_navigator_cmd struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param shift_navigator_cmd C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_navigator_cmd_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_shift_navigator_cmd_t* shift_navigator_cmd)
{
    return mavlink_msg_shift_navigator_cmd_pack_chan(system_id, component_id, chan, msg, shift_navigator_cmd->target_system, shift_navigator_cmd->time_usec, shift_navigator_cmd->pathIndex, shift_navigator_cmd->cmd);
}

/**
 * @brief Send a shift_navigator_cmd message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param time_usec Timestamp from the SHIFT PC
 * @param pathIndex Index of the path which is affected by this command
 * @param cmd The command itself. Using ENUM: SHIFT_NAVIGATOR_CMD_ENUM
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_shift_navigator_cmd_send(mavlink_channel_t chan, uint8_t target_system, uint64_t time_usec, uint32_t pathIndex, uint8_t cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_uint32_t(buf, 8, pathIndex);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD, buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_CRC);
#else
    mavlink_shift_navigator_cmd_t packet;
    packet.time_usec = time_usec;
    packet.pathIndex = pathIndex;
    packet.target_system = target_system;
    packet.cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD, (const char *)&packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_CRC);
#endif
}

/**
 * @brief Send a shift_navigator_cmd message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_shift_navigator_cmd_send_struct(mavlink_channel_t chan, const mavlink_shift_navigator_cmd_t* shift_navigator_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_shift_navigator_cmd_send(chan, shift_navigator_cmd->target_system, shift_navigator_cmd->time_usec, shift_navigator_cmd->pathIndex, shift_navigator_cmd->cmd);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD, (const char *)shift_navigator_cmd, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_CRC);
#endif
}

#if MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_shift_navigator_cmd_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint64_t time_usec, uint32_t pathIndex, uint8_t cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_uint32_t(buf, 8, pathIndex);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD, buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_CRC);
#else
    mavlink_shift_navigator_cmd_t *packet = (mavlink_shift_navigator_cmd_t *)msgbuf;
    packet->time_usec = time_usec;
    packet->pathIndex = pathIndex;
    packet->target_system = target_system;
    packet->cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD, (const char *)packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_CRC);
#endif
}
#endif

#endif

// MESSAGE SHIFT_NAVIGATOR_CMD UNPACKING


/**
 * @brief Get field target_system from shift_navigator_cmd message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_shift_navigator_cmd_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field time_usec from shift_navigator_cmd message
 *
 * @return Timestamp from the SHIFT PC
 */
static inline uint64_t mavlink_msg_shift_navigator_cmd_get_time_usec(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field pathIndex from shift_navigator_cmd message
 *
 * @return Index of the path which is affected by this command
 */
static inline uint32_t mavlink_msg_shift_navigator_cmd_get_pathIndex(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field cmd from shift_navigator_cmd message
 *
 * @return The command itself. Using ENUM: SHIFT_NAVIGATOR_CMD_ENUM
 */
static inline uint8_t mavlink_msg_shift_navigator_cmd_get_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  13);
}

/**
 * @brief Decode a shift_navigator_cmd message into a struct
 *
 * @param msg The message to decode
 * @param shift_navigator_cmd C-struct to decode the message contents into
 */
static inline void mavlink_msg_shift_navigator_cmd_decode(const mavlink_message_t* msg, mavlink_shift_navigator_cmd_t* shift_navigator_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    shift_navigator_cmd->time_usec = mavlink_msg_shift_navigator_cmd_get_time_usec(msg);
    shift_navigator_cmd->pathIndex = mavlink_msg_shift_navigator_cmd_get_pathIndex(msg);
    shift_navigator_cmd->target_system = mavlink_msg_shift_navigator_cmd_get_target_system(msg);
    shift_navigator_cmd->cmd = mavlink_msg_shift_navigator_cmd_get_cmd(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN? msg->len : MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN;
        memset(shift_navigator_cmd, 0, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_LEN);
    memcpy(shift_navigator_cmd, _MAV_PAYLOAD(msg), len);
#endif
}
