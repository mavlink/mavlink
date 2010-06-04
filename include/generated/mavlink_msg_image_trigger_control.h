// MESSAGE IMAGE_TRIGGER_CONTROL PACKING

#define MAVLINK_MSG_ID_IMAGE_TRIGGER_CONTROL 72

typedef struct __mavlink_image_trigger_control_t 
{
	uint8_t enable; ///< 0 to disable, 1 to enable
	float roll; ///< Roll angle in rad
	float pitch; ///< Pitch angle in rad

} mavlink_image_trigger_control_t;



/**
 * @brief Send a image_trigger_control message
 *
 * @param enable 0 to disable, 1 to enable
 * @param roll Roll angle in rad
 * @param pitch Pitch angle in rad
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_image_trigger_control_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t enable, float roll, float pitch)
{
	msg->msgid = MAVLINK_MSG_ID_IMAGE_TRIGGER_CONTROL;
	uint16_t i = 0;

	i += put_uint8_t_by_index(enable, i, msg->payload); //0 to disable, 1 to enable
	i += put_float_by_index(roll, i, msg->payload); //Roll angle in rad
	i += put_float_by_index(pitch, i, msg->payload); //Pitch angle in rad

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_image_trigger_control_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_image_trigger_control_t* image_trigger_control)
{
	return mavlink_msg_image_trigger_control_pack(system_id, component_id, msg, image_trigger_control->enable, image_trigger_control->roll, image_trigger_control->pitch);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_image_trigger_control_send(mavlink_channel_t chan, uint8_t enable, float roll, float pitch)
{
	mavlink_message_t msg;
	mavlink_msg_image_trigger_control_pack(mavlink_system.sysid, mavlink_system.compid, &msg, enable, roll, pitch);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE IMAGE_TRIGGER_CONTROL UNPACKING

/**
 * @brief Get field enable from image_trigger_control message
 *
 * @return 0 to disable, 1 to enable
 */
static inline uint8_t mavlink_msg_image_trigger_control_get_enable(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field roll from image_trigger_control message
 *
 * @return Roll angle in rad
 */
static inline float mavlink_msg_image_trigger_control_get_roll(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field pitch from image_trigger_control message
 *
 * @return Pitch angle in rad
 */
static inline float mavlink_msg_image_trigger_control_get_pitch(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(float))[3];
	return (float)r.f;
}

static inline void mavlink_msg_image_trigger_control_decode(const mavlink_message_t* msg, mavlink_image_trigger_control_t* image_trigger_control)
{
	image_trigger_control->enable = mavlink_msg_image_trigger_control_get_enable(msg);
	image_trigger_control->roll = mavlink_msg_image_trigger_control_get_roll(msg);
	image_trigger_control->pitch = mavlink_msg_image_trigger_control_get_pitch(msg);
}
