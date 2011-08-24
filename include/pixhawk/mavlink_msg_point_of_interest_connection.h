// MESSAGE POINT_OF_INTEREST_CONNECTION PACKING

#define MAVLINK_MSG_ID_POINT_OF_INTEREST_CONNECTION 192

typedef struct __mavlink_point_of_interest_connection_t
{
 float xp1; ///< X1 Position
 float yp1; ///< Y1 Position
 float zp1; ///< Z1 Position
 float xp2; ///< X2 Position
 float yp2; ///< Y2 Position
 float zp2; ///< Z2 Position
 uint16_t timeout; ///< 0: no timeout, >1: timeout in seconds
 uint8_t type; ///< 0: Notice, 1: Warning, 2: Critical, 3: Emergency, 4: Debug
 uint8_t color; ///< 0: blue, 1: yellow, 2: red, 3: orange, 4: green, 5: magenta
 uint8_t coordinate_system; ///< 0: global, 1:local
 char name[26]; ///< POI connection name
} mavlink_point_of_interest_connection_t;

/**
 * @brief Pack a point_of_interest_connection message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param type 0: Notice, 1: Warning, 2: Critical, 3: Emergency, 4: Debug
 * @param color 0: blue, 1: yellow, 2: red, 3: orange, 4: green, 5: magenta
 * @param coordinate_system 0: global, 1:local
 * @param timeout 0: no timeout, >1: timeout in seconds
 * @param xp1 X1 Position
 * @param yp1 Y1 Position
 * @param zp1 Z1 Position
 * @param xp2 X2 Position
 * @param yp2 Y2 Position
 * @param zp2 Z2 Position
 * @param name POI connection name
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_point_of_interest_connection_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t type, uint8_t color, uint8_t coordinate_system, uint16_t timeout, float xp1, float yp1, float zp1, float xp2, float yp2, float zp2, const char name[26])
{
	msg->msgid = MAVLINK_MSG_ID_POINT_OF_INTEREST_CONNECTION;

	put_float_by_index(xp1, 0,  msg->payload); // X1 Position
	put_float_by_index(yp1, 4,  msg->payload); // Y1 Position
	put_float_by_index(zp1, 8,  msg->payload); // Z1 Position
	put_float_by_index(xp2, 12,  msg->payload); // X2 Position
	put_float_by_index(yp2, 16,  msg->payload); // Y2 Position
	put_float_by_index(zp2, 20,  msg->payload); // Z2 Position
	put_uint16_t_by_index(timeout, 24,  msg->payload); // 0: no timeout, >1: timeout in seconds
	put_uint8_t_by_index(type, 26,  msg->payload); // 0: Notice, 1: Warning, 2: Critical, 3: Emergency, 4: Debug
	put_uint8_t_by_index(color, 27,  msg->payload); // 0: blue, 1: yellow, 2: red, 3: orange, 4: green, 5: magenta
	put_uint8_t_by_index(coordinate_system, 28,  msg->payload); // 0: global, 1:local
	put_char_array_by_index(name, 29, 26,  msg->payload); // POI connection name

	return mavlink_finalize_message(msg, system_id, component_id, 55, 103);
}

/**
 * @brief Pack a point_of_interest_connection message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param type 0: Notice, 1: Warning, 2: Critical, 3: Emergency, 4: Debug
 * @param color 0: blue, 1: yellow, 2: red, 3: orange, 4: green, 5: magenta
 * @param coordinate_system 0: global, 1:local
 * @param timeout 0: no timeout, >1: timeout in seconds
 * @param xp1 X1 Position
 * @param yp1 Y1 Position
 * @param zp1 Z1 Position
 * @param xp2 X2 Position
 * @param yp2 Y2 Position
 * @param zp2 Z2 Position
 * @param name POI connection name
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_point_of_interest_connection_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t type,uint8_t color,uint8_t coordinate_system,uint16_t timeout,float xp1,float yp1,float zp1,float xp2,float yp2,float zp2,const char name[26])
{
	msg->msgid = MAVLINK_MSG_ID_POINT_OF_INTEREST_CONNECTION;

	put_float_by_index(xp1, 0,  msg->payload); // X1 Position
	put_float_by_index(yp1, 4,  msg->payload); // Y1 Position
	put_float_by_index(zp1, 8,  msg->payload); // Z1 Position
	put_float_by_index(xp2, 12,  msg->payload); // X2 Position
	put_float_by_index(yp2, 16,  msg->payload); // Y2 Position
	put_float_by_index(zp2, 20,  msg->payload); // Z2 Position
	put_uint16_t_by_index(timeout, 24,  msg->payload); // 0: no timeout, >1: timeout in seconds
	put_uint8_t_by_index(type, 26,  msg->payload); // 0: Notice, 1: Warning, 2: Critical, 3: Emergency, 4: Debug
	put_uint8_t_by_index(color, 27,  msg->payload); // 0: blue, 1: yellow, 2: red, 3: orange, 4: green, 5: magenta
	put_uint8_t_by_index(coordinate_system, 28,  msg->payload); // 0: global, 1:local
	put_char_array_by_index(name, 29, 26,  msg->payload); // POI connection name

	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 55, 103);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

/**
 * @brief Pack a point_of_interest_connection message on a channel and send
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param type 0: Notice, 1: Warning, 2: Critical, 3: Emergency, 4: Debug
 * @param color 0: blue, 1: yellow, 2: red, 3: orange, 4: green, 5: magenta
 * @param coordinate_system 0: global, 1:local
 * @param timeout 0: no timeout, >1: timeout in seconds
 * @param xp1 X1 Position
 * @param yp1 Y1 Position
 * @param zp1 Z1 Position
 * @param xp2 X2 Position
 * @param yp2 Y2 Position
 * @param zp2 Z2 Position
 * @param name POI connection name
 */
