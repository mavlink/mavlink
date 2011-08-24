// MESSAGE WAYPOINT_REQUEST_LIST PACKING

#define MAVLINK_MSG_ID_WAYPOINT_REQUEST_LIST 43

typedef struct __mavlink_waypoint_request_list_t
{
 uint8_t target_system; ///< System ID
 uint8_t target_component; ///< Component ID
} mavlink_waypoint_request_list_t;

/**
 * @brief Pack a waypoint_request_list message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_request_list_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t target_system, uint8_t target_component)
{
	msg->msgid = MAVLINK_MSG_ID_WAYPOINT_REQUEST_LIST;

	put_uint8_t_by_index(target_system, 0,  msg->payload); // System ID
	put_uint8_t_by_index(target_component, 1,  msg->payload); // Component ID

	return mavlink_finalize_message(msg, system_id, component_id, 2, 68);
}

/**
 * @brief Pack a waypoint_request_list message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param target_component Component ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_request_list_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t target_system,uint8_t target_component)
{
	msg->msgid = MAVLINK_MSG_ID_WAYPOINT_REQUEST_LIST;

	put_uint8_t_by_index(target_system, 0,  msg->payload); // System ID
	put_uint8_t_by_index(target_component, 1,  msg->payload); // Component ID

	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 2, 68);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

/**
 * @brief Pack a waypoint_request_list message on a channel and send
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param target_component Component ID
 */
static inline void mavlink_msg_waypoint_request_list_pack_chan_send(mavlink_channel_t chan,
							   mavlink_message_t* msg,
						           uint8_t target_system,uint8_t target_component)
{
	msg->msgid = MAVLINK_MSG_ID_WAYPOINT_REQUEST_LIST;

	put_uint8_t_by_index(target_system, 0,  msg->payload); // System ID
	put_uint8_t_by_index(target_component, 1,  msg->payload); // Component ID

	mavlink_finalize_message_chan_send(msg, chan, 2, 68);
}
#endif // MAVLINK_USE_CONVENIENCE_FUNCTIONS


/**
 * @brief Encode a waypoint_request_list struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_request_list C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_request_list_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_request_list_t* waypoint_request_list)
{
	return mavlink_msg_waypoint_request_list_pack(system_id, component_id, msg, waypoint_request_list->target_system, waypoint_request_list->target_component);
}

/**
 * @brief Send a waypoint_request_list message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param target_component Component ID
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_request_list_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component)
{
	MAVLINK_ALIGNED_MESSAGE(msg, 2);
	mavlink_msg_waypoint_request_list_pack_chan_send(chan, msg, target_system, target_component);
}

#endif

// MESSAGE WAYPOINT_REQUEST_LIST UNPACKING


/**
 * @brief Get field target_system from waypoint_request_list message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_waypoint_request_list_get_target_system(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field target_component from waypoint_request_list message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_waypoint_request_list_get_target_component(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a waypoint_request_list message into a struct
 *
 * @param msg The message to decode
 * @param waypoint_request_list C-struct to decode the message contents into
 */
static inline void mavlink_msg_waypoint_request_list_decode(const mavlink_message_t* msg, mavlink_waypoint_request_list_t* waypoint_request_list)
{
#if MAVLINK_NEED_BYTE_SWAP
	waypoint_request_list->target_system = mavlink_msg_waypoint_request_list_get_target_system(msg);
	waypoint_request_list->target_component = mavlink_msg_waypoint_request_list_get_target_component(msg);
#else
	memcpy(waypoint_request_list, msg->payload, 2);
#endif
}
