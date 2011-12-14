// MESSAGE SET_QUAD_MOTORS_SETPOINT PACKING

#define MAVLINK_MSG_ID_SET_QUAD_MOTORS_SETPOINT 60

typedef struct __mavlink_set_quad_motors_setpoint_t
{
 uint8_t motorFront_NW; ///< Front motor in + configuration, front left motor in x configuration
 uint8_t motorRight_NE; ///< Right motor in + configuration, front right motor in x configuration
 uint8_t motorBack_SE; ///< Back motor in + configuration, back right motor in x configuration
 uint8_t motorLeft_SW; ///< Left motor in + configuration, back left motor in x configuration
} mavlink_set_quad_motors_setpoint_t;

#define MAVLINK_MSG_ID_SET_QUAD_MOTORS_SETPOINT_LEN 4
#define MAVLINK_MSG_ID_60_LEN 4



#define MAVLINK_MESSAGE_INFO_SET_QUAD_MOTORS_SETPOINT { \
	"SET_QUAD_MOTORS_SETPOINT", \
	4, \
	{  { "motorFront_NW", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_set_quad_motors_setpoint_t, motorFront_NW) }, \
         { "motorRight_NE", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_set_quad_motors_setpoint_t, motorRight_NE) }, \
         { "motorBack_SE", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_set_quad_motors_setpoint_t, motorBack_SE) }, \
         { "motorLeft_SW", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_set_quad_motors_setpoint_t, motorLeft_SW) }, \
         } \
}


/**
 * @brief Pack a set_quad_motors_setpoint message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param motorFront_NW Front motor in + configuration, front left motor in x configuration
 * @param motorRight_NE Right motor in + configuration, front right motor in x configuration
 * @param motorBack_SE Back motor in + configuration, back right motor in x configuration
 * @param motorLeft_SW Left motor in + configuration, back left motor in x configuration
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_quad_motors_setpoint_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t motorFront_NW, uint8_t motorRight_NE, uint8_t motorBack_SE, uint8_t motorLeft_SW)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[4];
	_mav_put_uint8_t(buf, 0, motorFront_NW);
	_mav_put_uint8_t(buf, 1, motorRight_NE);
	_mav_put_uint8_t(buf, 2, motorBack_SE);
	_mav_put_uint8_t(buf, 3, motorLeft_SW);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 4);
#else
	mavlink_set_quad_motors_setpoint_t packet;
	packet.motorFront_NW = motorFront_NW;
	packet.motorRight_NE = motorRight_NE;
	packet.motorBack_SE = motorBack_SE;
	packet.motorLeft_SW = motorLeft_SW;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 4);
#endif

	msg->msgid = MAVLINK_MSG_ID_SET_QUAD_MOTORS_SETPOINT;
	return mavlink_finalize_message(msg, system_id, component_id, 4, 100);
}

/**
 * @brief Pack a set_quad_motors_setpoint message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param motorFront_NW Front motor in + configuration, front left motor in x configuration
 * @param motorRight_NE Right motor in + configuration, front right motor in x configuration
 * @param motorBack_SE Back motor in + configuration, back right motor in x configuration
 * @param motorLeft_SW Left motor in + configuration, back left motor in x configuration
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_quad_motors_setpoint_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t motorFront_NW,uint8_t motorRight_NE,uint8_t motorBack_SE,uint8_t motorLeft_SW)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[4];
	_mav_put_uint8_t(buf, 0, motorFront_NW);
	_mav_put_uint8_t(buf, 1, motorRight_NE);
	_mav_put_uint8_t(buf, 2, motorBack_SE);
	_mav_put_uint8_t(buf, 3, motorLeft_SW);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 4);
#else
	mavlink_set_quad_motors_setpoint_t packet;
	packet.motorFront_NW = motorFront_NW;
	packet.motorRight_NE = motorRight_NE;
	packet.motorBack_SE = motorBack_SE;
	packet.motorLeft_SW = motorLeft_SW;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 4);
#endif

	msg->msgid = MAVLINK_MSG_ID_SET_QUAD_MOTORS_SETPOINT;
	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 4, 100);
}

/**
 * @brief Encode a set_quad_motors_setpoint struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param set_quad_motors_setpoint C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_quad_motors_setpoint_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_set_quad_motors_setpoint_t* set_quad_motors_setpoint)
{
	return mavlink_msg_set_quad_motors_setpoint_pack(system_id, component_id, msg, set_quad_motors_setpoint->motorFront_NW, set_quad_motors_setpoint->motorRight_NE, set_quad_motors_setpoint->motorBack_SE, set_quad_motors_setpoint->motorLeft_SW);
}

/**
 * @brief Send a set_quad_motors_setpoint message
 * @param chan MAVLink channel to send the message
 *
 * @param motorFront_NW Front motor in + configuration, front left motor in x configuration
 * @param motorRight_NE Right motor in + configuration, front right motor in x configuration
 * @param motorBack_SE Back motor in + configuration, back right motor in x configuration
 * @param motorLeft_SW Left motor in + configuration, back left motor in x configuration
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_set_quad_motors_setpoint_send(mavlink_channel_t chan, uint8_t motorFront_NW, uint8_t motorRight_NE, uint8_t motorBack_SE, uint8_t motorLeft_SW)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[4];
	_mav_put_uint8_t(buf, 0, motorFront_NW);
	_mav_put_uint8_t(buf, 1, motorRight_NE);
	_mav_put_uint8_t(buf, 2, motorBack_SE);
	_mav_put_uint8_t(buf, 3, motorLeft_SW);

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_QUAD_MOTORS_SETPOINT, buf, 4, 100);
#else
	mavlink_set_quad_motors_setpoint_t packet;
	packet.motorFront_NW = motorFront_NW;
	packet.motorRight_NE = motorRight_NE;
	packet.motorBack_SE = motorBack_SE;
	packet.motorLeft_SW = motorLeft_SW;

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_QUAD_MOTORS_SETPOINT, (const char *)&packet, 4, 100);
#endif
}

#endif

// MESSAGE SET_QUAD_MOTORS_SETPOINT UNPACKING


/**
 * @brief Get field motorFront_NW from set_quad_motors_setpoint message
 *
 * @return Front motor in + configuration, front left motor in x configuration
 */
