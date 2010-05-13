// MESSAGE WAYPOINT_SET_ACTIVE PACKING

#define MAVLINK_MSG_ID_WAYPOINT_SET_ACTIVE 95

typedef struct __mavlink_waypoint_set_active_t 
{
	uint8_t target_system; ///< System ID
	uint8_t target_component; ///< Component ID
	uint16_t id; ///< ID

} mavlink_waypoint_set_active_t;



/**
 * @brief Send a waypoint_set_active message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param id ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_set_active_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target_system, uint8_t target_component, uint16_t id)
{
	msg->msgid = MAVLINK_MSG_ID_WAYPOINT_SET_ACTIVE;
	uint16_t i = 0;

	i += put_uint8_t_by_index(target_system, i, msg->payload); //System ID
	i += put_uint8_t_by_index(target_component, i, msg->payload); //Component ID
	i += put_uint16_t_by_index(id, i, msg->payload); //ID

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_waypoint_set_active_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_set_active_t* waypoint_set_active)
{
	return mavlink_msg_waypoint_set_active_pack(system_id, component_id, msg, waypoint_set_active->target_system, waypoint_set_active->target_component, waypoint_set_active->id);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_set_active_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint16_t id)
{
	mavlink_message_t msg;
	mavlink_msg_waypoint_set_active_pack(mavlink_system.sysid, mavlink_system.compid, &msg, target_system, target_component, id);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE WAYPOINT_SET_ACTIVE UNPACKING

/**
 * @brief Get field target_system from waypoint_set_active message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_waypoint_set_active_get_target_system(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field target_component from waypoint_set_active message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_waypoint_set_active_get_target_component(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t))[0];
}

/**
 * @brief Get field id from waypoint_set_active message
 *
 * @return ID
 */
static inline uint16_t mavlink_msg_waypoint_set_active_get_id(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t))[0];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t))[1];
	return (uint16_t)r.s;
}

static inline void mavlink_msg_waypoint_set_active_decode(const mavlink_message_t* msg, mavlink_waypoint_set_active_t* waypoint_set_active)
{
	waypoint_set_active->target_system = mavlink_msg_waypoint_set_active_get_target_system(msg);
	waypoint_set_active->target_component = mavlink_msg_waypoint_set_active_get_target_component(msg);
	waypoint_set_active->id = mavlink_msg_waypoint_set_active_get_id(msg);
}
