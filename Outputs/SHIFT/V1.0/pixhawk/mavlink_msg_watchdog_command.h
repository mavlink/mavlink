#pragma once
// MESSAGE WATCHDOG_COMMAND PACKING

#define MAVLINK_MSG_ID_WATCHDOG_COMMAND 183

MAVPACKED(
typedef struct __mavlink_watchdog_command_t {
 uint16_t watchdog_id; /*< Watchdog ID*/
 uint16_t process_id; /*< Process ID*/
 uint8_t target_system_id; /*< Target system ID*/
 uint8_t command_id; /*< Command ID*/
}) mavlink_watchdog_command_t;

#define MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN 6
#define MAVLINK_MSG_ID_WATCHDOG_COMMAND_MIN_LEN 6
#define MAVLINK_MSG_ID_183_LEN 6
#define MAVLINK_MSG_ID_183_MIN_LEN 6

#define MAVLINK_MSG_ID_WATCHDOG_COMMAND_CRC 162
#define MAVLINK_MSG_ID_183_CRC 162



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_WATCHDOG_COMMAND { \
    183, \
    "WATCHDOG_COMMAND", \
    4, \
    {  { "watchdog_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_watchdog_command_t, watchdog_id) }, \
         { "process_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_watchdog_command_t, process_id) }, \
         { "target_system_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_watchdog_command_t, target_system_id) }, \
         { "command_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_watchdog_command_t, command_id) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_WATCHDOG_COMMAND { \
    "WATCHDOG_COMMAND", \
    4, \
    {  { "watchdog_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_watchdog_command_t, watchdog_id) }, \
         { "process_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_watchdog_command_t, process_id) }, \
         { "target_system_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_watchdog_command_t, target_system_id) }, \
         { "command_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_watchdog_command_t, command_id) }, \
         } \
}
#endif

