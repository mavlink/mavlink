// MESSAGE WAYPOINT_ERROR PACKING

#define MAVLINK_MSG_ID_WAYPOINT_ERROR 47

typedef struct __mavlink_waypoint_error_t 
{
	uint8_t target_system; ///< System ID
	uint8_t target_component; ///< Component ID
	uint8_t type; ///< 0: OK, 1: Error

} mavlink_waypoint_error_t;



/**
 * @brief Send a waypoint_error message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param type 0: OK, 1: Error
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_error_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target_system, uint8_t target_component, uint8_t type)
{
	msg->msgid = MAVLINK_MSG_ID_WAYPOINT_ERROR;
	uint16_t i = 0;

	i += put_uint8_t_by_index(target_system, i, msg->payload); //System ID
	i += put_uint8_t_by_index(target_component, i, msg->payload); //Component ID
	i += put_uint8_t_by_index(type, i, msg->payload); //0: OK, 1: Error

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_waypoint_error_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_error_t* waypoint_error)
{
	return mavlink_msg_waypoint_error_pack(system_id, component_id, msg, waypoint_error->target_system, waypoint_error->target_component, waypoint_error->type);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_error_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint8_t type)
{
	mavlink_message_t msg;
	mavlink_msg_waypoint_error_pack(mavlink_system.sysid, mavlink_system.compid, &msg, target_system, target_component, type);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE WAYPOINT_ERROR UNPACKING

/**
 * @brief Get field target_system from waypoint_error message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_waypoint_error_get_target_system(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field target_component from waypoint_error message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_waypoint_error_get_target_component(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t))[0];
}

/**
 * @brief Get field type from waypoint_error message
 *
 * @return 0: OK, 1: Error
 */
static inline uint8_t mavlink_msg_waypoint_error_get_type(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t)+sizeof(uint8_t))[0];
}

static inline void mavlink_msg_waypoint_error_decode(const mavlink_message_t* msg, mavlink_waypoint_error_t* waypoint_error)
{
	waypoint_error->target_system = mavlink_msg_waypoint_error_get_target_system(msg);
	waypoint_error->target_component = mavlink_msg_waypoint_error_get_target_component(msg);
	waypoint_error->type = mavlink_msg_waypoint_error_get_type(msg);
}
