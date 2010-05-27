// MESSAGE WAYPOINT_SET_SAFETY_AREA PACKING

#define MAVLINK_MSG_ID_WAYPOINT_SET_SAFETY_AREA 102

typedef struct __mavlink_waypoint_set_safety_area_t 
{
	uint8_t target_system; ///< System ID
	uint8_t target_component; ///< Component ID
	uint8_t type; ///< 0: global, 1: local
	float x1; ///< x position 1
	float y1; ///< y position 1
	float z1; ///< z position 1
	float x2; ///< x position 2
	float y2; ///< y position 2
	float z2; ///< z position 2

} mavlink_waypoint_set_safety_area_t;



/**
 * @brief Send a waypoint_set_safety_area message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param type 0: global, 1: local
 * @param x1 x position 1
 * @param y1 y position 1
 * @param z1 z position 1
 * @param x2 x position 2
 * @param y2 y position 2
 * @param z2 z position 2
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_set_safety_area_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target_system, uint8_t target_component, uint8_t type, float x1, float y1, float z1, float x2, float y2, float z2)
{
	msg->msgid = MAVLINK_MSG_ID_WAYPOINT_SET_SAFETY_AREA;
	uint16_t i = 0;

	i += put_uint8_t_by_index(target_system, i, msg->payload); //System ID
	i += put_uint8_t_by_index(target_component, i, msg->payload); //Component ID
	i += put_uint8_t_by_index(type, i, msg->payload); //0: global, 1: local
	i += put_float_by_index(x1, i, msg->payload); //x position 1
	i += put_float_by_index(y1, i, msg->payload); //y position 1
	i += put_float_by_index(z1, i, msg->payload); //z position 1
	i += put_float_by_index(x2, i, msg->payload); //x position 2
	i += put_float_by_index(y2, i, msg->payload); //y position 2
	i += put_float_by_index(z2, i, msg->payload); //z position 2

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_waypoint_set_safety_area_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_set_safety_area_t* waypoint_set_safety_area)
{
	return mavlink_msg_waypoint_set_safety_area_pack(system_id, component_id, msg, waypoint_set_safety_area->target_system, waypoint_set_safety_area->target_component, waypoint_set_safety_area->type, waypoint_set_safety_area->x1, waypoint_set_safety_area->y1, waypoint_set_safety_area->z1, waypoint_set_safety_area->x2, waypoint_set_safety_area->y2, waypoint_set_safety_area->z2);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_set_safety_area_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint8_t type, float x1, float y1, float z1, float x2, float y2, float z2)
{
	mavlink_message_t msg;
	mavlink_msg_waypoint_set_safety_area_pack(mavlink_system.sysid, mavlink_system.compid, &msg, target_system, target_component, type, x1, y1, z1, x2, y2, z2);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE WAYPOINT_SET_SAFETY_AREA UNPACKING

/**
 * @brief Get field target_system from waypoint_set_safety_area message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_waypoint_set_safety_area_get_target_system(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field target_component from waypoint_set_safety_area message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_waypoint_set_safety_area_get_target_component(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t))[0];
}

/**
 * @brief Get field type from waypoint_set_safety_area message
 *
 * @return 0: global, 1: local
 */
static inline uint8_t mavlink_msg_waypoint_set_safety_area_get_type(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t)+sizeof(uint8_t))[0];
}

/**
 * @brief Get field x1 from waypoint_set_safety_area message
 *
 * @return x position 1
 */
static inline float mavlink_msg_waypoint_set_safety_area_get_x1(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field y1 from waypoint_set_safety_area message
 *
 * @return y position 1
 */
static inline float mavlink_msg_waypoint_set_safety_area_get_y1(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field z1 from waypoint_set_safety_area message
 *
 * @return z position 1
 */
static inline float mavlink_msg_waypoint_set_safety_area_get_z1(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field x2 from waypoint_set_safety_area message
 *
 * @return x position 2
 */
static inline float mavlink_msg_waypoint_set_safety_area_get_x2(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field y2 from waypoint_set_safety_area message
 *
 * @return y position 2
 */
static inline float mavlink_msg_waypoint_set_safety_area_get_y2(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field z2 from waypoint_set_safety_area message
 *
 * @return z position 2
 */
static inline float mavlink_msg_waypoint_set_safety_area_get_z2(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

static inline void mavlink_msg_waypoint_set_safety_area_decode(const mavlink_message_t* msg, mavlink_waypoint_set_safety_area_t* waypoint_set_safety_area)
{
	waypoint_set_safety_area->target_system = mavlink_msg_waypoint_set_safety_area_get_target_system(msg);
	waypoint_set_safety_area->target_component = mavlink_msg_waypoint_set_safety_area_get_target_component(msg);
	waypoint_set_safety_area->type = mavlink_msg_waypoint_set_safety_area_get_type(msg);
	waypoint_set_safety_area->x1 = mavlink_msg_waypoint_set_safety_area_get_x1(msg);
	waypoint_set_safety_area->y1 = mavlink_msg_waypoint_set_safety_area_get_y1(msg);
	waypoint_set_safety_area->z1 = mavlink_msg_waypoint_set_safety_area_get_z1(msg);
	waypoint_set_safety_area->x2 = mavlink_msg_waypoint_set_safety_area_get_x2(msg);
	waypoint_set_safety_area->y2 = mavlink_msg_waypoint_set_safety_area_get_y2(msg);
	waypoint_set_safety_area->z2 = mavlink_msg_waypoint_set_safety_area_get_z2(msg);
}
