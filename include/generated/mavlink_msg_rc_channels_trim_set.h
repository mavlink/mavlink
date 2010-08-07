// MESSAGE RC_CHANNELS_TRIM_SET PACKING

#define MAVLINK_MSG_ID_RC_CHANNELS_TRIM_SET 36

typedef struct __mavlink_rc_channels_trim_set_t 
{
	uint16_t chan1_min; ///< RC channel 1 min value, in microseconds
	uint16_t chan1_zero; ///< RC channel 1 zero value, in microseconds
	uint16_t chan1_max; ///< RC channel 1 max value, in microseconds
	uint16_t chan2_min; ///< RC channel 2 min value, in microseconds
	uint16_t chan2_zero; ///< RC channel 2 zero value, in microseconds
	uint16_t chan2_max; ///< RC channel 2 max value, in microseconds
	uint16_t chan3_min; ///< RC channel 3 min value, in microseconds
	uint16_t chan3_zero; ///< RC channel 3 zero value, in microseconds
	uint16_t chan3_max; ///< RC channel 3 max value, in microseconds
	uint16_t chan4_min; ///< RC channel 4 min value, in microseconds
	uint16_t chan4_zero; ///< RC channel 4 zero value, in microseconds
	uint16_t chan4_max; ///< RC channel 4 max value, in microseconds
	uint16_t chan5_min; ///< RC channel 5 min value, in microseconds
	uint16_t chan5_zero; ///< RC channel 5 zero value, in microseconds
	uint16_t chan5_max; ///< RC channel 5 max value, in microseconds

} mavlink_rc_channels_trim_set_t;



/**
 * @brief Send a rc_channels_trim_set message
 *
 * @param chan1_min RC channel 1 min value, in microseconds
 * @param chan1_zero RC channel 1 zero value, in microseconds
 * @param chan1_max RC channel 1 max value, in microseconds
 * @param chan2_min RC channel 2 min value, in microseconds
 * @param chan2_zero RC channel 2 zero value, in microseconds
 * @param chan2_max RC channel 2 max value, in microseconds
 * @param chan3_min RC channel 3 min value, in microseconds
 * @param chan3_zero RC channel 3 zero value, in microseconds
 * @param chan3_max RC channel 3 max value, in microseconds
 * @param chan4_min RC channel 4 min value, in microseconds
 * @param chan4_zero RC channel 4 zero value, in microseconds
 * @param chan4_max RC channel 4 max value, in microseconds
 * @param chan5_min RC channel 5 min value, in microseconds
 * @param chan5_zero RC channel 5 zero value, in microseconds
 * @param chan5_max RC channel 5 max value, in microseconds
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint16_t chan1_min, uint16_t chan1_zero, uint16_t chan1_max, uint16_t chan2_min, uint16_t chan2_zero, uint16_t chan2_max, uint16_t chan3_min, uint16_t chan3_zero, uint16_t chan3_max, uint16_t chan4_min, uint16_t chan4_zero, uint16_t chan4_max, uint16_t chan5_min, uint16_t chan5_zero, uint16_t chan5_max)
{
	uint16_t i = 0;
	msg->msgid = MAVLINK_MSG_ID_RC_CHANNELS_TRIM_SET;

	i += put_uint16_t_by_index(chan1_min, i, msg->payload); //RC channel 1 min value, in microseconds
	i += put_uint16_t_by_index(chan1_zero, i, msg->payload); //RC channel 1 zero value, in microseconds
	i += put_uint16_t_by_index(chan1_max, i, msg->payload); //RC channel 1 max value, in microseconds
	i += put_uint16_t_by_index(chan2_min, i, msg->payload); //RC channel 2 min value, in microseconds
	i += put_uint16_t_by_index(chan2_zero, i, msg->payload); //RC channel 2 zero value, in microseconds
	i += put_uint16_t_by_index(chan2_max, i, msg->payload); //RC channel 2 max value, in microseconds
	i += put_uint16_t_by_index(chan3_min, i, msg->payload); //RC channel 3 min value, in microseconds
	i += put_uint16_t_by_index(chan3_zero, i, msg->payload); //RC channel 3 zero value, in microseconds
	i += put_uint16_t_by_index(chan3_max, i, msg->payload); //RC channel 3 max value, in microseconds
	i += put_uint16_t_by_index(chan4_min, i, msg->payload); //RC channel 4 min value, in microseconds
	i += put_uint16_t_by_index(chan4_zero, i, msg->payload); //RC channel 4 zero value, in microseconds
	i += put_uint16_t_by_index(chan4_max, i, msg->payload); //RC channel 4 max value, in microseconds
	i += put_uint16_t_by_index(chan5_min, i, msg->payload); //RC channel 5 min value, in microseconds
	i += put_uint16_t_by_index(chan5_zero, i, msg->payload); //RC channel 5 zero value, in microseconds
	i += put_uint16_t_by_index(chan5_max, i, msg->payload); //RC channel 5 max value, in microseconds

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_rc_channels_trim_set_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rc_channels_trim_set_t* rc_channels_trim_set)
{
	return mavlink_msg_rc_channels_trim_set_pack(system_id, component_id, msg, rc_channels_trim_set->chan1_min, rc_channels_trim_set->chan1_zero, rc_channels_trim_set->chan1_max, rc_channels_trim_set->chan2_min, rc_channels_trim_set->chan2_zero, rc_channels_trim_set->chan2_max, rc_channels_trim_set->chan3_min, rc_channels_trim_set->chan3_zero, rc_channels_trim_set->chan3_max, rc_channels_trim_set->chan4_min, rc_channels_trim_set->chan4_zero, rc_channels_trim_set->chan4_max, rc_channels_trim_set->chan5_min, rc_channels_trim_set->chan5_zero, rc_channels_trim_set->chan5_max);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rc_channels_trim_set_send(mavlink_channel_t chan, uint16_t chan1_min, uint16_t chan1_zero, uint16_t chan1_max, uint16_t chan2_min, uint16_t chan2_zero, uint16_t chan2_max, uint16_t chan3_min, uint16_t chan3_zero, uint16_t chan3_max, uint16_t chan4_min, uint16_t chan4_zero, uint16_t chan4_max, uint16_t chan5_min, uint16_t chan5_zero, uint16_t chan5_max)
{
	mavlink_message_t msg;
	mavlink_msg_rc_channels_trim_set_pack(mavlink_system.sysid, mavlink_system.compid, &msg, chan1_min, chan1_zero, chan1_max, chan2_min, chan2_zero, chan2_max, chan3_min, chan3_zero, chan3_max, chan4_min, chan4_zero, chan4_max, chan5_min, chan5_zero, chan5_max);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE RC_CHANNELS_TRIM_SET UNPACKING

/**
 * @brief Get field chan1_min from rc_channels_trim_set message
 *
 * @return RC channel 1 min value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan1_min(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload)[0];
	r.b[0] = (msg->payload)[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan1_zero from rc_channels_trim_set message
 *
 * @return RC channel 1 zero value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan1_zero(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan1_max from rc_channels_trim_set message
 *
 * @return RC channel 1 max value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan1_max(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan2_min from rc_channels_trim_set message
 *
 * @return RC channel 2 min value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan2_min(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan2_zero from rc_channels_trim_set message
 *
 * @return RC channel 2 zero value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan2_zero(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan2_max from rc_channels_trim_set message
 *
 * @return RC channel 2 max value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan2_max(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan3_min from rc_channels_trim_set message
 *
 * @return RC channel 3 min value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan3_min(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan3_zero from rc_channels_trim_set message
 *
 * @return RC channel 3 zero value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan3_zero(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan3_max from rc_channels_trim_set message
 *
 * @return RC channel 3 max value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan3_max(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan4_min from rc_channels_trim_set message
 *
 * @return RC channel 4 min value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan4_min(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan4_zero from rc_channels_trim_set message
 *
 * @return RC channel 4 zero value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan4_zero(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan4_max from rc_channels_trim_set message
 *
 * @return RC channel 4 max value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan4_max(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan5_min from rc_channels_trim_set message
 *
 * @return RC channel 5 min value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan5_min(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan5_zero from rc_channels_trim_set message
 *
 * @return RC channel 5 zero value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan5_zero(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field chan5_max from rc_channels_trim_set message
 *
 * @return RC channel 5 max value, in microseconds
 */
