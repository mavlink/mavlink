// MESSAGE VISION_SPEED_ESTIMATE PACKING

#define MAVLINK_MSG_ID_VISION_SPEED_ESTIMATE 158

typedef struct __mavlink_vision_speed_estimate_t
{
 uint64_t usec; ///< Timestamp (milliseconds)
 float x; ///< Global X speed
 float y; ///< Global Y speed
 float z; ///< Global Z speed
} mavlink_vision_speed_estimate_t;

/**
 * @brief Pack a vision_speed_estimate message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param usec Timestamp (milliseconds)
 * @param x Global X speed
 * @param y Global Y speed
 * @param z Global Z speed
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_vision_speed_estimate_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint64_t usec, float x, float y, float z)
{
	msg->msgid = MAVLINK_MSG_ID_VISION_SPEED_ESTIMATE;

	put_uint64_t_by_index(usec, 0,  msg->payload); // Timestamp (milliseconds)
	put_float_by_index(x, 8,  msg->payload); // Global X speed
	put_float_by_index(y, 12,  msg->payload); // Global Y speed
	put_float_by_index(z, 16,  msg->payload); // Global Z speed

	return mavlink_finalize_message(msg, system_id, component_id, 20, 45);
}

/**
 * @brief Pack a vision_speed_estimate message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param usec Timestamp (milliseconds)
 * @param x Global X speed
 * @param y Global Y speed
 * @param z Global Z speed
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_vision_speed_estimate_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint64_t usec,float x,float y,float z)
{
	msg->msgid = MAVLINK_MSG_ID_VISION_SPEED_ESTIMATE;

	put_uint64_t_by_index(usec, 0,  msg->payload); // Timestamp (milliseconds)
	put_float_by_index(x, 8,  msg->payload); // Global X speed
	put_float_by_index(y, 12,  msg->payload); // Global Y speed
	put_float_by_index(z, 16,  msg->payload); // Global Z speed

	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 20, 45);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

/**
 * @brief Pack a vision_speed_estimate message on a channel and send
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param usec Timestamp (milliseconds)
 * @param x Global X speed
 * @param y Global Y speed
 * @param z Global Z speed
 */
static inline void mavlink_msg_vision_speed_estimate_pack_chan_send(mavlink_channel_t chan,
							   mavlink_message_t* msg,
						           uint64_t usec,float x,float y,float z)
{
	msg->msgid = MAVLINK_MSG_ID_VISION_SPEED_ESTIMATE;

	put_uint64_t_by_index(usec, 0,  msg->payload); // Timestamp (milliseconds)
	put_float_by_index(x, 8,  msg->payload); // Global X speed
	put_float_by_index(y, 12,  msg->payload); // Global Y speed
	put_float_by_index(z, 16,  msg->payload); // Global Z speed

	mavlink_finalize_message_chan_send(msg, chan, 20, 45);
}
#endif // MAVLINK_USE_CONVENIENCE_FUNCTIONS


/**
 * @brief Encode a vision_speed_estimate struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param vision_speed_estimate C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_vision_speed_estimate_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_vision_speed_estimate_t* vision_speed_estimate)
{
	return mavlink_msg_vision_speed_estimate_pack(system_id, component_id, msg, vision_speed_estimate->usec, vision_speed_estimate->x, vision_speed_estimate->y, vision_speed_estimate->z);
}

/**
 * @brief Send a vision_speed_estimate message
 * @param chan MAVLink channel to send the message
 *
 * @param usec Timestamp (milliseconds)
 * @param x Global X speed
 * @param y Global Y speed
 * @param z Global Z speed
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_vision_speed_estimate_send(mavlink_channel_t chan, uint64_t usec, float x, float y, float z)
{
	MAVLINK_ALIGNED_MESSAGE(msg, 20);
	mavlink_msg_vision_speed_estimate_pack_chan_send(chan, msg, usec, x, y, z);
}

#endif

// MESSAGE VISION_SPEED_ESTIMATE UNPACKING


/**
 * @brief Get field usec from vision_speed_estimate message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint64_t mavlink_msg_vision_speed_estimate_get_usec(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field x from vision_speed_estimate message
 *
 * @return Global X speed
 */
static inline float mavlink_msg_vision_speed_estimate_get_x(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  8);
}

/**
 * @brief Get field y from vision_speed_estimate message
 *
 * @return Global Y speed
 */
static inline float mavlink_msg_vision_speed_estimate_get_y(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  12);
}

/**
 * @brief Get field z from vision_speed_estimate message
 *
 * @return Global Z speed
 */
static inline float mavlink_msg_vision_speed_estimate_get_z(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  16);
}

/**
 * @brief Decode a vision_speed_estimate message into a struct
 *
 * @param msg The message to decode
 * @param vision_speed_estimate C-struct to decode the message contents into
 */
static inline void mavlink_msg_vision_speed_estimate_decode(const mavlink_message_t* msg, mavlink_vision_speed_estimate_t* vision_speed_estimate)
{
#if MAVLINK_NEED_BYTE_SWAP
	vision_speed_estimate->usec = mavlink_msg_vision_speed_estimate_get_usec(msg);
	vision_speed_estimate->x = mavlink_msg_vision_speed_estimate_get_x(msg);
	vision_speed_estimate->y = mavlink_msg_vision_speed_estimate_get_y(msg);
	vision_speed_estimate->z = mavlink_msg_vision_speed_estimate_get_z(msg);
#else
	memcpy(vision_speed_estimate, msg->payload, 20);
#endif
}
