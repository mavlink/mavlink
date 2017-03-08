#pragma once
// MESSAGE VOLIRO_COMMAND PACKING

#define MAVLINK_MSG_ID_VOLIRO_COMMAND 269

MAVPACKED(
typedef struct __mavlink_voliro_command_t {
 float alpha[6]; /*< tiltingAngles*/
 float omega[6]; /*< motorSpeeds*/
}) mavlink_voliro_command_t;

#define MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN 48
#define MAVLINK_MSG_ID_VOLIRO_COMMAND_MIN_LEN 48
#define MAVLINK_MSG_ID_269_LEN 48
#define MAVLINK_MSG_ID_269_MIN_LEN 48

#define MAVLINK_MSG_ID_VOLIRO_COMMAND_CRC 8
#define MAVLINK_MSG_ID_269_CRC 8

#define MAVLINK_MSG_VOLIRO_COMMAND_FIELD_ALPHA_LEN 6
#define MAVLINK_MSG_VOLIRO_COMMAND_FIELD_OMEGA_LEN 6

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_VOLIRO_COMMAND { \
    269, \
    "VOLIRO_COMMAND", \
    2, \
    {  { "alpha", NULL, MAVLINK_TYPE_FLOAT, 6, 0, offsetof(mavlink_voliro_command_t, alpha) }, \
         { "omega", NULL, MAVLINK_TYPE_FLOAT, 6, 24, offsetof(mavlink_voliro_command_t, omega) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_VOLIRO_COMMAND { \
    "VOLIRO_COMMAND", \
    2, \
    {  { "alpha", NULL, MAVLINK_TYPE_FLOAT, 6, 0, offsetof(mavlink_voliro_command_t, alpha) }, \
         { "omega", NULL, MAVLINK_TYPE_FLOAT, 6, 24, offsetof(mavlink_voliro_command_t, omega) }, \
         } \
}
#endif

/**
 * @brief Pack a voliro_command message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param alpha tiltingAngles
 * @param omega motorSpeeds
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_voliro_command_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const float *alpha, const float *omega)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN];

    _mav_put_float_array(buf, 0, alpha, 6);
    _mav_put_float_array(buf, 24, omega, 6);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN);
#else
    mavlink_voliro_command_t packet;

    mav_array_memcpy(packet.alpha, alpha, sizeof(float)*6);
    mav_array_memcpy(packet.omega, omega, sizeof(float)*6);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_VOLIRO_COMMAND;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_VOLIRO_COMMAND_MIN_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_CRC);
}

/**
 * @brief Pack a voliro_command message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param alpha tiltingAngles
 * @param omega motorSpeeds
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_voliro_command_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const float *alpha,const float *omega)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN];

    _mav_put_float_array(buf, 0, alpha, 6);
    _mav_put_float_array(buf, 24, omega, 6);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN);
#else
    mavlink_voliro_command_t packet;

    mav_array_memcpy(packet.alpha, alpha, sizeof(float)*6);
    mav_array_memcpy(packet.omega, omega, sizeof(float)*6);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_VOLIRO_COMMAND;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_VOLIRO_COMMAND_MIN_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_CRC);
}

/**
 * @brief Encode a voliro_command struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param voliro_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_voliro_command_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_voliro_command_t* voliro_command)
{
    return mavlink_msg_voliro_command_pack(system_id, component_id, msg, voliro_command->alpha, voliro_command->omega);
}

/**
 * @brief Encode a voliro_command struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param voliro_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_voliro_command_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_voliro_command_t* voliro_command)
{
    return mavlink_msg_voliro_command_pack_chan(system_id, component_id, chan, msg, voliro_command->alpha, voliro_command->omega);
}

/**
 * @brief Send a voliro_command message
 * @param chan MAVLink channel to send the message
 *
 * @param alpha tiltingAngles
 * @param omega motorSpeeds
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_voliro_command_send(mavlink_channel_t chan, const float *alpha, const float *omega)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN];

    _mav_put_float_array(buf, 0, alpha, 6);
    _mav_put_float_array(buf, 24, omega, 6);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_VOLIRO_COMMAND, buf, MAVLINK_MSG_ID_VOLIRO_COMMAND_MIN_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_CRC);
#else
    mavlink_voliro_command_t packet;

    mav_array_memcpy(packet.alpha, alpha, sizeof(float)*6);
    mav_array_memcpy(packet.omega, omega, sizeof(float)*6);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_VOLIRO_COMMAND, (const char *)&packet, MAVLINK_MSG_ID_VOLIRO_COMMAND_MIN_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_CRC);
#endif
}

/**
 * @brief Send a voliro_command message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_voliro_command_send_struct(mavlink_channel_t chan, const mavlink_voliro_command_t* voliro_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_voliro_command_send(chan, voliro_command->alpha, voliro_command->omega);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_VOLIRO_COMMAND, (const char *)voliro_command, MAVLINK_MSG_ID_VOLIRO_COMMAND_MIN_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_CRC);
#endif
}

#if MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_voliro_command_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const float *alpha, const float *omega)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;

    _mav_put_float_array(buf, 0, alpha, 6);
    _mav_put_float_array(buf, 24, omega, 6);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_VOLIRO_COMMAND, buf, MAVLINK_MSG_ID_VOLIRO_COMMAND_MIN_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_CRC);
#else
    mavlink_voliro_command_t *packet = (mavlink_voliro_command_t *)msgbuf;

    mav_array_memcpy(packet->alpha, alpha, sizeof(float)*6);
    mav_array_memcpy(packet->omega, omega, sizeof(float)*6);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_VOLIRO_COMMAND, (const char *)packet, MAVLINK_MSG_ID_VOLIRO_COMMAND_MIN_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN, MAVLINK_MSG_ID_VOLIRO_COMMAND_CRC);
#endif
}
#endif

#endif

// MESSAGE VOLIRO_COMMAND UNPACKING


/**
 * @brief Get field alpha from voliro_command message
 *
 * @return tiltingAngles
 */
static inline uint16_t mavlink_msg_voliro_command_get_alpha(const mavlink_message_t* msg, float *alpha)
{
    return _MAV_RETURN_float_array(msg, alpha, 6,  0);
}

/**
 * @brief Get field omega from voliro_command message
 *
 * @return motorSpeeds
 */
static inline uint16_t mavlink_msg_voliro_command_get_omega(const mavlink_message_t* msg, float *omega)
{
    return _MAV_RETURN_float_array(msg, omega, 6,  24);
}

/**
 * @brief Decode a voliro_command message into a struct
 *
 * @param msg The message to decode
 * @param voliro_command C-struct to decode the message contents into
 */
static inline void mavlink_msg_voliro_command_decode(const mavlink_message_t* msg, mavlink_voliro_command_t* voliro_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_voliro_command_get_alpha(msg, voliro_command->alpha);
    mavlink_msg_voliro_command_get_omega(msg, voliro_command->omega);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN? msg->len : MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN;
        memset(voliro_command, 0, MAVLINK_MSG_ID_VOLIRO_COMMAND_LEN);
    memcpy(voliro_command, _MAV_PAYLOAD(msg), len);
#endif
}
