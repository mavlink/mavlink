// MESSAGE POSITION PACKING

#define MAVLINK_MSG_ID_POSITION 91

typedef struct __mavlink_position_t 
{
	uint64_t msec; ///< Timestamp (milliseconds)
	float x; ///< X Position
	float y; ///< Y Position
	float z; ///< Z Position
	float vx; ///< X Speed
	float vy; ///< Y Speed
	float vz; ///< Z Speed

} mavlink_position_t;



/**
 * @brief Send a position message
 *
 * @param msec Timestamp (milliseconds)
 * @param x X Position
 * @param y Y Position
 * @param z Z Position
 * @param vx X Speed
 * @param vy Y Speed
 * @param vz Z Speed
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_position_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint64_t msec, float x, float y, float z, float vx, float vy, float vz)
{
	msg->msgid = MAVLINK_MSG_ID_POSITION;
	uint16_t i = 0;

	i += put_uint64_t_by_index(msec, i, msg->payload); //Timestamp (milliseconds)
	i += put_float_by_index(x, i, msg->payload); //X Position
	i += put_float_by_index(y, i, msg->payload); //Y Position
	i += put_float_by_index(z, i, msg->payload); //Z Position
	i += put_float_by_index(vx, i, msg->payload); //X Speed
	i += put_float_by_index(vy, i, msg->payload); //Y Speed
	i += put_float_by_index(vz, i, msg->payload); //Z Speed

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_position_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_position_t* position)
{
	return mavlink_msg_position_pack(system_id, component_id, msg, position->msec, position->x, position->y, position->z, position->vx, position->vy, position->vz);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_position_send(mavlink_channel_t chan, uint64_t msec, float x, float y, float z, float vx, float vy, float vz)
{
	mavlink_message_t msg;
	mavlink_msg_position_pack(mavlink_system.sysid, mavlink_system.compid, &msg, msec, x, y, z, vx, vy, vz);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE POSITION UNPACKING

/**
 * @brief Get field msec from position message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint64_t mavlink_msg_position_get_msec(const mavlink_message_t* msg)
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
 * @brief Get field x from position message
 *
 * @return X Position
 */
static inline float mavlink_msg_position_get_x(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field y from position message
 *
 * @return Y Position
 */
static inline float mavlink_msg_position_get_y(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field z from position message
 *
 * @return Z Position
 */
static inline float mavlink_msg_position_get_z(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vx from position message
 *
 * @return X Speed
 */
static inline float mavlink_msg_position_get_vx(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vy from position message
 *
 * @return Y Speed
 */
static inline float mavlink_msg_position_get_vy(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vz from position message
 *
 * @return Z Speed
 */
static inline float mavlink_msg_position_get_vz(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

static inline void mavlink_msg_position_decode(const mavlink_message_t* msg, mavlink_position_t* position)
{
	position->msec = mavlink_msg_position_get_msec(msg);
	position->x = mavlink_msg_position_get_x(msg);
	position->y = mavlink_msg_position_get_y(msg);
	position->z = mavlink_msg_position_get_z(msg);
	position->vx = mavlink_msg_position_get_vx(msg);
	position->vy = mavlink_msg_position_get_vy(msg);
	position->vz = mavlink_msg_position_get_vz(msg);
}
