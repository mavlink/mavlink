// MESSAGE AUX_STATUS PACKING

#define MAVLINK_MSG_ID_AUX_STATUS 104

typedef struct __aux_status_t 
{
	uint16_t load; ///< Maximum usage in percent of the mainloop time, should be always below 100

} aux_status_t;

/**
 * @brief Send a aux_status message
 *
 * @param load Maximum usage in percent of the mainloop time, should be always below 100
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_aux_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint16_t load)
{
	msg->msgid = MAVLINK_MSG_ID_AUX_STATUS;
	uint16_t i = 0;

	i += put_uint16_t_by_index(load, i, msg->payload); //Maximum usage in percent of the mainloop time, should be always below 100

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_aux_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const aux_status_t* aux_status)
{
	return message_aux_status_pack(system_id, component_id, msg, aux_status->load);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_aux_status_send(mavlink_channel_t chan, uint16_t load)
{
	mavlink_message_t msg;
	message_aux_status_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, load);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE AUX_STATUS UNPACKING

/**
 * @brief Get field load from aux_status message
 *
 * @return Maximum usage in percent of the mainloop time, should be always below 100
 */
static inline uint16_t message_aux_status_get_load(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload)[0];
	r.b[0] = (msg->payload)[1];
	return (uint16_t)r.s;
}

static inline void message_aux_status_decode(const mavlink_message_t* msg, aux_status_t* aux_status)
{
	aux_status->load = message_aux_status_get_load(msg);
}
