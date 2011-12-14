// MESSAGE QUAD_SWARM_ROLL_PITCH_YAW_THRUST PACKING

#define MAVLINK_MSG_ID_QUAD_SWARM_ROLL_PITCH_YAW_THRUST 61

typedef struct __mavlink_quad_swarm_roll_pitch_yaw_thrust_t
{
 int16_t roll[7]; ///< Desired roll angle in radians, scaled to int16 for 7 quadrotors: 0..6
 int16_t pitch[7]; ///< Desired pitch angle in radians, scaled to int16 for 7 quadrotors: 0..6
 int16_t yaw[7]; ///< Desired yaw angle in radians, scaled to int16 for 7 quadrotors: 0..6
 uint16_t thrust[7]; ///< Collective thrust, scaled to uint16 for 7 quadrotors: 0..6
} mavlink_quad_swarm_roll_pitch_yaw_thrust_t;

#define MAVLINK_MSG_ID_QUAD_SWARM_ROLL_PITCH_YAW_THRUST_LEN 56
#define MAVLINK_MSG_ID_61_LEN 56

#define MAVLINK_MSG_QUAD_SWARM_ROLL_PITCH_YAW_THRUST_FIELD_ROLL_LEN 7
#define MAVLINK_MSG_QUAD_SWARM_ROLL_PITCH_YAW_THRUST_FIELD_PITCH_LEN 7
#define MAVLINK_MSG_QUAD_SWARM_ROLL_PITCH_YAW_THRUST_FIELD_YAW_LEN 7
#define MAVLINK_MSG_QUAD_SWARM_ROLL_PITCH_YAW_THRUST_FIELD_THRUST_LEN 7

#define MAVLINK_MESSAGE_INFO_QUAD_SWARM_ROLL_PITCH_YAW_THRUST { \
	"QUAD_SWARM_ROLL_PITCH_YAW_THRUST", \
	4, \
	{  { "roll", NULL, MAVLINK_TYPE_INT16_T, 7, 0, offsetof(mavlink_quad_swarm_roll_pitch_yaw_thrust_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_INT16_T, 7, 14, offsetof(mavlink_quad_swarm_roll_pitch_yaw_thrust_t, pitch) }, \
         { "yaw", NULL, MAVLINK_TYPE_INT16_T, 7, 28, offsetof(mavlink_quad_swarm_roll_pitch_yaw_thrust_t, yaw) }, \
         { "thrust", NULL, MAVLINK_TYPE_UINT16_T, 7, 42, offsetof(mavlink_quad_swarm_roll_pitch_yaw_thrust_t, thrust) }, \
         } \
}


