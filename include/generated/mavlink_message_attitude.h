// MESSAGE ATTITUDE PACKING

#define MAVLINK_MSG_ID_ATTITUDE 90

typedef struct __attitude_t 
{
	uint64_t msec; ///< Timestamp (milliseconds)
	float roll; ///< Roll angle (rad)
	float pitch; ///< Pitch angle (rad)
	float yaw; ///< Yaw angle (rad)
	float rollspeed; ///< Roll angular speed (rad/s)
	float pitchspeed; ///< Pitch angular speed (rad/s)
	float yawspeed; ///< Yaw angular speed (rad/s)

} attitude_t;

/**
 * @brief Send a attitude message
 *
 * @param msec Timestamp (milliseconds)
 * @param roll Roll angle (rad)
 * @param pitch Pitch angle (rad)
 * @param yaw Yaw angle (rad)
 * @param rollspeed Roll angular speed (rad/s)
 * @param pitchspeed Pitch angular speed (rad/s)
 * @param yawspeed Yaw angular speed (rad/s)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_attitude_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint64_t msec, float roll, float pitch, float yaw, float rollspeed, float pitchspeed, float yawspeed)
{
	msg->msgid = MAVLINK_MSG_ID_ATTITUDE;
	uint16_t i = 0;

	i += put_uint64_t_by_index(msec, i, msg->payload); //Timestamp (milliseconds)
	i += put_float_by_index(roll, i, msg->payload); //Roll angle (rad)
	i += put_float_by_index(pitch, i, msg->payload); //Pitch angle (rad)
	i += put_float_by_index(yaw, i, msg->payload); //Yaw angle (rad)
	i += put_float_by_index(rollspeed, i, msg->payload); //Roll angular speed (rad/s)
	i += put_float_by_index(pitchspeed, i, msg->payload); //Pitch angular speed (rad/s)
	i += put_float_by_index(yawspeed, i, msg->payload); //Yaw angular speed (rad/s)

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_attitude_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const attitude_t* attitude)
{
	return message_attitude_pack(system_id, component_id, msg, attitude->msec, attitude->roll, attitude->pitch, attitude->yaw, attitude->rollspeed, attitude->pitchspeed, attitude->yawspeed);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_attitude_send(mavlink_channel_t chan, uint64_t msec, float roll, float pitch, float yaw, float rollspeed, float pitchspeed, float yawspeed)
{
	mavlink_message_t msg;
	message_attitude_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, msec, roll, pitch, yaw, rollspeed, pitchspeed, yawspeed);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE ATTITUDE UNPACKING

/**
 * @brief Get field msec from attitude message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint64_t message_attitude_get_msec(const mavlink_message_t* msg)
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
 * @brief Get field roll from attitude message
 *
 * @return Roll angle (rad)
 */
static inline float message_attitude_get_roll(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field pitch from attitude message
 *
 * @return Pitch angle (rad)
 */
static inline float message_attitude_get_pitch(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field yaw from attitude message
 *
 * @return Yaw angle (rad)
 */
static inline float message_attitude_get_yaw(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field rollspeed from attitude message
 *
 * @return Roll angular speed (rad/s)
 */
static inline float message_attitude_get_rollspeed(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field pitchspeed from attitude message
 *
 * @return Pitch angular speed (rad/s)
 */
static inline float message_attitude_get_pitchspeed(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field yawspeed from attitude message
 *
 * @return Yaw angular speed (rad/s)
 */
static inline float message_attitude_get_yawspeed(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

static inline void message_attitude_decode(const mavlink_message_t* msg, attitude_t* attitude)
{
	attitude->msec = message_attitude_get_msec(msg);
	attitude->roll = message_attitude_get_roll(msg);
	attitude->pitch = message_attitude_get_pitch(msg);
	attitude->yaw = message_attitude_get_yaw(msg);
	attitude->rollspeed = message_attitude_get_rollspeed(msg);
	attitude->pitchspeed = message_attitude_get_pitchspeed(msg);
	attitude->yawspeed = message_attitude_get_yawspeed(msg);
}
