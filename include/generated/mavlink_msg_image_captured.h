// MESSAGE IMAGE_CAPTURED PACKING

#define MAVLINK_MSG_ID_IMAGE_CAPTURED 71

typedef struct __mavlink_image_captured_t 
{
	uint16_t exposure; ///< Exposure time, in microseconds
	float gain; ///< Camera gain
	uint32_t timestamp; ///< Timestamp

} mavlink_image_captured_t;



/**
 * @brief Send a image_captured message
 *
 * @param exposure Exposure time, in microseconds
 * @param gain Camera gain
 * @param timestamp Timestamp
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_image_captured_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint16_t exposure, float gain, uint32_t timestamp)
{
	msg->msgid = MAVLINK_MSG_ID_IMAGE_CAPTURED;
	uint16_t i = 0;

	i += put_uint16_t_by_index(exposure, i, msg->payload); //Exposure time, in microseconds
	i += put_float_by_index(gain, i, msg->payload); //Camera gain
	i += put_uint32_t_by_index(timestamp, i, msg->payload); //Timestamp

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_image_captured_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_image_captured_t* image_captured)
{
	return mavlink_msg_image_captured_pack(system_id, component_id, msg, image_captured->exposure, image_captured->gain, image_captured->timestamp);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_image_captured_send(mavlink_channel_t chan, uint16_t exposure, float gain, uint32_t timestamp)
{
	mavlink_message_t msg;
	mavlink_msg_image_captured_pack(mavlink_system.sysid, mavlink_system.compid, &msg, exposure, gain, timestamp);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE IMAGE_CAPTURED UNPACKING

/**
 * @brief Get field exposure from image_captured message
 *
 * @return Exposure time, in microseconds
 */
static inline uint16_t mavlink_msg_image_captured_get_exposure(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload)[0];
	r.b[0] = (msg->payload)[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field gain from image_captured message
 *
 * @return Camera gain
 */
static inline float mavlink_msg_image_captured_get_gain(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint16_t))[0];
	r.b[2] = (msg->payload+sizeof(uint16_t))[1];
	r.b[1] = (msg->payload+sizeof(uint16_t))[2];
	r.b[0] = (msg->payload+sizeof(uint16_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field timestamp from image_captured message
 *
 * @return Timestamp
 */
static inline uint32_t mavlink_msg_image_captured_get_timestamp(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint16_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint16_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(float))[3];
	return (uint32_t)r.i;
}

static inline void mavlink_msg_image_captured_decode(const mavlink_message_t* msg, mavlink_image_captured_t* image_captured)
{
	image_captured->exposure = mavlink_msg_image_captured_get_exposure(msg);
	image_captured->gain = mavlink_msg_image_captured_get_gain(msg);
	image_captured->timestamp = mavlink_msg_image_captured_get_timestamp(msg);
}
