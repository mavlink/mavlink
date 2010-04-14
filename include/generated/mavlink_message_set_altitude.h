// MESSAGE SET_ALTITUDE PACKING

#define MAVLINK_MSG_ID_SET_ALTITUDE 22

typedef struct __set_altitude_t 
{
	uint8_t target; ///< The system setting the altitude
	uint32_t mode; ///< The new altitude in meters

} set_altitude_t;

/**
 * @brief Send a set_altitude message
 *
 * @param target The system setting the altitude
 * @param mode The new altitude in meters
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_set_altitude_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target, uint32_t mode)
{
	msg->msgid = MAVLINK_MSG_ID_SET_ALTITUDE;
	uint16_t i = 0;

	i += put_uint8_t_by_index(target, i, msg->payload); //The system setting the altitude
	i += put_uint32_t_by_index(mode, i, msg->payload); //The new altitude in meters

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_set_altitude_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const set_altitude_t* set_altitude)
{
	return message_set_altitude_pack(system_id, component_id, msg, set_altitude->target, set_altitude->mode);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_set_altitude_send(mavlink_channel_t chan, uint8_t target, uint32_t mode)
{
	mavlink_message_t msg;
	message_set_altitude_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, target, mode);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE SET_ALTITUDE UNPACKING

/**
 * @brief Get field target from set_altitude message
 *
 * @return The system setting the altitude
 */
static inline uint8_t message_set_altitude_get_target(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field mode from set_altitude message
 *
 * @return The new altitude in meters
 */
static inline uint32_t message_set_altitude_get_mode(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t))[3];
	return (uint32_t)r.i;
}

static inline void message_set_altitude_decode(const mavlink_message_t* msg, set_altitude_t* set_altitude)
{
	set_altitude->target = message_set_altitude_get_target(msg);
	set_altitude->mode = message_set_altitude_get_mode(msg);
}
