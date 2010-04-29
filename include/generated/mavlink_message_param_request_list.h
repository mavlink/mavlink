// MESSAGE PARAM_REQUEST_LIST PACKING

#define MAVLINK_MSG_ID_PARAM_REQUEST_LIST 81

typedef struct __param_request_list_t 
{

} param_request_list_t;

/**
 * @brief Send a param_request_list message
 *
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_param_request_list_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg)
{
	msg->msgid = MAVLINK_MSG_ID_PARAM_REQUEST_LIST;
	uint16_t i = 0;


	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_param_request_list_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const param_request_list_t* param_request_list)
{
	return message_param_request_list_pack(system_id, component_id, msg);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_param_request_list_send(mavlink_channel_t chan)
{
	mavlink_message_t msg;
	message_param_request_list_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE PARAM_REQUEST_LIST UNPACKING

static inline void message_param_request_list_decode(const mavlink_message_t* msg, param_request_list_t* param_request_list)
{
}
