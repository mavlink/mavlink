// MESSAGE PID_SET PACKING

#define MAVLINK_MSG_ID_PID_SET 25

typedef struct __mavlink_pid_set_t 
{
	uint8_t target_system; ///< System ID
	uint8_t target_component; ///< Component ID
	uint8_t pid_id; ///< PID ID
	float k_p; ///< P
	float k_i; ///< I
	float k_d; ///< D

} mavlink_pid_set_t;



/**
 * @brief Send a pid_set message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param pid_id PID ID
 * @param k_p P
 * @param k_i I
 * @param k_d D
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_pid_set_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target_system, uint8_t target_component, uint8_t pid_id, float k_p, float k_i, float k_d)
{
	uint16_t i = 0;
	msg->msgid = MAVLINK_MSG_ID_PID_SET;

	i += put_uint8_t_by_index(target_system, i, msg->payload); //System ID
	i += put_uint8_t_by_index(target_component, i, msg->payload); //Component ID
	i += put_uint8_t_by_index(pid_id, i, msg->payload); //PID ID
	i += put_float_by_index(k_p, i, msg->payload); //P
	i += put_float_by_index(k_i, i, msg->payload); //I
	i += put_float_by_index(k_d, i, msg->payload); //D

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_pid_set_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_pid_set_t* pid_set)
{
	return mavlink_msg_pid_set_pack(system_id, component_id, msg, pid_set->target_system, pid_set->target_component, pid_set->pid_id, pid_set->k_p, pid_set->k_i, pid_set->k_d);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_pid_set_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint8_t pid_id, float k_p, float k_i, float k_d)
{
	mavlink_message_t msg;
	mavlink_msg_pid_set_pack(mavlink_system.sysid, mavlink_system.compid, &msg, target_system, target_component, pid_id, k_p, k_i, k_d);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE PID_SET UNPACKING

/**
 * @brief Get field target_system from pid_set message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_pid_set_get_target_system(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload)[0];
}

/**
 * @brief Get field target_component from pid_set message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_pid_set_get_target_component(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t))[0];
}

/**
 * @brief Get field pid_id from pid_set message
 *
 * @return PID ID
 */
static inline uint8_t mavlink_msg_pid_set_get_pid_id(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint8_t)+sizeof(uint8_t))[0];
}

/**
 * @brief Get field k_p from pid_set message
 *
 * @return P
 */
static inline float mavlink_msg_pid_set_get_k_p(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field k_i from pid_set message
 *
 * @return I
 */
static inline float mavlink_msg_pid_set_get_k_i(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field k_d from pid_set message
 *
 * @return D
 */
static inline float mavlink_msg_pid_set_get_k_d(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint8_t)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

static inline void mavlink_msg_pid_set_decode(const mavlink_message_t* msg, mavlink_pid_set_t* pid_set)
{
	pid_set->target_system = mavlink_msg_pid_set_get_target_system(msg);
	pid_set->target_component = mavlink_msg_pid_set_get_target_component(msg);
	pid_set->pid_id = mavlink_msg_pid_set_get_pid_id(msg);
	pid_set->k_p = mavlink_msg_pid_set_get_k_p(msg);
	pid_set->k_i = mavlink_msg_pid_set_get_k_i(msg);
	pid_set->k_d = mavlink_msg_pid_set_get_k_d(msg);
}
