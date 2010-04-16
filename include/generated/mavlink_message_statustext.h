// MESSAGE STATUSTEXT PACKING

#define MAVLINK_MSG_ID_STATUSTEXT 254

typedef struct __statustext_t 
{
	uint8_t severity; ///< Severity of status, 0 = info message, 255 = critical fault
	int8_t text[254]; ///< Status text message, without null termination character

} statustext_t;

/**
 * @brief Send a statustext message
 *
 * @param severity Severity of status, 0 = info message, 255 = critical fault
 * @param text Status text message, without null termination character
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_statustext_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t severity, const int8_t* text)
{
	msg->msgid = MAVLINK_MSG_ID_STATUSTEXT;
	uint16_t i = 0;

	i += put_uint8_t_by_index(severity, i, msg->payload); //Severity of status, 0 = info message, 255 = critical fault
	i += put_array_by_index(text, 254, i, msg->payload); //Status text message, without null termination character

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_statustext_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const statustext_t* statustext)
{
	return message_statustext_pack(system_id, component_id, msg, statustext->severity, statustext->text);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_statustext_send(mavlink_channel_t chan, uint8_t severity, const int8_t* text)
{
	mavlink_message_t msg;
	message_statustext_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, severity, text);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE STATUSTEXT UNPACKING

/**
 * @brief Get field severity from statustext message
 *
 * @return Severity of status, 0 = info message, 255 = critical fault
 */
static inline uint8_t message_statustext_get_severity(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field text from statustext message
 *
 * @return Status text message, without null termination character
 */
static inline uint16_t message_statustext_get_text(const mavlink_message_t* msg, int8_t* r_data)
{

	memcpy(r_data, msg->payload+sizeof(uint8_t), 254);
	return 254;
}

static inline void message_statustext_decode(const mavlink_message_t* msg, statustext_t* statustext)
{
	statustext->severity = message_statustext_get_severity(msg);
	message_statustext_get_text(msg, statustext->text);
}
