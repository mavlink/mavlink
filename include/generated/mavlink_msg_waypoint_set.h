// MESSAGE WAYPOINT_SET PACKING

#define MAVLINK_MSG_ID_WAYPOINT_SET 94

typedef struct __mavlink_waypoint_set_t 
{
	uint8_t target_system; ///< System ID
	uint8_t target_component; ///< Component ID
	uint16_t id; ///< ID
	uint16_t priority; ///< Priority
	float x; ///< x position
	float y; ///< y position
	float z; ///< z position
	float yaw; ///< yaw orientation in radians, 0 = NORTH
	uint16_t autocontinue; ///< autocontinue to next wp

} mavlink_waypoint_set_t;



/**
 * @brief Send a waypoint_set message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param id ID
 * @param priority Priority
 * @param x x position
 * @param y y position
 * @param z z position
 * @param yaw yaw orientation in radians, 0 = NORTH
 * @param autocontinue autocontinue to next wp
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_set_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target_system, uint8_t target_component, uint16_t id, uint16_t priority, float x, float y, float z, float yaw, uint16_t autocontinue)
{
	msg->msgid = MAVLINK_MSG_ID_WAYPOINT_SET;
	uint16_t i = 0;

	i += put_uint8_t_by_index(target_system, i, msg->payload); //System ID
	i += put_uint8_t_by_index(target_component, i, msg->payload); //Component ID
	i += put_uint16_t_by_index(id, i, msg->payload); //ID
	i += put_uint16_t_by_index(priority, i, msg->payload); //Priority
	i += put_float_by_index(x, i, msg->payload); //x position
	i += put_float_by_index(y, i, msg->payload); //y position
	i += put_float_by_index(z, i, msg->payload); //z position
	i += put_float_by_index(yaw, i, msg->payload); //yaw orientation in radians, 0 = NORTH
	i += put_uint16_t_by_index(autocontinue, i, msg->payload); //autocontinue to next wp

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_waypoint_set_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_set_t* waypoint_set)
{
	return mavlink_msg_waypoint_set_pack(system_id, component_id, msg, waypoint_set->target_system, waypoint_set->target_component, waypoint_set->id, waypoint_set->priority, waypoint_set->x, waypoint_set->y, waypoint_set->z, waypoint_set->yaw, waypoint_set->autocontinue);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_set_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint16_t id, uint16_t priority, float x, float y, float z, float yaw, uint16_t autocontinue)
{
	mavlink_message_t msg;
	mavlink_msg_waypoint_set_pack(mavlink_system.sysid, mavlink_system.compid, &msg, target_system, target_component, id, priority, x, y, z, yaw, autocontinue);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE WAYPOINT_SET UNPACKING

/**
 * @brief Get field target_system from waypoint_set message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_waypoint_set_get_target_system(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field target_component from waypoint_set message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_waypoint_set_get_target_component(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t))[0];
}

/**
 * @brief Get field id from waypoint_set message
 *
 * @return ID
 */
static inline uint16_t mavlink_msg_waypoint_set_get_id(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t))[0];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field priority from waypoint_set message
 *
 * @return Priority
 */
static inline uint16_t mavlink_msg_waypoint_set_get_priority(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field x from waypoint_set message
 *
 * @return x position
 */
static inline float mavlink_msg_waypoint_set_get_x(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field y from waypoint_set message
 *
 * @return y position
 */
static inline float mavlink_msg_waypoint_set_get_y(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field z from waypoint_set message
 *
 * @return z position
 */
static inline float mavlink_msg_waypoint_set_get_z(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field yaw from waypoint_set message
 *
 * @return yaw orientation in radians, 0 = NORTH
 */
static inline float mavlink_msg_waypoint_set_get_yaw(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field autocontinue from waypoint_set message
 *
 * @return autocontinue to next wp
 */
static inline uint16_t mavlink_msg_waypoint_set_get_autocontinue(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	return (uint16_t)r.s;
}

static inline void mavlink_msg_waypoint_set_decode(const mavlink_message_t* msg, mavlink_waypoint_set_t* waypoint_set)
{
	waypoint_set->target_system = mavlink_msg_waypoint_set_get_target_system(msg);
	waypoint_set->target_component = mavlink_msg_waypoint_set_get_target_component(msg);
	waypoint_set->id = mavlink_msg_waypoint_set_get_id(msg);
	waypoint_set->priority = mavlink_msg_waypoint_set_get_priority(msg);
	waypoint_set->x = mavlink_msg_waypoint_set_get_x(msg);
	waypoint_set->y = mavlink_msg_waypoint_set_get_y(msg);
	waypoint_set->z = mavlink_msg_waypoint_set_get_z(msg);
	waypoint_set->yaw = mavlink_msg_waypoint_set_get_yaw(msg);
	waypoint_set->autocontinue = mavlink_msg_waypoint_set_get_autocontinue(msg);
}
