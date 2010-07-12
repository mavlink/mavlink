// MESSAGE LOCAL_POSITION PACKING

#define MAVLINK_MSG_ID_LOCAL_POSITION 31

typedef struct __mavlink_local_position_t 
{
	uint64_t usec; ///< Timestamp (microseconds since unix epoch)
	float x; ///< X Position
	float y; ///< Y Position
	float z; ///< Z Position
	float roll; ///< Roll
	float pitch; ///< Pitch
	float yaw; ///< Yaw

} mavlink_local_position_t;



/**
 * @brief Send a local_position message
 *
 * @param usec Timestamp (microseconds since unix epoch)
 * @param x X Position
 * @param y Y Position
 * @param z Z Position
 * @param roll Roll
 * @param pitch Pitch
 * @param yaw Yaw
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_local_position_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint64_t usec, float x, float y, float z, float roll, float pitch, float yaw)
{
	msg->msgid = MAVLINK_MSG_ID_LOCAL_POSITION;
	uint16_t i = 0;

	i += put_uint64_t_by_index(usec, i, msg->payload); //Timestamp (microseconds since unix epoch)
	i += put_float_by_index(x, i, msg->payload); //X Position
	i += put_float_by_index(y, i, msg->payload); //Y Position
	i += put_float_by_index(z, i, msg->payload); //Z Position
	i += put_float_by_index(roll, i, msg->payload); //Roll
	i += put_float_by_index(pitch, i, msg->payload); //Pitch
	i += put_float_by_index(yaw, i, msg->payload); //Yaw

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_local_position_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_local_position_t* local_position)
{
	return mavlink_msg_local_position_pack(system_id, component_id, msg, local_position->usec, local_position->x, local_position->y, local_position->z, local_position->roll, local_position->pitch, local_position->yaw);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_local_position_send(mavlink_channel_t chan, uint64_t usec, float x, float y, float z, float roll, float pitch, float yaw)
{
	mavlink_message_t msg;
	mavlink_msg_local_position_pack(mavlink_system.sysid, mavlink_system.compid, &msg, usec, x, y, z, roll, pitch, yaw);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE LOCAL_POSITION UNPACKING

/**
 * @brief Get field usec from local_position message
 *
 * @return Timestamp (microseconds since unix epoch)
 */
static inline uint64_t mavlink_msg_local_position_get_usec(const mavlink_message_t* msg)
{
	generic_64bit r;
	r.b[7] = (msg->payload)[0];
	r.b[6] = (msg->payload)[1];
	r.b[5] = (msg->payload)[2];
	r.b[4] = (msg->payload)[3];
	r.b[3] = (msg->payload)[4];
	r.b[2] = (msg->payload)[5];
	r.b[1] = (msg->payload)[6];
	r.b[0] = (msg->payload)[7];
	return (uint64_t)r.ll;
}

/**
 * @brief Get field x from local_position message
 *
 * @return X Position
 */
static inline float mavlink_msg_local_position_get_x(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field y from local_position message
 *
 * @return Y Position
 */
static inline float mavlink_msg_local_position_get_y(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field z from local_position message
 *
 * @return Z Position
 */
static inline float mavlink_msg_local_position_get_z(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field roll from local_position message
 *
 * @return Roll
 */
static inline float mavlink_msg_local_position_get_roll(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field pitch from local_position message
 *
 * @return Pitch
 */
static inline float mavlink_msg_local_position_get_pitch(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field yaw from local_position message
 *
 * @return Yaw
 */
static inline float mavlink_msg_local_position_get_yaw(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

static inline void mavlink_msg_local_position_decode(const mavlink_message_t* msg, mavlink_local_position_t* local_position)
{
	local_position->usec = mavlink_msg_local_position_get_usec(msg);
	local_position->x = mavlink_msg_local_position_get_x(msg);
	local_position->y = mavlink_msg_local_position_get_y(msg);
	local_position->z = mavlink_msg_local_position_get_z(msg);
	local_position->roll = mavlink_msg_local_position_get_roll(msg);
	local_position->pitch = mavlink_msg_local_position_get_pitch(msg);
	local_position->yaw = mavlink_msg_local_position_get_yaw(msg);
}