static inline uint16_t mavlink_msg_rc_channels_trim_set_get_chan5_max(const mavlink_message_t* msg)
{
	generic_16bit r;
	r.b[1] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[0] = (msg->payload+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

static inline void mavlink_msg_rc_channels_trim_set_decode(const mavlink_message_t* msg, mavlink_rc_channels_trim_set_t* rc_channels_trim_set)
{
	rc_channels_trim_set->chan1_min = mavlink_msg_rc_channels_trim_set_get_chan1_min(msg);
	rc_channels_trim_set->chan1_zero = mavlink_msg_rc_channels_trim_set_get_chan1_zero(msg);
	rc_channels_trim_set->chan1_max = mavlink_msg_rc_channels_trim_set_get_chan1_max(msg);
	rc_channels_trim_set->chan2_min = mavlink_msg_rc_channels_trim_set_get_chan2_min(msg);
	rc_channels_trim_set->chan2_zero = mavlink_msg_rc_channels_trim_set_get_chan2_zero(msg);
	rc_channels_trim_set->chan2_max = mavlink_msg_rc_channels_trim_set_get_chan2_max(msg);
	rc_channels_trim_set->chan3_min = mavlink_msg_rc_channels_trim_set_get_chan3_min(msg);
	rc_channels_trim_set->chan3_zero = mavlink_msg_rc_channels_trim_set_get_chan3_zero(msg);
	rc_channels_trim_set->chan3_max = mavlink_msg_rc_channels_trim_set_get_chan3_max(msg);
	rc_channels_trim_set->chan4_min = mavlink_msg_rc_channels_trim_set_get_chan4_min(msg);
	rc_channels_trim_set->chan4_zero = mavlink_msg_rc_channels_trim_set_get_chan4_zero(msg);
	rc_channels_trim_set->chan4_max = mavlink_msg_rc_channels_trim_set_get_chan4_max(msg);
	rc_channels_trim_set->chan5_min = mavlink_msg_rc_channels_trim_set_get_chan5_min(msg);
	rc_channels_trim_set->chan5_zero = mavlink_msg_rc_channels_trim_set_get_chan5_zero(msg);
	rc_channels_trim_set->chan5_max = mavlink_msg_rc_channels_trim_set_get_chan5_max(msg);
}
