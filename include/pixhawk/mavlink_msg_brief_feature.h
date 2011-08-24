// MESSAGE BRIEF_FEATURE PACKING

#define MAVLINK_MSG_ID_BRIEF_FEATURE 195

typedef struct __mavlink_brief_feature_t
{
 float x; ///< x position in m
 float y; ///< y position in m
 float z; ///< z position in m
 float response; ///< Harris operator response at this location
 uint16_t size; ///< Size in pixels
 uint16_t orientation; ///< Orientation
 uint8_t orientation_assignment; ///< Orientation assignment 0: false, 1:true
 uint8_t descriptor[32]; ///< Descriptor
} mavlink_brief_feature_t;

/**
 * @brief Pack a brief_feature message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param x x position in m
 * @param y y position in m
 * @param z z position in m
 * @param orientation_assignment Orientation assignment 0: false, 1:true
 * @param size Size in pixels
 * @param orientation Orientation
 * @param descriptor Descriptor
 * @param response Harris operator response at this location
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_brief_feature_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       float x, float y, float z, uint8_t orientation_assignment, uint16_t size, uint16_t orientation, const uint8_t descriptor[32], float response)
{
	msg->msgid = MAVLINK_MSG_ID_BRIEF_FEATURE;

	put_float_by_index(x, 0,  msg->payload); // x position in m
	put_float_by_index(y, 4,  msg->payload); // y position in m
	put_float_by_index(z, 8,  msg->payload); // z position in m
	put_float_by_index(response, 12,  msg->payload); // Harris operator response at this location
	put_uint16_t_by_index(size, 16,  msg->payload); // Size in pixels
	put_uint16_t_by_index(orientation, 18,  msg->payload); // Orientation
	put_uint8_t_by_index(orientation_assignment, 20,  msg->payload); // Orientation assignment 0: false, 1:true
	put_uint8_t_array_by_index(descriptor, 21, 32,  msg->payload); // Descriptor

	return mavlink_finalize_message(msg, system_id, component_id, 53, 95);
}

/**
 * @brief Pack a brief_feature message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param x x position in m
 * @param y y position in m
 * @param z z position in m
 * @param orientation_assignment Orientation assignment 0: false, 1:true
 * @param size Size in pixels
 * @param orientation Orientation
 * @param descriptor Descriptor
 * @param response Harris operator response at this location
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_brief_feature_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           float x,float y,float z,uint8_t orientation_assignment,uint16_t size,uint16_t orientation,const uint8_t descriptor[32],float response)
{
	msg->msgid = MAVLINK_MSG_ID_BRIEF_FEATURE;

	put_float_by_index(x, 0,  msg->payload); // x position in m
	put_float_by_index(y, 4,  msg->payload); // y position in m
	put_float_by_index(z, 8,  msg->payload); // z position in m
	put_float_by_index(response, 12,  msg->payload); // Harris operator response at this location
	put_uint16_t_by_index(size, 16,  msg->payload); // Size in pixels
	put_uint16_t_by_index(orientation, 18,  msg->payload); // Orientation
	put_uint8_t_by_index(orientation_assignment, 20,  msg->payload); // Orientation assignment 0: false, 1:true
	put_uint8_t_array_by_index(descriptor, 21, 32,  msg->payload); // Descriptor

	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 53, 95);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

/**
 * @brief Pack a brief_feature message on a channel and send
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param x x position in m
 * @param y y position in m
 * @param z z position in m
 * @param orientation_assignment Orientation assignment 0: false, 1:true
 * @param size Size in pixels
 * @param orientation Orientation
 * @param descriptor Descriptor
 * @param response Harris operator response at this location
 */
static inline void mavlink_msg_brief_feature_pack_chan_send(mavlink_channel_t chan,
							   mavlink_message_t* msg,
						           float x,float y,float z,uint8_t orientation_assignment,uint16_t size,uint16_t orientation,const uint8_t descriptor[32],float response)
{
	msg->msgid = MAVLINK_MSG_ID_BRIEF_FEATURE;

	put_float_by_index(x, 0,  msg->payload); // x position in m
	put_float_by_index(y, 4,  msg->payload); // y position in m
	put_float_by_index(z, 8,  msg->payload); // z position in m
	put_float_by_index(response, 12,  msg->payload); // Harris operator response at this location
	put_uint16_t_by_index(size, 16,  msg->payload); // Size in pixels
	put_uint16_t_by_index(orientation, 18,  msg->payload); // Orientation
	put_uint8_t_by_index(orientation_assignment, 20,  msg->payload); // Orientation assignment 0: false, 1:true
	put_uint8_t_array_by_index(descriptor, 21, 32,  msg->payload); // Descriptor

	mavlink_finalize_message_chan_send(msg, chan, 53, 95);
}
#endif // MAVLINK_USE_CONVENIENCE_FUNCTIONS


