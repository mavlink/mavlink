// MESSAGE WAYPOINT PACKING

#define MAVLINK_MSG_ID_WAYPOINT 92

typedef struct __mavlink_waypoint_t 
{
	int8_t name[20]; ///< Name of the Waypoint
	int8_t id[20]; ///< ID
	int8_t priority[20]; ///< Priority
	int8_t x[20]; ///< x position
	int8_t y[20]; ///< y position
	int8_t z[20]; ///< z position
	int8_t yaw[20]; ///< yaw orientation

} mavlink_waypoint_t;

#define MAVLINK_MSG_WAYPOINT_FIELD_NAME_LEN 20#define MAVLINK_MSG_WAYPOINT_FIELD_ID_LEN 20#define MAVLINK_MSG_WAYPOINT_FIELD_PRIORITY_LEN 20#define MAVLINK_MSG_WAYPOINT_FIELD_X_LEN 20#define MAVLINK_MSG_WAYPOINT_FIELD_Y_LEN 20#define MAVLINK_MSG_WAYPOINT_FIELD_Z_LEN 20#define MAVLINK_MSG_WAYPOINT_FIELD_YAW_LEN 20

/**
 * @brief Send a waypoint message
 *
 * @param name Name of the Waypoint
 * @param id ID
 * @param priority Priority
 * @param x x position
 * @param y y position
 * @param z z position
 * @param yaw yaw orientation
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const int8_t* name, const int8_t* id, const int8_t* priority, const int8_t* x, const int8_t* y, const int8_t* z, const int8_t* yaw)
{
	msg->msgid = MAVLINK_MSG_ID_WAYPOINT;
	uint16_t i = 0;

	i += put_array_by_index(name, 20, i, msg->payload); //Name of the Waypoint
	i += put_array_by_index(id, 20, i, msg->payload); //ID
	i += put_array_by_index(priority, 20, i, msg->payload); //Priority
	i += put_array_by_index(x, 20, i, msg->payload); //x position
	i += put_array_by_index(y, 20, i, msg->payload); //y position
	i += put_array_by_index(z, 20, i, msg->payload); //z position
	i += put_array_by_index(yaw, 20, i, msg->payload); //yaw orientation

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_waypoint_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_t* waypoint)
{
	return mavlink_msg_waypoint_pack(system_id, component_id, msg, waypoint->name, waypoint->id, waypoint->priority, waypoint->x, waypoint->y, waypoint->z, waypoint->yaw);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_send(mavlink_channel_t chan, const int8_t* name, const int8_t* id, const int8_t* priority, const int8_t* x, const int8_t* y, const int8_t* z, const int8_t* yaw)
{
	mavlink_message_t msg;
	mavlink_msg_waypoint_pack(mavlink_system.sysid, mavlink_system.compid, &msg, name, id, priority, x, y, z, yaw);
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
static inline uint16_t mavlink_msg_waypoint_get_id(const mavlink_message_t* msg, int8_t* r_data)
{

	memcpy(r_data, msg->payload+20, 20);
	return 20;
}

/**
 * @brief Get field priority from waypoint message
 *
 * @return Priority
 */
static inline uint16_t mavlink_msg_waypoint_get_priority(const mavlink_message_t* msg, int8_t* r_data)
{

	memcpy(r_data, msg->payload+20+20, 20);
	return 20;
}

/**
 * @brief Get field x from waypoint message
 *
 * @return x position
 */
static inline uint16_t mavlink_msg_waypoint_get_x(const mavlink_message_t* msg, int8_t* r_data)
{

	memcpy(r_data, msg->payload+20+20+20, 20);
	return 20;
}

/**
 * @brief Get field y from waypoint message
 *
 * @return y position
 */
static inline uint16_t mavlink_msg_waypoint_get_y(const mavlink_message_t* msg, int8_t* r_data)
{

	memcpy(r_data, msg->payload+20+20+20+20, 20);
	return 20;
}

/**
 * @brief Get field z from waypoint message
 *
 * @return z position
 */
static inline uint16_t mavlink_msg_waypoint_get_z(const mavlink_message_t* msg, int8_t* r_data)
{

	memcpy(r_data, msg->payload+20+20+20+20+20, 20);
	return 20;
}

/**
 * @brief Get field yaw from waypoint message
 *
 * @return yaw orientation
 */
static inline uint16_t mavlink_msg_waypoint_get_yaw(const mavlink_message_t* msg, int8_t* r_data)
{

	memcpy(r_data, msg->payload+20+20+20+20+20+20, 20);
	return 20;
}

static inline void mavlink_msg_waypoint_decode(const mavlink_message_t* msg, mavlink_waypoint_t* waypoint)
{
	mavlink_msg_waypoint_get_name(msg, waypoint->name);
	mavlink_msg_waypoint_get_id(msg, waypoint->id);
	mavlink_msg_waypoint_get_priority(msg, waypoint->priority);
	mavlink_msg_waypoint_get_x(msg, waypoint->x);
	mavlink_msg_waypoint_get_y(msg, waypoint->y);
	mavlink_msg_waypoint_get_z(msg, waypoint->z);
	mavlink_msg_waypoint_get_yaw(msg, waypoint->yaw);
}
