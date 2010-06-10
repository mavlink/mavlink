// MESSAGE VISION_POSITION_ESTIMATE PACKING

#define MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE 111

typedef struct __mavlink_vision_position_estimate_t 
{
	uint64_t usec; ///< Timestamp (milliseconds)
	float x; ///< Global X position
	float y; ///< Global Y position
	float z; ///< Global Z position
	float roll; ///< Roll angle in rad
	float pitch; ///< Pitch angle in rad
	float yaw; ///< Yaw angle in rad
	float vx; ///< Global X speed
	float vy; ///< Global Y speed
	float vz; ///< Global Z speed
	float vyaw; ///< Yaw angular speed in rad
	uint8_t confidence; ///< Confidence about the position. 0 = 0%, 255 = 100%

} mavlink_vision_position_estimate_t;



/**
 * @brief Send a vision_position_estimate message
 *
 * @param usec Timestamp (milliseconds)
 * @param x Global X position
 * @param y Global Y position
 * @param z Global Z position
 * @param roll Roll angle in rad
 * @param pitch Pitch angle in rad
 * @param yaw Yaw angle in rad
 * @param vx Global X speed
 * @param vy Global Y speed
 * @param vz Global Z speed
 * @param vyaw Yaw angular speed in rad
 * @param confidence Confidence about the position. 0 = 0%, 255 = 100%
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_vision_position_estimate_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint64_t usec, float x, float y, float z, float roll, float pitch, float yaw, float vx, float vy, float vz, float vyaw, uint8_t confidence)
{
	msg->msgid = MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE;
	uint16_t i = 0;

	i += put_uint64_t_by_index(usec, i, msg->payload); //Timestamp (milliseconds)
	i += put_float_by_index(x, i, msg->payload); //Global X position
	i += put_float_by_index(y, i, msg->payload); //Global Y position
	i += put_float_by_index(z, i, msg->payload); //Global Z position
	i += put_float_by_index(roll, i, msg->payload); //Roll angle in rad
	i += put_float_by_index(pitch, i, msg->payload); //Pitch angle in rad
	i += put_float_by_index(yaw, i, msg->payload); //Yaw angle in rad
	i += put_float_by_index(vx, i, msg->payload); //Global X speed
	i += put_float_by_index(vy, i, msg->payload); //Global Y speed
	i += put_float_by_index(vz, i, msg->payload); //Global Z speed
	i += put_float_by_index(vyaw, i, msg->payload); //Yaw angular speed in rad
	i += put_uint8_t_by_index(confidence, i, msg->payload); //Confidence about the position. 0 = 0%, 255 = 100%

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_vision_position_estimate_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_vision_position_estimate_t* vision_position_estimate)
{
	return mavlink_msg_vision_position_estimate_pack(system_id, component_id, msg, vision_position_estimate->usec, vision_position_estimate->x, vision_position_estimate->y, vision_position_estimate->z, vision_position_estimate->roll, vision_position_estimate->pitch, vision_position_estimate->yaw, vision_position_estimate->vx, vision_position_estimate->vy, vision_position_estimate->vz, vision_position_estimate->vyaw, vision_position_estimate->confidence);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_vision_position_estimate_send(mavlink_channel_t chan, uint64_t usec, float x, float y, float z, float roll, float pitch, float yaw, float vx, float vy, float vz, float vyaw, uint8_t confidence)
{
	mavlink_message_t msg;
	mavlink_msg_vision_position_estimate_pack(mavlink_system.sysid, mavlink_system.compid, &msg, usec, x, y, z, roll, pitch, yaw, vx, vy, vz, vyaw, confidence);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE VISION_POSITION_ESTIMATE UNPACKING

/**
 * @brief Get field usec from vision_position_estimate message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint64_t mavlink_msg_vision_position_estimate_get_usec(const mavlink_message_t* msg)
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
 * @brief Get field x from vision_position_estimate message
 *
 * @return Global X position
 */
static inline float mavlink_msg_vision_position_estimate_get_x(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field y from vision_position_estimate message
 *
 * @return Global Y position
 */
static inline float mavlink_msg_vision_position_estimate_get_y(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field z from vision_position_estimate message
 *
 * @return Global Z position
 */
static inline float mavlink_msg_vision_position_estimate_get_z(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field roll from vision_position_estimate message
 *
 * @return Roll angle in rad
 */
static inline float mavlink_msg_vision_position_estimate_get_roll(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field pitch from vision_position_estimate message
 *
 * @return Pitch angle in rad
 */
static inline float mavlink_msg_vision_position_estimate_get_pitch(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field yaw from vision_position_estimate message
 *
 * @return Yaw angle in rad
 */
static inline float mavlink_msg_vision_position_estimate_get_yaw(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vx from vision_position_estimate message
 *
 * @return Global X speed
 */
static inline float mavlink_msg_vision_position_estimate_get_vx(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vy from vision_position_estimate message
 *
 * @return Global Y speed
 */
static inline float mavlink_msg_vision_position_estimate_get_vy(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vz from vision_position_estimate message
 *
 * @return Global Z speed
 */
static inline float mavlink_msg_vision_position_estimate_get_vz(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vyaw from vision_position_estimate message
 *
 * @return Yaw angular speed in rad
 */
static inline float mavlink_msg_vision_position_estimate_get_vyaw(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field confidence from vision_position_estimate message
 *
 * @return Confidence about the position. 0 = 0%, 255 = 100%
 */
static inline uint8_t mavlink_msg_vision_position_estimate_get_confidence(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
}

static inline void mavlink_msg_vision_position_estimate_decode(const mavlink_message_t* msg, mavlink_vision_position_estimate_t* vision_position_estimate)
{
	vision_position_estimate->usec = mavlink_msg_vision_position_estimate_get_usec(msg);
	vision_position_estimate->x = mavlink_msg_vision_position_estimate_get_x(msg);
	vision_position_estimate->y = mavlink_msg_vision_position_estimate_get_y(msg);
	vision_position_estimate->z = mavlink_msg_vision_position_estimate_get_z(msg);
	vision_position_estimate->roll = mavlink_msg_vision_position_estimate_get_roll(msg);
	vision_position_estimate->pitch = mavlink_msg_vision_position_estimate_get_pitch(msg);
	vision_position_estimate->yaw = mavlink_msg_vision_position_estimate_get_yaw(msg);
	vision_position_estimate->vx = mavlink_msg_vision_position_estimate_get_vx(msg);
	vision_position_estimate->vy = mavlink_msg_vision_position_estimate_get_vy(msg);
	vision_position_estimate->vz = mavlink_msg_vision_position_estimate_get_vz(msg);
	vision_position_estimate->vyaw = mavlink_msg_vision_position_estimate_get_vyaw(msg);
	vision_position_estimate->confidence = mavlink_msg_vision_position_estimate_get_confidence(msg);
}
