// MESSAGE PATTERN_DETECTED PACKING

#define MAVLINK_MSG_ID_PATTERN_DETECTED 200

typedef struct __mavlink_pattern_detected_t 
{
	float confidence; ///< Confidence of detection
	int8_t file[100]; ///< Pattern file name
	uint8_t detected; ///< Accepted as true detection, 0 no, 1 yes

} mavlink_pattern_detected_t;

/**
 * @brief Send a pattern_detected message
 *
 * @param confidence Confidence of detection
 * @param file Pattern file name
 * @param detected Accepted as true detection, 0 no, 1 yes
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_pattern_detected_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, float confidence, const int8_t* file, uint8_t detected)
{
	msg->msgid = MAVLINK_MSG_ID_PATTERN_DETECTED;
	uint16_t i = 0;

	i += put_float_by_index(confidence, i, msg->payload); //Confidence of detection
	i += put_array_by_index(file, 100, i, msg->payload); //Pattern file name
	i += put_uint8_t_by_index(detected, i, msg->payload); //Accepted as true detection, 0 no, 1 yes

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_pattern_detected_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_pattern_detected_t* pattern_detected)
{
	return mavlink_msg_pattern_detected_pack(system_id, component_id, msg, pattern_detected->confidence, pattern_detected->file, pattern_detected->detected);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_pattern_detected_send(mavlink_channel_t chan, float confidence, const int8_t* file, uint8_t detected)
{
	mavlink_message_t msg;
	mavlink_msg_pattern_detected_pack(mavlink_system.sysid, mavlink_system.compid, &msg, confidence, file, detected);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE PATTERN_DETECTED UNPACKING

/**
 * @brief Get field confidence from pattern_detected message
 *
 * @return Confidence of detection
 */
static inline float mavlink_msg_pattern_detected_get_confidence(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload)[0];
	r.b[2] = (msg->payload)[1];
	r.b[1] = (msg->payload)[2];
	r.b[0] = (msg->payload)[3];
	return (float)r.f;
}

/**
 * @brief Get field file from pattern_detected message
 *
 * @return Pattern file name
 */
static inline uint16_t mavlink_msg_pattern_detected_get_file(const mavlink_message_t* msg, int8_t* r_data)
{

	memcpy(r_data, msg->payload+sizeof(float), 100);
	return 100;
}

/**
 * @brief Get field detected from pattern_detected message
 *
 * @return Accepted as true detection, 0 no, 1 yes
 */
static inline uint8_t mavlink_msg_pattern_detected_get_detected(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(float)+100)[0];
}

static inline void mavlink_msg_pattern_detected_decode(const mavlink_message_t* msg, mavlink_pattern_detected_t* pattern_detected)
{
	pattern_detected->confidence = mavlink_msg_pattern_detected_get_confidence(msg);
	mavlink_msg_pattern_detected_get_file(msg, pattern_detected->file);
	pattern_detected->detected = mavlink_msg_pattern_detected_get_detected(msg);
}
