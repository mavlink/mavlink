// MESSAGE HEARTBEAT PACKING

#define MAVLINK_MSG_ID_HEARTBEAT 0

typedef struct __mavlink_heartbeat_t 
{
	uint8_t type; ///< Type of the MAV (quadrotor, helicopter, etc.)

} mavlink_heartbeat_t;



/**
 * @brief Send a heartbeat message
 *
 * @param type Type of the MAV (quadrotor, helicopter, etc.)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_heartbeat_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t type)
{
	msg->msgid = MAVLINK_MSG_ID_HEARTBEAT;
	uint16_t i = 0;

	i += put_uint8_t_by_index(type, i, msg->payload); //Type of the MAV (quadrotor, helicopter, etc.)

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_heartbeat_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_heartbeat_t* heartbeat)
{
	return mavlink_msg_heartbeat_pack(system_id, component_id, msg, heartbeat->type);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_heartbeat_send(mavlink_channel_t chan, uint8_t type)
{
	mavlink_message_t msg;
	mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &msg, type);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE HEARTBEAT UNPACKING

/**
 * @brief Get field type from heartbeat message
 *
 * @return Type of the MAV (quadrotor, helicopter, etc.)
 */
static inline uint8_t mavlink_msg_heartbeat_get_type(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

static inline void mavlink_msg_heartbeat_decode(const mavlink_message_t* msg, mavlink_heartbeat_t* heartbeat)
{
	heartbeat->type = mavlink_msg_heartbeat_get_type(msg);
}