static inline uint8_t mavlink_msg_set_quad_motors_setpoint_get_motorFront_NW(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field motorRight_NE from set_quad_motors_setpoint message
 *
 * @return Right motor in + configuration, front right motor in x configuration
 */
static inline uint8_t mavlink_msg_set_quad_motors_setpoint_get_motorRight_NE(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field motorBack_SE from set_quad_motors_setpoint message
 *
 * @return Back motor in + configuration, back right motor in x configuration
 */
static inline uint8_t mavlink_msg_set_quad_motors_setpoint_get_motorBack_SE(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field motorLeft_SW from set_quad_motors_setpoint message
 *
 * @return Left motor in + configuration, back left motor in x configuration
 */
static inline uint8_t mavlink_msg_set_quad_motors_setpoint_get_motorLeft_SW(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Decode a set_quad_motors_setpoint message into a struct
 *
 * @param msg The message to decode
 * @param set_quad_motors_setpoint C-struct to decode the message contents into
 */
static inline void mavlink_msg_set_quad_motors_setpoint_decode(const mavlink_message_t* msg, mavlink_set_quad_motors_setpoint_t* set_quad_motors_setpoint)
{
#if MAVLINK_NEED_BYTE_SWAP
	set_quad_motors_setpoint->motorFront_NW = mavlink_msg_set_quad_motors_setpoint_get_motorFront_NW(msg);
	set_quad_motors_setpoint->motorRight_NE = mavlink_msg_set_quad_motors_setpoint_get_motorRight_NE(msg);
	set_quad_motors_setpoint->motorBack_SE = mavlink_msg_set_quad_motors_setpoint_get_motorBack_SE(msg);
	set_quad_motors_setpoint->motorLeft_SW = mavlink_msg_set_quad_motors_setpoint_get_motorLeft_SW(msg);
#else
	memcpy(set_quad_motors_setpoint, _MAV_PAYLOAD(msg), 4);
#endif
}
