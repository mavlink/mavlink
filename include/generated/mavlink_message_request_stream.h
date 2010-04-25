// MESSAGE REQUEST_STREAM PACKING

#define MAVLINK_MSG_ID_REQUEST_STREAM 23

typedef struct __request_stream_t 
{
	uint8_t target_system; ///< The target requested to send the message stream.
	uint8_t target_component; ///< The target requested to send the message stream.
	uint8_t req_message_id; ///< The ID of the requested message type
	uint16_t req_message_rate; ///< The requested interval between two messages of this type
	uint8_t start_stop; ///< 1 to start sending, 0 to stop sending.

} request_stream_t;

/**
 * @brief Send a request_stream message
 *
 * @param target_system The target requested to send the message stream.
 * @param target_component The target requested to send the message stream.
 * @param req_message_id The ID of the requested message type
 * @param req_message_rate The requested interval between two messages of this type
 * @param start_stop 1 to start sending, 0 to stop sending.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_request_stream_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target_system, uint8_t target_component, uint8_t req_message_id, uint16_t req_message_rate, uint8_t start_stop)
{
	msg->msgid = MAVLINK_MSG_ID_REQUEST_STREAM;
	uint16_t i = 0;

	i += put_uint8_t_by_index(target_system, i, msg->payload); //The target requested to send the message stream.
	i += put_uint8_t_by_index(target_component, i, msg->payload); //The target requested to send the message stream.
	i += put_uint8_t_by_index(req_message_id, i, msg->payload); //The ID of the requested message type
	i += put_uint16_t_by_index(req_message_rate, i, msg->payload); //The requested interval between two messages of this type
	i += put_uint8_t_by_index(start_stop, i, msg->payload); //1 to start sending, 0 to stop sending.

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_request_stream_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const request_stream_t* request_stream)
{
	return message_request_stream_pack(system_id, component_id, msg, request_stream->target_system, request_stream->target_component, request_stream->req_message_id, request_stream->req_message_rate, request_stream->start_stop);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_request_stream_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint8_t req_message_id, uint16_t req_message_rate, uint8_t start_stop)
{
	mavlink_message_t msg;
	message_request_stream_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, target_system, target_component, req_message_id, req_message_rate, start_stop);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE REQUEST_STREAM UNPACKING

/**
 * @brief Get field target_system from request_stream message
 *
 * @return The target requested to send the message stream.
 */
static inline uint8_t message_request_stream_get_target_system(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field target_component from request_stream message
 *
 * @return The target requested to send the message stream.
 */
static inline uint8_t message_request_stream_get_target_component(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t))[0];
}

/**
 * @brief Get field req_message_id from request_stream message
 *
 * @return The ID of the requested message type
 */
static inline uint8_t message_request_stream_get_req_message_id(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t)+sizeof(uint8_t))[0];
}

/**
 * @brief Get field req_message_rate from request_stream message
 *
 * @return The requested interval between two messages of this type
 */
static inline uint16_t message_request_stream_get_req_message_rate(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[0];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field start_stop from request_stream message
 *
 * @return 1 to start sending, 0 to stop sending.
 */
static inline uint8_t message_request_stream_get_start_stop(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t))[0];
}

static inline void message_request_stream_decode(const mavlink_message_t* msg, request_stream_t* request_stream)
{
	request_stream->target_system = message_request_stream_get_target_system(msg);
	request_stream->target_component = message_request_stream_get_target_component(msg);
	request_stream->req_message_id = message_request_stream_get_req_message_id(msg);
	request_stream->req_message_rate = message_request_stream_get_req_message_rate(msg);
	request_stream->start_stop = message_request_stream_get_start_stop(msg);
}