/**
 * @brief Encode a brief_feature struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param brief_feature C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_brief_feature_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_brief_feature_t* brief_feature)
{
	return mavlink_msg_brief_feature_pack(system_id, component_id, msg, brief_feature->x, brief_feature->y, brief_feature->z, brief_feature->orientation_assignment, brief_feature->size, brief_feature->orientation, brief_feature->descriptor, brief_feature->response);
}

/**
 * @brief Send a brief_feature message
 * @param chan MAVLink channel to send the message
 *
 * @param x x position in m
 * @param y y position in m
 * @param z z position in m
 * @param orientation_assignment Orientation assignment 0: false, 1:true
 * @param size Size in pixels
 * @param orientation Orientation
 * @param descriptor Descriptor
 * @param response Harris operator response at this location
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_brief_feature_send(mavlink_channel_t chan, float x, float y, float z, uint8_t orientation_assignment, uint16_t size, uint16_t orientation, const uint8_t descriptor[32], float response)
{
	MAVLINK_ALIGNED_MESSAGE(msg, 53);
	mavlink_msg_brief_feature_pack_chan_send(chan, msg, x, y, z, orientation_assignment, size, orientation, descriptor, response);
}

#endif

// MESSAGE BRIEF_FEATURE UNPACKING


/**
 * @brief Get field x from brief_feature message
 *
 * @return x position in m
 */
static inline float mavlink_msg_brief_feature_get_x(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  0);
}

/**
 * @brief Get field y from brief_feature message
 *
 * @return y position in m
 */
static inline float mavlink_msg_brief_feature_get_y(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  4);
}

/**
 * @brief Get field z from brief_feature message
 *
 * @return z position in m
 */
static inline float mavlink_msg_brief_feature_get_z(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  8);
}

/**
 * @brief Get field orientation_assignment from brief_feature message
 *
 * @return Orientation assignment 0: false, 1:true
 */
static inline uint8_t mavlink_msg_brief_feature_get_orientation_assignment(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field size from brief_feature message
 *
 * @return Size in pixels
 */
static inline uint16_t mavlink_msg_brief_feature_get_size(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint16_t(msg,  16);
}

/**
 * @brief Get field orientation from brief_feature message
 *
 * @return Orientation
 */
static inline uint16_t mavlink_msg_brief_feature_get_orientation(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint16_t(msg,  18);
}

/**
 * @brief Get field descriptor from brief_feature message
 *
 * @return Descriptor
 */
static inline uint16_t mavlink_msg_brief_feature_get_descriptor(const mavlink_message_t* msg, uint8_t *descriptor)
{
	return MAVLINK_MSG_RETURN_uint8_t_array(msg, descriptor, 32,  21);
}

/**
 * @brief Get field response from brief_feature message
 *
 * @return Harris operator response at this location
 */
static inline float mavlink_msg_brief_feature_get_response(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  12);
}

/**
 * @brief Decode a brief_feature message into a struct
 *
 * @param msg The message to decode
 * @param brief_feature C-struct to decode the message contents into
 */
static inline void mavlink_msg_brief_feature_decode(const mavlink_message_t* msg, mavlink_brief_feature_t* brief_feature)
{
#if MAVLINK_NEED_BYTE_SWAP
	brief_feature->x = mavlink_msg_brief_feature_get_x(msg);
	brief_feature->y = mavlink_msg_brief_feature_get_y(msg);
	brief_feature->z = mavlink_msg_brief_feature_get_z(msg);
	brief_feature->response = mavlink_msg_brief_feature_get_response(msg);
	brief_feature->size = mavlink_msg_brief_feature_get_size(msg);
	brief_feature->orientation = mavlink_msg_brief_feature_get_orientation(msg);
	brief_feature->orientation_assignment = mavlink_msg_brief_feature_get_orientation_assignment(msg);
	mavlink_msg_brief_feature_get_descriptor(msg, brief_feature->descriptor);
#else
	memcpy(brief_feature, msg->payload, 53);
#endif
}
