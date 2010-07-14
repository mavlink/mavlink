// MESSAGE RC_CHANNELS_RAW PACKING

#define MAVLINK_MSG_ID_RC_CHANNELS_RAW 35

typedef struct __mavlink_rc_channels_raw_t 
{
	uint16_t chan1; ///< RC channel 1 value, in microseconds
	uint16_t chan2; ///< RC channel 2 value, in microseconds
	uint16_t chan3; ///< RC channel 3 value, in microseconds
	uint16_t chan4; ///< RC channel 4 value, in microseconds
	uint16_t chan5; ///< RC channel 5 value, in microseconds
	uint16_t chan6; ///< RC channel 6 value, in microseconds
	uint16_t chan7; ///< RC channel 7 value, in microseconds
	uint16_t chan8; ///< RC channel 8 value, in microseconds

} mavlink_rc_channels_raw_t;



/**
 * @brief Send a rc_channels_raw message
 *
 * @param chan1 RC channel 1 value, in microseconds
 * @param chan2 RC channel 2 value, in microseconds
 * @param chan3 RC channel 3 value, in microseconds
 * @param chan4 RC channel 4 value, in microseconds
 * @param chan5 RC channel 5 value, in microseconds
 * @param chan6 RC channel 6 value, in microseconds
 * @param chan7 RC channel 7 value, in microseconds
 * @param chan8 RC channel 8 value, in microseconds
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rc_channels_raw_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint16_t chan1, uint16_t chan2, uint16_t chan3, uint16_t chan4, uint16_t chan5, uint16_t chan6, uint16_t chan7, uint16_t chan8)
{
	msg->msgid = MAVLINK_MSG_ID_RC_CHANNELS_RAW;
	uint16_t i = 0;

	i += put_uint16_t_by_index(chan1, i, msg->payload); //RC channel 1 value, in microseconds
	i += put_uint16_t_by_index(chan2, i, msg->payload); //RC channel 2 value, in microseconds
	i += put_uint16_t_by_index(chan3, i, msg->payload); //RC channel 3 value, in microseconds
	i += put_uint16_t_by_index(chan4, i, msg->payload); //RC channel 4 value, in microseconds
	i += put_uint16_t_by_index(chan5, i, msg->payload); //RC channel 5 value, in microseconds
	i += put_uint16_t_by_index(chan6, i, msg->payload); //RC channel 6 value, in microseconds
	i += put_uint16_t_by_index(chan7, i, msg->payload); //RC channel 7 value, in microseconds
	i += put_uint16_t_by_index(chan8, i, msg->payload); //RC channel 8 value, in microseconds

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_rc_channels_raw_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rc_channels_raw_t* rc_channels_raw)
{
	return mavlink_msg_rc_channels_raw_pack(system_id, component_id, msg, rc_channels_raw->chan1, rc_channels_raw->chan2, rc_channels_raw->chan3, rc_channels_raw->chan4, rc_channels_raw->chan5, rc_channels_raw->chan6, rc_channels_raw->chan7, rc_channels_raw->chan8);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rc_channels_raw_send(mavlink_channel_t chan, uint16_t chan1, uint16_t chan2, uint16_t chan3, uint16_t chan4, uint16_t chan5, uint16_t chan6, uint16_t chan7, uint16_t chan8)
{
	mavlink_message_t msg;
	mavlink_msg_rc_channels_raw_pack(mavlink_system.sysid, mavlink_system.compid, &msg, chan1, chan2, chan3, chan4, chan5, chan6, chan7, chan8);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE RC_CHANNELS_RAW UNPACKING

/**
 * @brief Get field chan1 from rc_channels_raw message
 *
 * @return RC channel 1 value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_raw_get_chan1(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload)[0];
	r.b[0] = (msg->payload)[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan2 from rc_channels_raw message
 *
 * @return RC channel 2 value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_raw_get_chan2(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan3 from rc_channels_raw message
 *
 * @return RC channel 3 value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_raw_get_chan3(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan4 from rc_channels_raw message
 *
 * @return RC channel 4 value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_raw_get_chan4(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan5 from rc_channels_raw message
 *
 * @return RC channel 5 value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_raw_get_chan5(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan6 from rc_channels_raw message
 *
 * @return RC channel 6 value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_raw_get_chan6(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan7 from rc_channels_raw message
 *
 * @return RC channel 7 value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_raw_get_chan7(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan8 from rc_channels_raw message
 *
 * @return RC channel 8 value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_raw_get_chan8(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

static inline void mavlink_msg_rc_channels_raw_decode(const mavlink_message_t* msg, mavlink_rc_channels_raw_t* rc_channels_raw)
{
	rc_channels_raw->chan1 = mavlink_msg_rc_channels_raw_get_chan1(msg);
	rc_channels_raw->chan2 = mavlink_msg_rc_channels_raw_get_chan2(msg);
	rc_channels_raw->chan3 = mavlink_msg_rc_channels_raw_get_chan3(msg);
	rc_channels_raw->chan4 = mavlink_msg_rc_channels_raw_get_chan4(msg);
	rc_channels_raw->chan5 = mavlink_msg_rc_channels_raw_get_chan5(msg);
	rc_channels_raw->chan6 = mavlink_msg_rc_channels_raw_get_chan6(msg);
	rc_channels_raw->chan7 = mavlink_msg_rc_channels_raw_get_chan7(msg);
	rc_channels_raw->chan8 = mavlink_msg_rc_channels_raw_get_chan8(msg);
}
