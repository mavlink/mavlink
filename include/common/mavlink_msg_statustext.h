// MESSAGE STATUSTEXT PACKING

#define MAVLINK_MSG_ID_STATUSTEXT 254

typedef struct __mavlink_statustext_t
{
 uint8_t severity; ///< Severity of status, 0 = info message, 255 = critical fault
 char text[50]; ///< Status text message, without null termination character
} mavlink_statustext_t;

/**
 * @brief Pack a statustext message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param severity Severity of status, 0 = info message, 255 = critical fault
 * @param text Status text message, without null termination character
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_statustext_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t severity, const char text[50])
{
	msg->msgid = MAVLINK_MSG_ID_STATUSTEXT;

	put_uint8_t_by_index(severity, 0,  msg->payload); // Severity of status, 0 = info message, 255 = critical fault
	put_char_array_by_index(text, 1, 50,  msg->payload); // Status text message, without null termination character

	return mavlink_finalize_message(msg, system_id, component_id, 51, 55);
}

/**
 * @brief Pack a statustext message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param severity Severity of status, 0 = info message, 255 = critical fault
 * @param text Status text message, without null termination character
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_statustext_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t severity,const char text[50])
{
	msg->msgid = MAVLINK_MSG_ID_STATUSTEXT;

	put_uint8_t_by_index(severity, 0,  msg->payload); // Severity of status, 0 = info message, 255 = critical fault
	put_char_array_by_index(text, 1, 50,  msg->payload); // Status text message, without null termination character

	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 51, 55);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

/**
 * @brief Pack a statustext message on a channel and send
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param severity Severity of status, 0 = info message, 255 = critical fault
 * @param text Status text message, without null termination character
 */
static inline void mavlink_msg_statustext_pack_chan_send(mavlink_channel_t chan,
							   mavlink_message_t* msg,
						           uint8_t severity,const char text[50])
{
	msg->msgid = MAVLINK_MSG_ID_STATUSTEXT;

	put_uint8_t_by_index(severity, 0,  msg->payload); // Severity of status, 0 = info message, 255 = critical fault
	put_char_array_by_index(text, 1, 50,  msg->payload); // Status text message, without null termination character

	mavlink_finalize_message_chan_send(msg, chan, 51, 55);
}
#endif // MAVLINK_USE_CONVENIENCE_FUNCTIONS


/**
 * @brief Encode a statustext struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param statustext C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_statustext_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_statustext_t* statustext)
{
	return mavlink_msg_statustext_pack(system_id, component_id, msg, statustext->severity, statustext->text);
}

/**
 * @brief Send a statustext message
 * @param chan MAVLink channel to send the message
 *
 * @param severity Severity of status, 0 = info message, 255 = critical fault
 * @param text Status text message, without null termination character
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_statustext_send(mavlink_channel_t chan, uint8_t severity, const char text[50])
{
	MAVLINK_ALIGNED_MESSAGE(msg, 51);
	mavlink_msg_statustext_pack_chan_send(chan, msg, severity, text);
}

#endif

// MESSAGE STATUSTEXT UNPACKING


/**
 * @brief Get field severity from statustext message
 *
 * @return Severity of status, 0 = info message, 255 = critical fault
 */
static inline uint8_t mavlink_msg_statustext_get_severity(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field text from statustext message
 *
 * @return Status text message, without null termination character
 */
static inline uint16_t mavlink_msg_statustext_get_text(const mavlink_message_t* msg, char *text)
{
	return MAVLINK_MSG_RETURN_char_array(msg, text, 50,  1);
}

/**
 * @brief Decode a statustext message into a struct
 *
 * @param msg The message to decode
 * @param statustext C-struct to decode the message contents into
 */
static inline void mavlink_msg_statustext_decode(const mavlink_message_t* msg, mavlink_statustext_t* statustext)
{
#if MAVLINK_NEED_BYTE_SWAP
	statustext->severity = mavlink_msg_statustext_get_severity(msg);
	mavlink_msg_statustext_get_text(msg, statustext->text);
#else
	memcpy(statustext, msg->payload, 51);
#endif
}
