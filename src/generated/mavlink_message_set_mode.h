// MESSAGE SET_MODE PACKING

#define MAVLINK_MSG_ID_SET_MODE 21

typedef struct __set_mode_t 
{
	uint8_t target; ///< The system setting the mode
	uint8_t mode; ///< The new mode

} set_mode_t;

/**
 * @brief Send a set_mode message
 *
 * @param target The system setting the mode
 * @param mode The new mode
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_set_mode_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target, uint8_t mode)
{
	msg->msgid = MAVLINK_MSG_ID_SET_MODE;
	uint16_t i = 0;

	i += put_uint8_t_by_index(target, i, msg->payload); //The system setting the mode
	i += put_uint8_t_by_index(mode, i, msg->payload); //The new mode

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_set_mode_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const set_mode_t* set_mode)
{
	return message_set_mode_pack(system_id, component_id, msg, set_mode->target, set_mode->mode);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_set_mode_send(mavlink_channel_t chan, uint8_t target, uint8_t mode)
{
	mavlink_message_t msg;
	message_set_mode_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, target, mode);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE SET_MODE UNPACKING

/**
 * @brief Get field target from set_mode message
 *
 * @return The system setting the mode
 */
static inline uint8_t message_set_mode_get_target(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field mode from set_mode message
 *
 * @return The new mode
 */
static inline uint8_t message_set_mode_get_mode(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t))[0];
}

static inline void message_set_mode_decode(const mavlink_message_t* msg, set_mode_t* set_mode)
{
	set_mode->target = message_set_mode_get_target(msg);
	set_mode->mode = message_set_mode_get_mode(msg);
}
