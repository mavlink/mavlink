// MESSAGE LOCAL_POSITION PACKING

#define MAVLINK_MSG_ID_LOCAL_POSITION 31

typedef struct __mavlink_local_position_t
{
 uint64_t usec; ///< Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 float x; ///< X Position
 float y; ///< Y Position
 float z; ///< Z Position
 float vx; ///< X Speed
 float vy; ///< Y Speed
 float vz; ///< Z Speed
} mavlink_local_position_t;

/**
 * @brief Pack a local_position message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param usec Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param x X Position
 * @param y Y Position
 * @param z Z Position
 * @param vx X Speed
 * @param vy Y Speed
 * @param vz Z Speed
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_local_position_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint64_t usec, float x, float y, float z, float vx, float vy, float vz)
{
	msg->msgid = MAVLINK_MSG_ID_LOCAL_POSITION;

	put_uint64_t_by_index(usec, 0,  msg->payload); // Timestamp (microseconds since UNIX epoch or microseconds since system boot)
	put_float_by_index(x, 8,  msg->payload); // X Position
	put_float_by_index(y, 12,  msg->payload); // Y Position
	put_float_by_index(z, 16,  msg->payload); // Z Position
	put_float_by_index(vx, 20,  msg->payload); // X Speed
	put_float_by_index(vy, 24,  msg->payload); // Y Speed
	put_float_by_index(vz, 28,  msg->payload); // Z Speed

	return mavlink_finalize_message(msg, system_id, component_id, 32, 2);
}

/**
 * @brief Pack a local_position message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param usec Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param x X Position
 * @param y Y Position
 * @param z Z Position
 * @param vx X Speed
 * @param vy Y Speed
 * @param vz Z Speed
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_local_position_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint64_t usec,float x,float y,float z,float vx,float vy,float vz)
{
	msg->msgid = MAVLINK_MSG_ID_LOCAL_POSITION;

	put_uint64_t_by_index(usec, 0,  msg->payload); // Timestamp (microseconds since UNIX epoch or microseconds since system boot)
	put_float_by_index(x, 8,  msg->payload); // X Position
	put_float_by_index(y, 12,  msg->payload); // Y Position
	put_float_by_index(z, 16,  msg->payload); // Z Position
	put_float_by_index(vx, 20,  msg->payload); // X Speed
	put_float_by_index(vy, 24,  msg->payload); // Y Speed
	put_float_by_index(vz, 28,  msg->payload); // Z Speed

	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 32, 2);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

/**
 * @brief Pack a local_position message on a channel and send
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param usec Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param x X Position
 * @param y Y Position
 * @param z Z Position
 * @param vx X Speed
 * @param vy Y Speed
 * @param vz Z Speed
 */
static inline void mavlink_msg_local_position_pack_chan_send(mavlink_channel_t chan,
							   mavlink_message_t* msg,
						           uint64_t usec,float x,float y,float z,float vx,float vy,float vz)
{
	msg->msgid = MAVLINK_MSG_ID_LOCAL_POSITION;

	put_uint64_t_by_index(usec, 0,  msg->payload); // Timestamp (microseconds since UNIX epoch or microseconds since system boot)
	put_float_by_index(x, 8,  msg->payload); // X Position
	put_float_by_index(y, 12,  msg->payload); // Y Position
	put_float_by_index(z, 16,  msg->payload); // Z Position
	put_float_by_index(vx, 20,  msg->payload); // X Speed
	put_float_by_index(vy, 24,  msg->payload); // Y Speed
	put_float_by_index(vz, 28,  msg->payload); // Z Speed

	mavlink_finalize_message_chan_send(msg, chan, 32, 2);
}
#endif // MAVLINK_USE_CONVENIENCE_FUNCTIONS


/**
 * @brief Encode a local_position struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param local_position C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_local_position_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_local_position_t* local_position)
{
	return mavlink_msg_local_position_pack(system_id, component_id, msg, local_position->usec, local_position->x, local_position->y, local_position->z, local_position->vx, local_position->vy, local_position->vz);
}

/**
 * @brief Send a local_position message
 * @param chan MAVLink channel to send the message
 *
 * @param usec Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param x X Position
 * @param y Y Position
 * @param z Z Position
 * @param vx X Speed
 * @param vy Y Speed
 * @param vz Z Speed
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_local_position_send(mavlink_channel_t chan, uint64_t usec, float x, float y, float z, float vx, float vy, float vz)
{
	MAVLINK_ALIGNED_MESSAGE(msg, 32);
	mavlink_msg_local_position_pack_chan_send(chan, msg, usec, x, y, z, vx, vy, vz);
}

#endif

// MESSAGE LOCAL_POSITION UNPACKING


/**
 * @brief Get field usec from local_position message
 *
 * @return Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 */
static inline uint64_t mavlink_msg_local_position_get_usec(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field x from local_position message
 *
 * @return X Position
 */
static inline float mavlink_msg_local_position_get_x(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  8);
}

/**
 * @brief Get field y from local_position message
 *
 * @return Y Position
 */
static inline float mavlink_msg_local_position_get_y(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  12);
}

/**
 * @brief Get field z from local_position message
 *
 * @return Z Position
 */
static inline float mavlink_msg_local_position_get_z(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  16);
}

/**
 * @brief Get field vx from local_position message
 *
 * @return X Speed
 */
static inline float mavlink_msg_local_position_get_vx(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  20);
}

/**
 * @brief Get field vy from local_position message
 *
 * @return Y Speed
 */
static inline float mavlink_msg_local_position_get_vy(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  24);
}

/**
 * @brief Get field vz from local_position message
 *
 * @return Z Speed
 */
static inline float mavlink_msg_local_position_get_vz(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  28);
}

/**
 * @brief Decode a local_position message into a struct
 *
 * @param msg The message to decode
 * @param local_position C-struct to decode the message contents into
 */
static inline void mavlink_msg_local_position_decode(const mavlink_message_t* msg, mavlink_local_position_t* local_position)
{
#if MAVLINK_NEED_BYTE_SWAP
	local_position->usec = mavlink_msg_local_position_get_usec(msg);
	local_position->x = mavlink_msg_local_position_get_x(msg);
	local_position->y = mavlink_msg_local_position_get_y(msg);
	local_position->z = mavlink_msg_local_position_get_z(msg);
	local_position->vx = mavlink_msg_local_position_get_vx(msg);
	local_position->vy = mavlink_msg_local_position_get_vy(msg);
	local_position->vz = mavlink_msg_local_position_get_vz(msg);
#else
	memcpy(local_position, msg->payload, 32);
#endif
}
