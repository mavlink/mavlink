// MESSAGE ACTION PACKING

#define MAVLINK_MSG_ID_ACTION 20

typedef struct __action_t 
{
	uint8_t target; ///< The system executing the action
	uint8_t action; ///< The action id

} action_t;

/**
 * @brief Send a action message
 *
 * @param target The system executing the action
 * @param action The action id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_action_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target, uint8_t action)
{
	msg->msgid = MAVLINK_MSG_ID_ACTION;
	uint16_t i = 0;

	i += put_uint8_t_by_index(target, i, msg->payload); //The system executing the action
	i += put_uint8_t_by_index(action, i, msg->payload); //The action id

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_action_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const action_t* action)
{
	return message_action_pack(system_id, component_id, msg, action->target, action->action);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_action_send(mavlink_channel_t chan, uint8_t target, uint8_t action)
{
	mavlink_message_t msg;
	message_action_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, target, action);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE ACTION UNPACKING

/**
 * @brief Get field target from action message
 *
 * @return The system executing the action
 */
static inline uint8_t message_action_get_target(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field action from action message
 *
 * @return The action id
 */
static inline uint8_t message_action_get_action(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t))[0];
}

static inline void message_action_decode(const mavlink_message_t* msg, action_t* action)
{
	action->target = message_action_get_target(msg);
	action->action = message_action_get_action(msg);
}