static inline void mavlink_msg_point_of_interest_connection_pack_chan_send(mavlink_channel_t chan,
							   mavlink_message_t* msg,
						           uint8_t type,uint8_t color,uint8_t coordinate_system,uint16_t timeout,float xp1,float yp1,float zp1,float xp2,float yp2,float zp2,const char name[26])
{
	msg->msgid = MAVLINK_MSG_ID_POINT_OF_INTEREST_CONNECTION;

	put_float_by_index(xp1, 0,  msg->payload); // X1 Position
	put_float_by_index(yp1, 4,  msg->payload); // Y1 Position
	put_float_by_index(zp1, 8,  msg->payload); // Z1 Position
	put_float_by_index(xp2, 12,  msg->payload); // X2 Position
	put_float_by_index(yp2, 16,  msg->payload); // Y2 Position
	put_float_by_index(zp2, 20,  msg->payload); // Z2 Position
	put_uint16_t_by_index(timeout, 24,  msg->payload); // 0: no timeout, >1: timeout in seconds
	put_uint8_t_by_index(type, 26,  msg->payload); // 0: Notice, 1: Warning, 2: Critical, 3: Emergency, 4: Debug
	put_uint8_t_by_index(color, 27,  msg->payload); // 0: blue, 1: yellow, 2: red, 3: orange, 4: green, 5: magenta
	put_uint8_t_by_index(coordinate_system, 28,  msg->payload); // 0: global, 1:local
	put_char_array_by_index(name, 29, 26,  msg->payload); // POI connection name

	mavlink_finalize_message_chan_send(msg, chan, 55, 103);
}
#endif // MAVLINK_USE_CONVENIENCE_FUNCTIONS


/**
 * @brief Encode a point_of_interest_connection struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param point_of_interest_connection C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_point_of_interest_connection_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_point_of_interest_connection_t* point_of_interest_connection)
{
	return mavlink_msg_point_of_interest_connection_pack(system_id, component_id, msg, point_of_interest_connection->type, point_of_interest_connection->color, point_of_interest_connection->coordinate_system, point_of_interest_connection->timeout, point_of_interest_connection->xp1, point_of_interest_connection->yp1, point_of_interest_connection->zp1, point_of_interest_connection->xp2, point_of_interest_connection->yp2, point_of_interest_connection->zp2, point_of_interest_connection->name);
}

/**
 * @brief Send a point_of_interest_connection message
 * @param chan MAVLink channel to send the message
 *
 * @param type 0: Notice, 1: Warning, 2: Critical, 3: Emergency, 4: Debug
 * @param color 0: blue, 1: yellow, 2: red, 3: orange, 4: green, 5: magenta
 * @param coordinate_system 0: global, 1:local
 * @param timeout 0: no timeout, >1: timeout in seconds
 * @param xp1 X1 Position
 * @param yp1 Y1 Position
 * @param zp1 Z1 Position
 * @param xp2 X2 Position
 * @param yp2 Y2 Position
 * @param zp2 Z2 Position
 * @param name POI connection name
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_point_of_interest_connection_send(mavlink_channel_t chan, uint8_t type, uint8_t color, uint8_t coordinate_system, uint16_t timeout, float xp1, float yp1, float zp1, float xp2, float yp2, float zp2, const char name[26])
{
	MAVLINK_ALIGNED_MESSAGE(msg, 55);
	mavlink_msg_point_of_interest_connection_pack_chan_send(chan, msg, type, color, coordinate_system, timeout, xp1, yp1, zp1, xp2, yp2, zp2, name);
}

#endif

// MESSAGE POINT_OF_INTEREST_CONNECTION UNPACKING


/**
 * @brief Get field type from point_of_interest_connection message
 *
 * @return 0: Notice, 1: Warning, 2: Critical, 3: Emergency, 4: Debug
 */
