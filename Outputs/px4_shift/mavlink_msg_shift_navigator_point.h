#pragma once
// MESSAGE SHIFT_NAVIGATOR_POINT PACKING

#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT 560

MAVPACKED(
typedef struct __mavlink_shift_navigator_point_t {
 float positionX; /*< positionX value*/
 float positionY; /*< positionY value*/
 float positionZ; /*< positionZ value*/
 float positionRoll; /*< positionRoll value*/
 float positionPitch; /*< positionPitch value*/
 float positionYaw; /*< positionYaw value*/
 float duration; /*< duration to reach this point(x,y,z,r,p,y)*/
 float arcRoll; /*< arcRoll value*/
 float arcPitch; /*< arcPitch value*/
 float arcYaw; /*< arcYaw value*/
 uint16_t pathIndex; /*< The pathIndex to which this point belongs*/
 uint16_t pointIndex; /*< The pointIndex of this point*/
 uint8_t target_system; /*< System ID*/
 uint8_t yawBehaviour; /*< Behaviour of the yaw while traveling to this point. Using ENUM: SHIFT_NAVIGATOR_YAW_BEHAVIOUR_ENUM*/
 uint8_t type; /*< Type of this point. Using ENUM: SHIFT_NAVIGATOR_POINT_TYPE_ENUM*/
}) mavlink_shift_navigator_point_t;

#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN 47
#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN 47
#define MAVLINK_MSG_ID_560_LEN 47
#define MAVLINK_MSG_ID_560_MIN_LEN 47

#define MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_CRC 137
#define MAVLINK_MSG_ID_560_CRC 137



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SHIFT_NAVIGATOR_POINT { \
    560, \
    "SHIFT_NAVIGATOR_POINT", \
    15, \
    {  { "positionX", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_shift_navigator_point_t, positionX) }, \
         { "positionY", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_shift_navigator_point_t, positionY) }, \
         { "positionZ", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_shift_navigator_point_t, positionZ) }, \
         { "positionRoll", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_shift_navigator_point_t, positionRoll) }, \
         { "positionPitch", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_shift_navigator_point_t, positionPitch) }, \
         { "positionYaw", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_shift_navigator_point_t, positionYaw) }, \
         { "duration", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_shift_navigator_point_t, duration) }, \
         { "arcRoll", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_shift_navigator_point_t, arcRoll) }, \
         { "arcPitch", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_shift_navigator_point_t, arcPitch) }, \
         { "arcYaw", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_shift_navigator_point_t, arcYaw) }, \
         { "pathIndex", NULL, MAVLINK_TYPE_UINT16_T, 0, 40, offsetof(mavlink_shift_navigator_point_t, pathIndex) }, \
         { "pointIndex", NULL, MAVLINK_TYPE_UINT16_T, 0, 42, offsetof(mavlink_shift_navigator_point_t, pointIndex) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 44, offsetof(mavlink_shift_navigator_point_t, target_system) }, \
         { "yawBehaviour", NULL, MAVLINK_TYPE_UINT8_T, 0, 45, offsetof(mavlink_shift_navigator_point_t, yawBehaviour) }, \
         { "type", NULL, MAVLINK_TYPE_UINT8_T, 0, 46, offsetof(mavlink_shift_navigator_point_t, type) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SHIFT_NAVIGATOR_POINT { \
    "SHIFT_NAVIGATOR_POINT", \
    15, \
    {  { "positionX", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_shift_navigator_point_t, positionX) }, \
         { "positionY", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_shift_navigator_point_t, positionY) }, \
         { "positionZ", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_shift_navigator_point_t, positionZ) }, \
         { "positionRoll", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_shift_navigator_point_t, positionRoll) }, \
         { "positionPitch", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_shift_navigator_point_t, positionPitch) }, \
         { "positionYaw", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_shift_navigator_point_t, positionYaw) }, \
         { "duration", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_shift_navigator_point_t, duration) }, \
         { "arcRoll", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_shift_navigator_point_t, arcRoll) }, \
         { "arcPitch", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_shift_navigator_point_t, arcPitch) }, \
         { "arcYaw", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_shift_navigator_point_t, arcYaw) }, \
         { "pathIndex", NULL, MAVLINK_TYPE_UINT16_T, 0, 40, offsetof(mavlink_shift_navigator_point_t, pathIndex) }, \
         { "pointIndex", NULL, MAVLINK_TYPE_UINT16_T, 0, 42, offsetof(mavlink_shift_navigator_point_t, pointIndex) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 44, offsetof(mavlink_shift_navigator_point_t, target_system) }, \
         { "yawBehaviour", NULL, MAVLINK_TYPE_UINT8_T, 0, 45, offsetof(mavlink_shift_navigator_point_t, yawBehaviour) }, \
         { "type", NULL, MAVLINK_TYPE_UINT8_T, 0, 46, offsetof(mavlink_shift_navigator_point_t, type) }, \
         } \
}
#endif