/**
 * @brief Pack a quad_swarm_roll_pitch_yaw_thrust message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param roll Desired roll angle in radians, scaled to int16 for 7 quadrotors: 0..6
 * @param pitch Desired pitch angle in radians, scaled to int16 for 7 quadrotors: 0..6
 * @param yaw Desired yaw angle in radians, scaled to int16 for 7 quadrotors: 0..6
 * @param thrust Collective thrust, scaled to uint16 for 7 quadrotors: 0..6
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       const int16_t *roll, const int16_t *pitch, const int16_t *yaw, const uint16_t *thrust)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[56];

	_mav_put_int16_t_array(buf, 0, roll, 7);
	_mav_put_int16_t_array(buf, 14, pitch, 7);
	_mav_put_int16_t_array(buf, 28, yaw, 7);
	_mav_put_uint16_t_array(buf, 42, thrust, 7);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 56);
#else
	mavlink_quad_swarm_roll_pitch_yaw_thrust_t packet;

	mav_array_memcpy(packet.roll, roll, sizeof(int16_t)*7);
	mav_array_memcpy(packet.pitch, pitch, sizeof(int16_t)*7);
	mav_array_memcpy(packet.yaw, yaw, sizeof(int16_t)*7);
	mav_array_memcpy(packet.thrust, thrust, sizeof(uint16_t)*7);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 56);
#endif

	msg->msgid = MAVLINK_MSG_ID_QUAD_SWARM_ROLL_PITCH_YAW_THRUST;
	return mavlink_finalize_message(msg, system_id, component_id, 56, 44);
}

/**
 * @brief Pack a quad_swarm_roll_pitch_yaw_thrust message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param roll Desired roll angle in radians, scaled to int16 for 7 quadrotors: 0..6
 * @param pitch Desired pitch angle in radians, scaled to int16 for 7 quadrotors: 0..6
 * @param yaw Desired yaw angle in radians, scaled to int16 for 7 quadrotors: 0..6
 * @param thrust Collective thrust, scaled to uint16 for 7 quadrotors: 0..6
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           const int16_t *roll,const int16_t *pitch,const int16_t *yaw,const uint16_t *thrust)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[56];

	_mav_put_int16_t_array(buf, 0, roll, 7);
	_mav_put_int16_t_array(buf, 14, pitch, 7);
	_mav_put_int16_t_array(buf, 28, yaw, 7);
	_mav_put_uint16_t_array(buf, 42, thrust, 7);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 56);
#else
	mavlink_quad_swarm_roll_pitch_yaw_thrust_t packet;

	mav_array_memcpy(packet.roll, roll, sizeof(int16_t)*7);
	mav_array_memcpy(packet.pitch, pitch, sizeof(int16_t)*7);
	mav_array_memcpy(packet.yaw, yaw, sizeof(int16_t)*7);
	mav_array_memcpy(packet.thrust, thrust, sizeof(uint16_t)*7);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 56);
#endif

	msg->msgid = MAVLINK_MSG_ID_QUAD_SWARM_ROLL_PITCH_YAW_THRUST;
	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 56, 44);
}

/**
 * @brief Encode a quad_swarm_roll_pitch_yaw_thrust struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param quad_swarm_roll_pitch_yaw_thrust C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_quad_swarm_roll_pitch_yaw_thrust_t* quad_swarm_roll_pitch_yaw_thrust)
{
	return mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_pack(system_id, component_id, msg, quad_swarm_roll_pitch_yaw_thrust->roll, quad_swarm_roll_pitch_yaw_thrust->pitch, quad_swarm_roll_pitch_yaw_thrust->yaw, quad_swarm_roll_pitch_yaw_thrust->thrust);
}

/**
 * @brief Send a quad_swarm_roll_pitch_yaw_thrust message
 * @param chan MAVLink channel to send the message
 *
 * @param roll Desired roll angle in radians, scaled to int16 for 7 quadrotors: 0..6
 * @param pitch Desired pitch angle in radians, scaled to int16 for 7 quadrotors: 0..6
 * @param yaw Desired yaw angle in radians, scaled to int16 for 7 quadrotors: 0..6
 * @param thrust Collective thrust, scaled to uint16 for 7 quadrotors: 0..6
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_send(mavlink_channel_t chan, const int16_t *roll, const int16_t *pitch, const int16_t *yaw, const uint16_t *thrust)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[56];

	_mav_put_int16_t_array(buf, 0, roll, 7);
	_mav_put_int16_t_array(buf, 14, pitch, 7);
	_mav_put_int16_t_array(buf, 28, yaw, 7);
	_mav_put_uint16_t_array(buf, 42, thrust, 7);
	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_QUAD_SWARM_ROLL_PITCH_YAW_THRUST, buf, 56, 44);
#else
	mavlink_quad_swarm_roll_pitch_yaw_thrust_t packet;

	mav_array_memcpy(packet.roll, roll, sizeof(int16_t)*7);
	mav_array_memcpy(packet.pitch, pitch, sizeof(int16_t)*7);
	mav_array_memcpy(packet.yaw, yaw, sizeof(int16_t)*7);
	mav_array_memcpy(packet.thrust, thrust, sizeof(uint16_t)*7);
	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_QUAD_SWARM_ROLL_PITCH_YAW_THRUST, (const char *)&packet, 56, 44);
#endif
}

#endif

// MESSAGE QUAD_SWARM_ROLL_PITCH_YAW_THRUST UNPACKING


/**
 * @brief Get field roll from quad_swarm_roll_pitch_yaw_thrust message
 *
 * @return Desired roll angle in radians, scaled to int16 for 7 quadrotors: 0..6
 */
static inline uint16_t mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_get_roll(const mavlink_message_t* msg, int16_t *roll)
{
	return _MAV_RETURN_int16_t_array(msg, roll, 7,  0);
}

/**
 * @brief Get field pitch from quad_swarm_roll_pitch_yaw_thrust message
 *
 * @return Desired pitch angle in radians, scaled to int16 for 7 quadrotors: 0..6
 */
static inline uint16_t mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_get_pitch(const mavlink_message_t* msg, int16_t *pitch)
{
	return _MAV_RETURN_int16_t_array(msg, pitch, 7,  14);
}

/**
 * @brief Get field yaw from quad_swarm_roll_pitch_yaw_thrust message
 *
 * @return Desired yaw angle in radians, scaled to int16 for 7 quadrotors: 0..6
 */
static inline uint16_t mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_get_yaw(const mavlink_message_t* msg, int16_t *yaw)
{
	return _MAV_RETURN_int16_t_array(msg, yaw, 7,  28);
}

/**
 * @brief Get field thrust from quad_swarm_roll_pitch_yaw_thrust message
 *
 * @return Collective thrust, scaled to uint16 for 7 quadrotors: 0..6
 */
static inline uint16_t mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_get_thrust(const mavlink_message_t* msg, uint16_t *thrust)
{
	return _MAV_RETURN_uint16_t_array(msg, thrust, 7,  42);
}

/**
 * @brief Decode a quad_swarm_roll_pitch_yaw_thrust message into a struct
 *
 * @param msg The message to decode
 * @param quad_swarm_roll_pitch_yaw_thrust C-struct to decode the message contents into
 */
static inline void mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_decode(const mavlink_message_t* msg, mavlink_quad_swarm_roll_pitch_yaw_thrust_t* quad_swarm_roll_pitch_yaw_thrust)
{
#if MAVLINK_NEED_BYTE_SWAP
	mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_get_roll(msg, quad_swarm_roll_pitch_yaw_thrust->roll);
	mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_get_pitch(msg, quad_swarm_roll_pitch_yaw_thrust->pitch);
	mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_get_yaw(msg, quad_swarm_roll_pitch_yaw_thrust->yaw);
	mavlink_msg_quad_swarm_roll_pitch_yaw_thrust_get_thrust(msg, quad_swarm_roll_pitch_yaw_thrust->thrust);
#else
	memcpy(quad_swarm_roll_pitch_yaw_thrust, _MAV_PAYLOAD(msg), 56);
#endif
}
