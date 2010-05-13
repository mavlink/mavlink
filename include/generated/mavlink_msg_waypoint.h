// MESSAGE WAYPOINT PACKING

#define MAVLINK_MSG_ID_WAYPOINT 93

typedef struct __mavlink_waypoint_t 
{
	int8_t name[20]; ///< Name of the Waypoint
	uint16_t id; ///< ID
	uint16_t priority; ///< Priority
	float x; ///< x position
	float y; ///< y position
	float z; ///< z position
	float yaw; ///< yaw orientation in radians, 0 = NORTH
	uint16_t autocontinue; ///< autocontinue to next wp

} mavlink_waypoint_t;

#define MAVLINK_MSG_WAYPOINT_FIELD_NAME_LEN 20

/**
 * @brief Send a waypoint message
 *
 * @param name Name of the Waypoint
 * @param id ID
 * @param priority Priority
 * @param x x position
 * @param y y position
 * @param z z position
 * @param yaw yaw orientation in radians, 0 = NORTH
 * @param autocontinue autocontinue to next wp
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const int8_t* name, uint16_t id, uint16_t priority, float x, float y, float z, float yaw, uint16_t autocontinue)
{
	msg->msgid = MAVLINK_MSG_ID_WAYPOINT;
	uint16_t i = 0;

	i += put_array_by_index(name, 20, i, msg->payload); //Name of the Waypoint
	i += put_uint16_t_by_index(id, i, msg->payload); //ID
	i += put_uint16_t_by_index(priority, i, msg->payload); //Priority
	i += put_float_by_index(x, i, msg->payload); //x position
	i += put_float_by_index(y, i, msg->payload); //y position
	i += put_float_by_index(z, i, msg->payload); //z position
	i += put_float_by_index(yaw, i, msg->payload); //yaw orientation in radians, 0 = NORTH
	i += put_uint16_t_by_index(autocontinue, i, msg->payload); //autocontinue to next wp

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_waypoint_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_t* waypoint)
{
	return mavlink_msg_waypoint_pack(system_id, component_id, msg, waypoint->name, waypoint->id, waypoint->priority, waypoint->x, waypoint->y, waypoint->z, waypoint->yaw, waypoint->autocontinue);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_send(mavlink_channel_t chan, const int8_t* name, uint16_t id, uint16_t priority, float x, float y, float z, float yaw, uint16_t autocontinue)
{
	mavlink_message_t msg;
	mavlink_msg_waypoint_pack(mavlink_system.sysid, mavlink_system.compid, &msg, name, id, priority, x, y, z, yaw, autocontinue);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE WAYPOINT UNPACKING

/**
 * @brief Get field name from waypoint message
 *
 * @return Name of the Waypoint
 */
static inline uint16_t mavlink_msg_waypoint_get_name(const mavlink_message_t* msg, int8_t* r_data)
{

	memcpy(r_data, msg->payload, 20);
	return 20;
}

/**
 * @brief Get field id from waypoint message
 *
 * @return ID
 */
static inline uint16_t mavlink_msg_waypoint_get_id(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+20)[0];
	r.b[0] = (msg->payload+20)[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field priority from waypoint message
 *
 * @return Priority
 */
static inline uint16_t mavlink_msg_waypoint_get_priority(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+20+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+20+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field x from waypoint message
 *
 * @return x position
 */
static inline float mavlink_msg_waypoint_get_x(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[2] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t))[1];
	r.b[1] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t))[2];
	r.b[0] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field y from waypoint message
 *
 * @return y position
 */
static inline float mavlink_msg_waypoint_get_y(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field z from waypoint message
 *
 * @return z position
 */
static inline float mavlink_msg_waypoint_get_z(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field yaw from waypoint message
 *
 * @return yaw orientation in radians, 0 = NORTH
 */
static inline float mavlink_msg_waypoint_get_yaw(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field autocontinue from waypoint message
 *
 * @return autocontinue to next wp
 */
static inline uint16_t mavlink_msg_waypoint_get_autocontinue(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[0] = (msg->payload+20+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	return (uint16_t)r.s;
}

static inline void mavlink_msg_waypoint_decode(const mavlink_message_t* msg, mavlink_waypoint_t* waypoint)
{
	mavlink_msg_waypoint_get_name(msg, waypoint->name);
	waypoint->id = mavlink_msg_waypoint_get_id(msg);
	waypoint->priority = mavlink_msg_waypoint_get_priority(msg);
	waypoint->x = mavlink_msg_waypoint_get_x(msg);
	waypoint->y = mavlink_msg_waypoint_get_y(msg);
	waypoint->z = mavlink_msg_waypoint_get_z(msg);
	waypoint->yaw = mavlink_msg_waypoint_get_yaw(msg);
	waypoint->autocontinue = mavlink_msg_waypoint_get_autocontinue(msg);
}
