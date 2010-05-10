// MESSAGE VISION_POSITION_ESTIMATE PACKING

#define MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE 91

typedef struct __mavlink_vision_position_estimate_t 
{
	uint64_t usec; ///< Timestamp (milliseconds)
	float x; ///< Global X Position
	float y; ///< Global Y Position
	float z; ///< Global Z Position
	float r1; ///< World-to-body rotation matrix first column, first row
	float r2; ///< Rotation matrix second column, first row
	float r3; ///< Rotation matrix third column, first row
	float r4; ///< Rotation matrix first column, second row
	float r5; ///< Rotation matrix second column, second row
	float r6; ///< Rotation matrix third column, second row
	float r7; ///< Rotation matrix first column, third row
	float r8; ///< Rotation matrix second column, third row
	float r9; ///< Rotation matrix third column, third row
	uint8_t confidence; ///< Confidence about the position. 0 = 0%, 255 = 100%

} mavlink_vision_position_estimate_t;



/**
 * @brief Send a vision_position_estimate message
 *
 * @param usec Timestamp (milliseconds)
 * @param x Global X Position
 * @param y Global Y Position
 * @param z Global Z Position
 * @param r1 World-to-body rotation matrix first column, first row
 * @param r2 Rotation matrix second column, first row
 * @param r3 Rotation matrix third column, first row
 * @param r4 Rotation matrix first column, second row
 * @param r5 Rotation matrix second column, second row
 * @param r6 Rotation matrix third column, second row
 * @param r7 Rotation matrix first column, third row
 * @param r8 Rotation matrix second column, third row
 * @param r9 Rotation matrix third column, third row
 * @param confidence Confidence about the position. 0 = 0%, 255 = 100%
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_vision_position_estimate_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint64_t usec, float x, float y, float z, float r1, float r2, float r3, float r4, float r5, float r6, float r7, float r8, float r9, uint8_t confidence)
{
	msg->msgid = MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE;
	uint16_t i = 0;

	i += put_uint64_t_by_index(usec, i, msg->payload); //Timestamp (milliseconds)
	i += put_float_by_index(x, i, msg->payload); //Global X Position
	i += put_float_by_index(y, i, msg->payload); //Global Y Position
	i += put_float_by_index(z, i, msg->payload); //Global Z Position
	i += put_float_by_index(r1, i, msg->payload); //World-to-body rotation matrix first column, first row
	i += put_float_by_index(r2, i, msg->payload); //Rotation matrix second column, first row
	i += put_float_by_index(r3, i, msg->payload); //Rotation matrix third column, first row
	i += put_float_by_index(r4, i, msg->payload); //Rotation matrix first column, second row
	i += put_float_by_index(r5, i, msg->payload); //Rotation matrix second column, second row
	i += put_float_by_index(r6, i, msg->payload); //Rotation matrix third column, second row
	i += put_float_by_index(r7, i, msg->payload); //Rotation matrix first column, third row
	i += put_float_by_index(r8, i, msg->payload); //Rotation matrix second column, third row
	i += put_float_by_index(r9, i, msg->payload); //Rotation matrix third column, third row
	i += put_uint8_t_by_index(confidence, i, msg->payload); //Confidence about the position. 0 = 0%, 255 = 100%

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_vision_position_estimate_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_vision_position_estimate_t* vision_position_estimate)
{
	return mavlink_msg_vision_position_estimate_pack(system_id, component_id, msg, vision_position_estimate->usec, vision_position_estimate->x, vision_position_estimate->y, vision_position_estimate->z, vision_position_estimate->r1, vision_position_estimate->r2, vision_position_estimate->r3, vision_position_estimate->r4, vision_position_estimate->r5, vision_position_estimate->r6, vision_position_estimate->r7, vision_position_estimate->r8, vision_position_estimate->r9, vision_position_estimate->confidence);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_vision_position_estimate_send(mavlink_channel_t chan, uint64_t usec, float x, float y, float z, float r1, float r2, float r3, float r4, float r5, float r6, float r7, float r8, float r9, uint8_t confidence)
{
	mavlink_message_t msg;
	mavlink_msg_vision_position_estimate_pack(mavlink_system.sysid, mavlink_system.compid, &msg, usec, x, y, z, r1, r2, r3, r4, r5, r6, r7, r8, r9, confidence);
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
 * @return Global X Position
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
 * @return Global Y Position
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
 * @return Global Z Position
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
 * @brief Get field r1 from vision_position_estimate message
 *
 * @return World-to-body rotation matrix first column, first row
 */
static inline float mavlink_msg_vision_position_estimate_get_r1(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field r2 from vision_position_estimate message
 *
 * @return Rotation matrix second column, first row
 */
static inline float mavlink_msg_vision_position_estimate_get_r2(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field r3 from vision_position_estimate message
 *
 * @return Rotation matrix third column, first row
 */
static inline float mavlink_msg_vision_position_estimate_get_r3(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field r4 from vision_position_estimate message
 *
 * @return Rotation matrix first column, second row
 */
static inline float mavlink_msg_vision_position_estimate_get_r4(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field r5 from vision_position_estimate message
 *
 * @return Rotation matrix second column, second row
 */
static inline float mavlink_msg_vision_position_estimate_get_r5(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field r6 from vision_position_estimate message
 *
 * @return Rotation matrix third column, second row
 */
static inline float mavlink_msg_vision_position_estimate_get_r6(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field r7 from vision_position_estimate message
 *
 * @return Rotation matrix first column, third row
 */
static inline float mavlink_msg_vision_position_estimate_get_r7(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field r8 from vision_position_estimate message
 *
 * @return Rotation matrix second column, third row
 */
static inline float mavlink_msg_vision_position_estimate_get_r8(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field r9 from vision_position_estimate message
 *
 * @return Rotation matrix third column, third row
 */
static inline float mavlink_msg_vision_position_estimate_get_r9(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field confidence from vision_position_estimate message
 *
 * @return Confidence about the position. 0 = 0%, 255 = 100%
 */
static inline uint8_t mavlink_msg_vision_position_estimate_get_confidence(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
}

static inline void mavlink_msg_vision_position_estimate_decode(const mavlink_message_t* msg, mavlink_vision_position_estimate_t* vision_position_estimate)
{
	vision_position_estimate->usec = mavlink_msg_vision_position_estimate_get_usec(msg);
	vision_position_estimate->x = mavlink_msg_vision_position_estimate_get_x(msg);
	vision_position_estimate->y = mavlink_msg_vision_position_estimate_get_y(msg);
	vision_position_estimate->z = mavlink_msg_vision_position_estimate_get_z(msg);
	vision_position_estimate->r1 = mavlink_msg_vision_position_estimate_get_r1(msg);
	vision_position_estimate->r2 = mavlink_msg_vision_position_estimate_get_r2(msg);
	vision_position_estimate->r3 = mavlink_msg_vision_position_estimate_get_r3(msg);
	vision_position_estimate->r4 = mavlink_msg_vision_position_estimate_get_r4(msg);
	vision_position_estimate->r5 = mavlink_msg_vision_position_estimate_get_r5(msg);
	vision_position_estimate->r6 = mavlink_msg_vision_position_estimate_get_r6(msg);
	vision_position_estimate->r7 = mavlink_msg_vision_position_estimate_get_r7(msg);
	vision_position_estimate->r8 = mavlink_msg_vision_position_estimate_get_r8(msg);
	vision_position_estimate->r9 = mavlink_msg_vision_position_estimate_get_r9(msg);
	vision_position_estimate->confidence = mavlink_msg_vision_position_estimate_get_confidence(msg);
}