static inline uint8_t mavlink_msg_point_of_interest_connection_get_type(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint8_t(msg,  26);
}

/**
 * @brief Get field color from point_of_interest_connection message
 *
 * @return 0: blue, 1: yellow, 2: red, 3: orange, 4: green, 5: magenta
 */
static inline uint8_t mavlink_msg_point_of_interest_connection_get_color(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint8_t(msg,  27);
}

/**
 * @brief Get field coordinate_system from point_of_interest_connection message
 *
 * @return 0: global, 1:local
 */
static inline uint8_t mavlink_msg_point_of_interest_connection_get_coordinate_system(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint8_t(msg,  28);
}

/**
 * @brief Get field timeout from point_of_interest_connection message
 *
 * @return 0: no timeout, >1: timeout in seconds
 */
static inline uint16_t mavlink_msg_point_of_interest_connection_get_timeout(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint16_t(msg,  24);
}

/**
 * @brief Get field xp1 from point_of_interest_connection message
 *
 * @return X1 Position
 */
static inline float mavlink_msg_point_of_interest_connection_get_xp1(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  0);
}

/**
 * @brief Get field yp1 from point_of_interest_connection message
 *
 * @return Y1 Position
 */
static inline float mavlink_msg_point_of_interest_connection_get_yp1(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  4);
}

/**
 * @brief Get field zp1 from point_of_interest_connection message
 *
 * @return Z1 Position
 */
static inline float mavlink_msg_point_of_interest_connection_get_zp1(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  8);
}

/**
 * @brief Get field xp2 from point_of_interest_connection message
 *
 * @return X2 Position
 */
static inline float mavlink_msg_point_of_interest_connection_get_xp2(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  12);
}

/**
 * @brief Get field yp2 from point_of_interest_connection message
 *
 * @return Y2 Position
 */
static inline float mavlink_msg_point_of_interest_connection_get_yp2(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  16);
}

/**
 * @brief Get field zp2 from point_of_interest_connection message
 *
 * @return Z2 Position
 */
static inline float mavlink_msg_point_of_interest_connection_get_zp2(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  20);
}

/**
 * @brief Get field name from point_of_interest_connection message
 *
 * @return POI connection name
 */
static inline uint16_t mavlink_msg_point_of_interest_connection_get_name(const mavlink_message_t* msg, char *name)
{
	return MAVLINK_MSG_RETURN_char_array(msg, name, 26,  29);
}

/**
 * @brief Decode a point_of_interest_connection message into a struct
 *
 * @param msg The message to decode
 * @param point_of_interest_connection C-struct to decode the message contents into
 */
static inline void mavlink_msg_point_of_interest_connection_decode(const mavlink_message_t* msg, mavlink_point_of_interest_connection_t* point_of_interest_connection)
{
#if MAVLINK_NEED_BYTE_SWAP
	point_of_interest_connection->xp1 = mavlink_msg_point_of_interest_connection_get_xp1(msg);
	point_of_interest_connection->yp1 = mavlink_msg_point_of_interest_connection_get_yp1(msg);
	point_of_interest_connection->zp1 = mavlink_msg_point_of_interest_connection_get_zp1(msg);
	point_of_interest_connection->xp2 = mavlink_msg_point_of_interest_connection_get_xp2(msg);
	point_of_interest_connection->yp2 = mavlink_msg_point_of_interest_connection_get_yp2(msg);
	point_of_interest_connection->zp2 = mavlink_msg_point_of_interest_connection_get_zp2(msg);
	point_of_interest_connection->timeout = mavlink_msg_point_of_interest_connection_get_timeout(msg);
	point_of_interest_connection->type = mavlink_msg_point_of_interest_connection_get_type(msg);
	point_of_interest_connection->color = mavlink_msg_point_of_interest_connection_get_color(msg);
	point_of_interest_connection->coordinate_system = mavlink_msg_point_of_interest_connection_get_coordinate_system(msg);
	mavlink_msg_point_of_interest_connection_get_name(msg, point_of_interest_connection->name);
#else
	memcpy(point_of_interest_connection, msg->payload, 55);
#endif
}
