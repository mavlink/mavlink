// MESSAGE RC_CHANNELS_MAPPING PACKING

#define MAVLINK_MSG_ID_RC_CHANNELS_MAPPING 37

typedef struct __mavlink_rc_channels_mapping_t 
{
	uint8_t chan1_function; ///< RC channel 1 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
	uint8_t chan2_function; ///< RC channel 2 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
	uint8_t chan3_function; ///< RC channel 3 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
	uint8_t chan4_function; ///< RC channel 4 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
	uint8_t chan5_function; ///< RC channel 5 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h

} mavlink_rc_channels_mapping_t;



/**
 * @brief Send a rc_channels_mapping message
 *
 * @param chan1_function RC channel 1 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 * @param chan2_function RC channel 2 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 * @param chan3_function RC channel 3 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 * @param chan4_function RC channel 4 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 * @param chan5_function RC channel 5 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rc_channels_mapping_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t chan1_function, uint8_t chan2_function, uint8_t chan3_function, uint8_t chan4_function, uint8_t chan5_function)
{
	uint16_t i = 0;
	msg->msgid = MAVLINK_MSG_ID_RC_CHANNELS_MAPPING;

	i += put_uint8_t_by_index(chan1_function, i, msg->payload); //RC channel 1 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
	i += put_uint8_t_by_index(chan2_function, i, msg->payload); //RC channel 2 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
	i += put_uint8_t_by_index(chan3_function, i, msg->payload); //RC channel 3 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
	i += put_uint8_t_by_index(chan4_function, i, msg->payload); //RC channel 4 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
	i += put_uint8_t_by_index(chan5_function, i, msg->payload); //RC channel 5 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_rc_channels_mapping_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rc_channels_mapping_t* rc_channels_mapping)
{
	return mavlink_msg_rc_channels_mapping_pack(system_id, component_id, msg, rc_channels_mapping->chan1_function, rc_channels_mapping->chan2_function, rc_channels_mapping->chan3_function, rc_channels_mapping->chan4_function, rc_channels_mapping->chan5_function);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rc_channels_mapping_send(mavlink_channel_t chan, uint8_t chan1_function, uint8_t chan2_function, uint8_t chan3_function, uint8_t chan4_function, uint8_t chan5_function)
{
	mavlink_message_t msg;
	mavlink_msg_rc_channels_mapping_pack(mavlink_system.sysid, mavlink_system.compid, &msg, chan1_function, chan2_function, chan3_function, chan4_function, chan5_function);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE RC_CHANNELS_MAPPING UNPACKING

/**
 * @brief Get field chan1_function from rc_channels_mapping message
 *
 * @return RC channel 1 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 */
static inline uint8_t mavlink_msg_rc_channels_mapping_get_chan1_function(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field chan2_function from rc_channels_mapping message
 *
 * @return RC channel 2 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 */
static inline uint8_t mavlink_msg_rc_channels_mapping_get_chan2_function(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t))[0];
}

/**
 * @brief Get field chan3_function from rc_channels_mapping message
 *
 * @return RC channel 3 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 */
static inline uint8_t mavlink_msg_rc_channels_mapping_get_chan3_function(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t)+sizeof(uint8_t))[0];
}

/**
 * @brief Get field chan4_function from rc_channels_mapping message
 *
 * @return RC channel 4 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 */
static inline uint8_t mavlink_msg_rc_channels_mapping_get_chan4_function(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[0];
}

/**
 * @brief Get field chan5_function from rc_channels_mapping message
 *
 * @return RC channel 5 function, as defined in ENUM MAVLINK_RC_CHAN_MAPPING in mavlink/include/mavlink_types.h
 */
static inline uint8_t mavlink_msg_rc_channels_mapping_get_chan5_function(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[0];
}

static inline void mavlink_msg_rc_channels_mapping_decode(const mavlink_message_t* msg, mavlink_rc_channels_mapping_t* rc_channels_mapping)
{
	rc_channels_mapping->chan1_function = mavlink_msg_rc_channels_mapping_get_chan1_function(msg);
	rc_channels_mapping->chan2_function = mavlink_msg_rc_channels_mapping_get_chan2_function(msg);
	rc_channels_mapping->chan3_function = mavlink_msg_rc_channels_mapping_get_chan3_function(msg);
	rc_channels_mapping->chan4_function = mavlink_msg_rc_channels_mapping_get_chan4_function(msg);
	rc_channels_mapping->chan5_function = mavlink_msg_rc_channels_mapping_get_chan5_function(msg);
}