/**
 * @brief Pack a shift_navigator_point message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param pathIndex The pathIndex to which this point belongs
 * @param pointIndex The pointIndex of this point
 * @param positionX positionX value
 * @param positionY positionY value
 * @param positionZ positionZ value
 * @param positionRoll positionRoll value
 * @param positionPitch positionPitch value
 * @param positionYaw positionYaw value
 * @param duration duration to reach this point(x,y,z,r,p,y)
 * @param yawBehaviour Behaviour of the yaw while traveling to this point. Using ENUM: SHIFT_NAVIGATOR_YAW_BEHAVIOUR_ENUM
 * @param type Type of this point. Using ENUM: SHIFT_NAVIGATOR_POINT_TYPE_ENUM
 * @param arcRoll arcRoll value
 * @param arcPitch arcPitch value
 * @param arcYaw arcYaw value
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_navigator_point_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint16_t pathIndex, uint16_t pointIndex, float positionX, float positionY, float positionZ, float positionRoll, float positionPitch, float positionYaw, float duration, uint8_t yawBehaviour, uint8_t type, float arcRoll, float arcPitch, float arcYaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN];
    _mav_put_float(buf, 0, positionX);
    _mav_put_float(buf, 4, positionY);
    _mav_put_float(buf, 8, positionZ);
    _mav_put_float(buf, 12, positionRoll);
    _mav_put_float(buf, 16, positionPitch);
    _mav_put_float(buf, 20, positionYaw);
    _mav_put_float(buf, 24, duration);
    _mav_put_float(buf, 28, arcRoll);
    _mav_put_float(buf, 32, arcPitch);
    _mav_put_float(buf, 36, arcYaw);
    _mav_put_uint16_t(buf, 40, pathIndex);
    _mav_put_uint16_t(buf, 42, pointIndex);
    _mav_put_uint8_t(buf, 44, target_system);
    _mav_put_uint8_t(buf, 45, yawBehaviour);
    _mav_put_uint8_t(buf, 46, type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN);
#else
    mavlink_shift_navigator_point_t packet;
    packet.positionX = positionX;
    packet.positionY = positionY;
    packet.positionZ = positionZ;
    packet.positionRoll = positionRoll;
    packet.positionPitch = positionPitch;
    packet.positionYaw = positionYaw;
    packet.duration = duration;
    packet.arcRoll = arcRoll;
    packet.arcPitch = arcPitch;
    packet.arcYaw = arcYaw;
    packet.pathIndex = pathIndex;
    packet.pointIndex = pointIndex;
    packet.target_system = target_system;
    packet.yawBehaviour = yawBehaviour;
    packet.type = type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_CRC);
}

/**
 * @brief Pack a shift_navigator_point message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param pathIndex The pathIndex to which this point belongs
 * @param pointIndex The pointIndex of this point
 * @param positionX positionX value
 * @param positionY positionY value
 * @param positionZ positionZ value
 * @param positionRoll positionRoll value
 * @param positionPitch positionPitch value
 * @param positionYaw positionYaw value
 * @param duration duration to reach this point(x,y,z,r,p,y)
 * @param yawBehaviour Behaviour of the yaw while traveling to this point. Using ENUM: SHIFT_NAVIGATOR_YAW_BEHAVIOUR_ENUM
 * @param type Type of this point. Using ENUM: SHIFT_NAVIGATOR_POINT_TYPE_ENUM
 * @param arcRoll arcRoll value
 * @param arcPitch arcPitch value
 * @param arcYaw arcYaw value
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_shift_navigator_point_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint16_t pathIndex,uint16_t pointIndex,float positionX,float positionY,float positionZ,float positionRoll,float positionPitch,float positionYaw,float duration,uint8_t yawBehaviour,uint8_t type,float arcRoll,float arcPitch,float arcYaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN];
    _mav_put_float(buf, 0, positionX);
    _mav_put_float(buf, 4, positionY);
    _mav_put_float(buf, 8, positionZ);
    _mav_put_float(buf, 12, positionRoll);
    _mav_put_float(buf, 16, positionPitch);
    _mav_put_float(buf, 20, positionYaw);
    _mav_put_float(buf, 24, duration);
    _mav_put_float(buf, 28, arcRoll);
    _mav_put_float(buf, 32, arcPitch);
    _mav_put_float(buf, 36, arcYaw);
    _mav_put_uint16_t(buf, 40, pathIndex);
    _mav_put_uint16_t(buf, 42, pointIndex);
    _mav_put_uint8_t(buf, 44, target_system);
    _mav_put_uint8_t(buf, 45, yawBehaviour);
    _mav_put_uint8_t(buf, 46, type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN);
#else
    mavlink_shift_navigator_point_t packet;
    packet.positionX = positionX;
    packet.positionY = positionY;
    packet.positionZ = positionZ;
    packet.positionRoll = positionRoll;
    packet.positionPitch = positionPitch;
    packet.positionYaw = positionYaw;
    packet.duration = duration;
    packet.arcRoll = arcRoll;
    packet.arcPitch = arcPitch;
    packet.arcYaw = arcYaw;
    packet.pathIndex = pathIndex;
    packet.pointIndex = pointIndex;
    packet.target_system = target_system;
    packet.yawBehaviour = yawBehaviour;
    packet.type = type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_CRC);
}

/**
 * @brief Encode a shift_navigator_point struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param shift_navigator_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_navigator_point_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_shift_navigator_point_t* shift_navigator_point)
{
    return mavlink_msg_shift_navigator_point_pack(system_id, component_id, msg, shift_navigator_point->target_system, shift_navigator_point->pathIndex, shift_navigator_point->pointIndex, shift_navigator_point->positionX, shift_navigator_point->positionY, shift_navigator_point->positionZ, shift_navigator_point->positionRoll, shift_navigator_point->positionPitch, shift_navigator_point->positionYaw, shift_navigator_point->duration, shift_navigator_point->yawBehaviour, shift_navigator_point->type, shift_navigator_point->arcRoll, shift_navigator_point->arcPitch, shift_navigator_point->arcYaw);
}

/**
 * @brief Encode a shift_navigator_point struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param shift_navigator_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_shift_navigator_point_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_shift_navigator_point_t* shift_navigator_point)
{
    return mavlink_msg_shift_navigator_point_pack_chan(system_id, component_id, chan, msg, shift_navigator_point->target_system, shift_navigator_point->pathIndex, shift_navigator_point->pointIndex, shift_navigator_point->positionX, shift_navigator_point->positionY, shift_navigator_point->positionZ, shift_navigator_point->positionRoll, shift_navigator_point->positionPitch, shift_navigator_point->positionYaw, shift_navigator_point->duration, shift_navigator_point->yawBehaviour, shift_navigator_point->type, shift_navigator_point->arcRoll, shift_navigator_point->arcPitch, shift_navigator_point->arcYaw);
}

/**
 * @brief Send a shift_navigator_point message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param pathIndex The pathIndex to which this point belongs
 * @param pointIndex The pointIndex of this point
 * @param positionX positionX value
 * @param positionY positionY value
 * @param positionZ positionZ value
 * @param positionRoll positionRoll value
 * @param positionPitch positionPitch value
 * @param positionYaw positionYaw value
 * @param duration duration to reach this point(x,y,z,r,p,y)
 * @param yawBehaviour Behaviour of the yaw while traveling to this point. Using ENUM: SHIFT_NAVIGATOR_YAW_BEHAVIOUR_ENUM
 * @param type Type of this point. Using ENUM: SHIFT_NAVIGATOR_POINT_TYPE_ENUM
 * @param arcRoll arcRoll value
 * @param arcPitch arcPitch value
 * @param arcYaw arcYaw value
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_shift_navigator_point_send(mavlink_channel_t chan, uint8_t target_system, uint16_t pathIndex, uint16_t pointIndex, float positionX, float positionY, float positionZ, float positionRoll, float positionPitch, float positionYaw, float duration, uint8_t yawBehaviour, uint8_t type, float arcRoll, float arcPitch, float arcYaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN];
    _mav_put_float(buf, 0, positionX);
    _mav_put_float(buf, 4, positionY);
    _mav_put_float(buf, 8, positionZ);
    _mav_put_float(buf, 12, positionRoll);
    _mav_put_float(buf, 16, positionPitch);
    _mav_put_float(buf, 20, positionYaw);
    _mav_put_float(buf, 24, duration);
    _mav_put_float(buf, 28, arcRoll);
    _mav_put_float(buf, 32, arcPitch);
    _mav_put_float(buf, 36, arcYaw);
    _mav_put_uint16_t(buf, 40, pathIndex);
    _mav_put_uint16_t(buf, 42, pointIndex);
    _mav_put_uint8_t(buf, 44, target_system);
    _mav_put_uint8_t(buf, 45, yawBehaviour);
    _mav_put_uint8_t(buf, 46, type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT, buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_CRC);
#else
    mavlink_shift_navigator_point_t packet;
    packet.positionX = positionX;
    packet.positionY = positionY;
    packet.positionZ = positionZ;
    packet.positionRoll = positionRoll;
    packet.positionPitch = positionPitch;
    packet.positionYaw = positionYaw;
    packet.duration = duration;
    packet.arcRoll = arcRoll;
    packet.arcPitch = arcPitch;
    packet.arcYaw = arcYaw;
    packet.pathIndex = pathIndex;
    packet.pointIndex = pointIndex;
    packet.target_system = target_system;
    packet.yawBehaviour = yawBehaviour;
    packet.type = type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT, (const char *)&packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_CRC);
#endif
}

/**
 * @brief Send a shift_navigator_point message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_shift_navigator_point_send_struct(mavlink_channel_t chan, const mavlink_shift_navigator_point_t* shift_navigator_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_shift_navigator_point_send(chan, shift_navigator_point->target_system, shift_navigator_point->pathIndex, shift_navigator_point->pointIndex, shift_navigator_point->positionX, shift_navigator_point->positionY, shift_navigator_point->positionZ, shift_navigator_point->positionRoll, shift_navigator_point->positionPitch, shift_navigator_point->positionYaw, shift_navigator_point->duration, shift_navigator_point->yawBehaviour, shift_navigator_point->type, shift_navigator_point->arcRoll, shift_navigator_point->arcPitch, shift_navigator_point->arcYaw);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT, (const char *)shift_navigator_point, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_CRC);
#endif
}

#if MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_shift_navigator_point_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint16_t pathIndex, uint16_t pointIndex, float positionX, float positionY, float positionZ, float positionRoll, float positionPitch, float positionYaw, float duration, uint8_t yawBehaviour, uint8_t type, float arcRoll, float arcPitch, float arcYaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, positionX);
    _mav_put_float(buf, 4, positionY);
    _mav_put_float(buf, 8, positionZ);
    _mav_put_float(buf, 12, positionRoll);
    _mav_put_float(buf, 16, positionPitch);
    _mav_put_float(buf, 20, positionYaw);
    _mav_put_float(buf, 24, duration);
    _mav_put_float(buf, 28, arcRoll);
    _mav_put_float(buf, 32, arcPitch);
    _mav_put_float(buf, 36, arcYaw);
    _mav_put_uint16_t(buf, 40, pathIndex);
    _mav_put_uint16_t(buf, 42, pointIndex);
    _mav_put_uint8_t(buf, 44, target_system);
    _mav_put_uint8_t(buf, 45, yawBehaviour);
    _mav_put_uint8_t(buf, 46, type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT, buf, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_CRC);
#else
    mavlink_shift_navigator_point_t *packet = (mavlink_shift_navigator_point_t *)msgbuf;
    packet->positionX = positionX;
    packet->positionY = positionY;
    packet->positionZ = positionZ;
    packet->positionRoll = positionRoll;
    packet->positionPitch = positionPitch;
    packet->positionYaw = positionYaw;
    packet->duration = duration;
    packet->arcRoll = arcRoll;
    packet->arcPitch = arcPitch;
    packet->arcYaw = arcYaw;
    packet->pathIndex = pathIndex;
    packet->pointIndex = pointIndex;
    packet->target_system = target_system;
    packet->yawBehaviour = yawBehaviour;
    packet->type = type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT, (const char *)packet, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_CRC);
#endif
}
#endif

#endif

// MESSAGE SHIFT_NAVIGATOR_POINT UNPACKING


/**
 * @brief Get field target_system from shift_navigator_point message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_shift_navigator_point_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  44);
}

/**
 * @brief Get field pathIndex from shift_navigator_point message
 *
 * @return The pathIndex to which this point belongs
 */