/**
 * @brief Pack a watchdog_command message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system_id Target system ID
 * @param watchdog_id Watchdog ID
 * @param process_id Process ID
 * @param command_id Command ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_watchdog_command_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system_id, uint16_t watchdog_id, uint16_t process_id, uint8_t command_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN];
    _mav_put_uint16_t(buf, 0, watchdog_id);
    _mav_put_uint16_t(buf, 2, process_id);
    _mav_put_uint8_t(buf, 4, target_system_id);
    _mav_put_uint8_t(buf, 5, command_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN);
#else
    mavlink_watchdog_command_t packet;
    packet.watchdog_id = watchdog_id;
    packet.process_id = process_id;
    packet.target_system_id = target_system_id;
    packet.command_id = command_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WATCHDOG_COMMAND;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_WATCHDOG_COMMAND_MIN_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_CRC);
}

/**
 * @brief Pack a watchdog_command message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system_id Target system ID
 * @param watchdog_id Watchdog ID
 * @param process_id Process ID
 * @param command_id Command ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_watchdog_command_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system_id,uint16_t watchdog_id,uint16_t process_id,uint8_t command_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN];
    _mav_put_uint16_t(buf, 0, watchdog_id);
    _mav_put_uint16_t(buf, 2, process_id);
    _mav_put_uint8_t(buf, 4, target_system_id);
    _mav_put_uint8_t(buf, 5, command_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN);
#else
    mavlink_watchdog_command_t packet;
    packet.watchdog_id = watchdog_id;
    packet.process_id = process_id;
    packet.target_system_id = target_system_id;
    packet.command_id = command_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WATCHDOG_COMMAND;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_WATCHDOG_COMMAND_MIN_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_CRC);
}

/**
 * @brief Encode a watchdog_command struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param watchdog_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_watchdog_command_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_watchdog_command_t* watchdog_command)
{
    return mavlink_msg_watchdog_command_pack(system_id, component_id, msg, watchdog_command->target_system_id, watchdog_command->watchdog_id, watchdog_command->process_id, watchdog_command->command_id);
}

/**
 * @brief Encode a watchdog_command struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param watchdog_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_watchdog_command_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_watchdog_command_t* watchdog_command)
{
    return mavlink_msg_watchdog_command_pack_chan(system_id, component_id, chan, msg, watchdog_command->target_system_id, watchdog_command->watchdog_id, watchdog_command->process_id, watchdog_command->command_id);
}

/**
 * @brief Send a watchdog_command message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system_id Target system ID
 * @param watchdog_id Watchdog ID
 * @param process_id Process ID
 * @param command_id Command ID
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_watchdog_command_send(mavlink_channel_t chan, uint8_t target_system_id, uint16_t watchdog_id, uint16_t process_id, uint8_t command_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN];
    _mav_put_uint16_t(buf, 0, watchdog_id);
    _mav_put_uint16_t(buf, 2, process_id);
    _mav_put_uint8_t(buf, 4, target_system_id);
    _mav_put_uint8_t(buf, 5, command_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WATCHDOG_COMMAND, buf, MAVLINK_MSG_ID_WATCHDOG_COMMAND_MIN_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_CRC);
#else
    mavlink_watchdog_command_t packet;
    packet.watchdog_id = watchdog_id;
    packet.process_id = process_id;
    packet.target_system_id = target_system_id;
    packet.command_id = command_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WATCHDOG_COMMAND, (const char *)&packet, MAVLINK_MSG_ID_WATCHDOG_COMMAND_MIN_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_CRC);
#endif
}

/**
 * @brief Send a watchdog_command message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_watchdog_command_send_struct(mavlink_channel_t chan, const mavlink_watchdog_command_t* watchdog_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_watchdog_command_send(chan, watchdog_command->target_system_id, watchdog_command->watchdog_id, watchdog_command->process_id, watchdog_command->command_id);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WATCHDOG_COMMAND, (const char *)watchdog_command, MAVLINK_MSG_ID_WATCHDOG_COMMAND_MIN_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_CRC);
#endif
}

#if MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_watchdog_command_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system_id, uint16_t watchdog_id, uint16_t process_id, uint8_t command_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, watchdog_id);
    _mav_put_uint16_t(buf, 2, process_id);
    _mav_put_uint8_t(buf, 4, target_system_id);
    _mav_put_uint8_t(buf, 5, command_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WATCHDOG_COMMAND, buf, MAVLINK_MSG_ID_WATCHDOG_COMMAND_MIN_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_CRC);
#else
    mavlink_watchdog_command_t *packet = (mavlink_watchdog_command_t *)msgbuf;
    packet->watchdog_id = watchdog_id;
    packet->process_id = process_id;
    packet->target_system_id = target_system_id;
    packet->command_id = command_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WATCHDOG_COMMAND, (const char *)packet, MAVLINK_MSG_ID_WATCHDOG_COMMAND_MIN_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN, MAVLINK_MSG_ID_WATCHDOG_COMMAND_CRC);
#endif
}
#endif

#endif

// MESSAGE WATCHDOG_COMMAND UNPACKING


/**
 * @brief Get field target_system_id from watchdog_command message
 *
 * @return Target system ID
 */
static inline uint8_t mavlink_msg_watchdog_command_get_target_system_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field watchdog_id from watchdog_command message
 *
 * @return Watchdog ID
 */
static inline uint16_t mavlink_msg_watchdog_command_get_watchdog_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field process_id from watchdog_command message
 *
 * @return Process ID
 */
static inline uint16_t mavlink_msg_watchdog_command_get_process_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Get field command_id from watchdog_command message
 *
 * @return Command ID
 */
static inline uint8_t mavlink_msg_watchdog_command_get_command_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Decode a watchdog_command message into a struct
 *
 * @param msg The message to decode
 * @param watchdog_command C-struct to decode the message contents into
 */
static inline void mavlink_msg_watchdog_command_decode(const mavlink_message_t* msg, mavlink_watchdog_command_t* watchdog_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    watchdog_command->watchdog_id = mavlink_msg_watchdog_command_get_watchdog_id(msg);
    watchdog_command->process_id = mavlink_msg_watchdog_command_get_process_id(msg);
    watchdog_command->target_system_id = mavlink_msg_watchdog_command_get_target_system_id(msg);
    watchdog_command->command_id = mavlink_msg_watchdog_command_get_command_id(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN? msg->len : MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN;
        memset(watchdog_command, 0, MAVLINK_MSG_ID_WATCHDOG_COMMAND_LEN);
    memcpy(watchdog_command, _MAV_PAYLOAD(msg), len);
#endif
}