static inline uint16_t mavlink_msg_shift_navigator_point_get_pathIndex(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  40);
}

/**
 * @brief Get field pointIndex from shift_navigator_point message
 *
 * @return The pointIndex of this point
 */
static inline uint16_t mavlink_msg_shift_navigator_point_get_pointIndex(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  42);
}

/**
 * @brief Get field positionX from shift_navigator_point message
 *
 * @return positionX value
 */
static inline float mavlink_msg_shift_navigator_point_get_positionX(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field positionY from shift_navigator_point message
 *
 * @return positionY value
 */
static inline float mavlink_msg_shift_navigator_point_get_positionY(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field positionZ from shift_navigator_point message
 *
 * @return positionZ value
 */
static inline float mavlink_msg_shift_navigator_point_get_positionZ(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field positionRoll from shift_navigator_point message
 *
 * @return positionRoll value
 */
static inline float mavlink_msg_shift_navigator_point_get_positionRoll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field positionPitch from shift_navigator_point message
 *
 * @return positionPitch value
 */
static inline float mavlink_msg_shift_navigator_point_get_positionPitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field positionYaw from shift_navigator_point message
 *
 * @return positionYaw value
 */
static inline float mavlink_msg_shift_navigator_point_get_positionYaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field duration from shift_navigator_point message
 *
 * @return duration to reach this point(x,y,z,r,p,y)
 */
static inline float mavlink_msg_shift_navigator_point_get_duration(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field yawBehaviour from shift_navigator_point message
 *
 * @return Behaviour of the yaw while traveling to this point. Using ENUM: SHIFT_NAVIGATOR_YAW_BEHAVIOUR_ENUM
 */
static inline uint8_t mavlink_msg_shift_navigator_point_get_yawBehaviour(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  45);
}

/**
 * @brief Get field type from shift_navigator_point message
 *
 * @return Type of this point. Using ENUM: SHIFT_NAVIGATOR_POINT_TYPE_ENUM
 */
static inline uint8_t mavlink_msg_shift_navigator_point_get_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  46);
}

/**
 * @brief Get field arcRoll from shift_navigator_point message
 *
 * @return arcRoll value
 */
static inline float mavlink_msg_shift_navigator_point_get_arcRoll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field arcPitch from shift_navigator_point message
 *
 * @return arcPitch value
 */
static inline float mavlink_msg_shift_navigator_point_get_arcPitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field arcYaw from shift_navigator_point message
 *
 * @return arcYaw value
 */
static inline float mavlink_msg_shift_navigator_point_get_arcYaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Decode a shift_navigator_point message into a struct
 *
 * @param msg The message to decode
 * @param shift_navigator_point C-struct to decode the message contents into
 */
static inline void mavlink_msg_shift_navigator_point_decode(const mavlink_message_t* msg, mavlink_shift_navigator_point_t* shift_navigator_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    shift_navigator_point->positionX = mavlink_msg_shift_navigator_point_get_positionX(msg);
    shift_navigator_point->positionY = mavlink_msg_shift_navigator_point_get_positionY(msg);
    shift_navigator_point->positionZ = mavlink_msg_shift_navigator_point_get_positionZ(msg);
    shift_navigator_point->positionRoll = mavlink_msg_shift_navigator_point_get_positionRoll(msg);
    shift_navigator_point->positionPitch = mavlink_msg_shift_navigator_point_get_positionPitch(msg);
    shift_navigator_point->positionYaw = mavlink_msg_shift_navigator_point_get_positionYaw(msg);
    shift_navigator_point->duration = mavlink_msg_shift_navigator_point_get_duration(msg);
    shift_navigator_point->arcRoll = mavlink_msg_shift_navigator_point_get_arcRoll(msg);
    shift_navigator_point->arcPitch = mavlink_msg_shift_navigator_point_get_arcPitch(msg);
    shift_navigator_point->arcYaw = mavlink_msg_shift_navigator_point_get_arcYaw(msg);
    shift_navigator_point->pathIndex = mavlink_msg_shift_navigator_point_get_pathIndex(msg);
    shift_navigator_point->pointIndex = mavlink_msg_shift_navigator_point_get_pointIndex(msg);
    shift_navigator_point->target_system = mavlink_msg_shift_navigator_point_get_target_system(msg);
    shift_navigator_point->yawBehaviour = mavlink_msg_shift_navigator_point_get_yawBehaviour(msg);
    shift_navigator_point->type = mavlink_msg_shift_navigator_point_get_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN? msg->len : MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN;
        memset(shift_navigator_point, 0, MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_LEN);
    memcpy(shift_navigator_point, _MAV_PAYLOAD(msg), len);
#endif
}
