// MESSAGE HIL_STATE PACKING

#define MAVLINK_MSG_ID_HIL_STATE 90

typedef struct __mavlink_hil_state_t
{
 uint64_t time_us; ///< Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 float roll; ///< Roll angle (rad)
 float pitch; ///< Pitch angle (rad)
 float yaw; ///< Yaw angle (rad)
 float rollspeed; ///< Roll angular speed (rad/s)
 float pitchspeed; ///< Pitch angular speed (rad/s)
 float yawspeed; ///< Yaw angular speed (rad/s)
 int32_t lat; ///< Latitude, expressed as * 1E7
 int32_t lon; ///< Longitude, expressed as * 1E7
 int32_t alt; ///< Altitude in meters, expressed as * 1000 (millimeters)
 int16_t vx; ///< Ground X Speed (Latitude), expressed as m/s * 100
 int16_t vy; ///< Ground Y Speed (Longitude), expressed as m/s * 100
 int16_t vz; ///< Ground Z Speed (Altitude), expressed as m/s * 100
 int16_t xacc; ///< X acceleration (mg)
 int16_t yacc; ///< Y acceleration (mg)
 int16_t zacc; ///< Z acceleration (mg)
} mavlink_hil_state_t;

/**
 * @brief Pack a hil_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_us Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param roll Roll angle (rad)
 * @param pitch Pitch angle (rad)
 * @param yaw Yaw angle (rad)
 * @param rollspeed Roll angular speed (rad/s)
 * @param pitchspeed Pitch angular speed (rad/s)
 * @param yawspeed Yaw angular speed (rad/s)
 * @param lat Latitude, expressed as * 1E7
 * @param lon Longitude, expressed as * 1E7
 * @param alt Altitude in meters, expressed as * 1000 (millimeters)
 * @param vx Ground X Speed (Latitude), expressed as m/s * 100
 * @param vy Ground Y Speed (Longitude), expressed as m/s * 100
 * @param vz Ground Z Speed (Altitude), expressed as m/s * 100
 * @param xacc X acceleration (mg)
 * @param yacc Y acceleration (mg)
 * @param zacc Z acceleration (mg)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hil_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint64_t time_us, float roll, float pitch, float yaw, float rollspeed, float pitchspeed, float yawspeed, int32_t lat, int32_t lon, int32_t alt, int16_t vx, int16_t vy, int16_t vz, int16_t xacc, int16_t yacc, int16_t zacc)
{
	msg->msgid = MAVLINK_MSG_ID_HIL_STATE;

	put_uint64_t_by_index(time_us, 0,  msg->payload); // Timestamp (microseconds since UNIX epoch or microseconds since system boot)
	put_float_by_index(roll, 8,  msg->payload); // Roll angle (rad)
	put_float_by_index(pitch, 12,  msg->payload); // Pitch angle (rad)
	put_float_by_index(yaw, 16,  msg->payload); // Yaw angle (rad)
	put_float_by_index(rollspeed, 20,  msg->payload); // Roll angular speed (rad/s)
	put_float_by_index(pitchspeed, 24,  msg->payload); // Pitch angular speed (rad/s)
	put_float_by_index(yawspeed, 28,  msg->payload); // Yaw angular speed (rad/s)
	put_int32_t_by_index(lat, 32,  msg->payload); // Latitude, expressed as * 1E7
	put_int32_t_by_index(lon, 36,  msg->payload); // Longitude, expressed as * 1E7
	put_int32_t_by_index(alt, 40,  msg->payload); // Altitude in meters, expressed as * 1000 (millimeters)
	put_int16_t_by_index(vx, 44,  msg->payload); // Ground X Speed (Latitude), expressed as m/s * 100
	put_int16_t_by_index(vy, 46,  msg->payload); // Ground Y Speed (Longitude), expressed as m/s * 100
	put_int16_t_by_index(vz, 48,  msg->payload); // Ground Z Speed (Altitude), expressed as m/s * 100
	put_int16_t_by_index(xacc, 50,  msg->payload); // X acceleration (mg)
	put_int16_t_by_index(yacc, 52,  msg->payload); // Y acceleration (mg)
	put_int16_t_by_index(zacc, 54,  msg->payload); // Z acceleration (mg)

	return mavlink_finalize_message(msg, system_id, component_id, 56, 224);
}

/**
 * @brief Pack a hil_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_us Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param roll Roll angle (rad)
 * @param pitch Pitch angle (rad)
 * @param yaw Yaw angle (rad)
 * @param rollspeed Roll angular speed (rad/s)
 * @param pitchspeed Pitch angular speed (rad/s)
 * @param yawspeed Yaw angular speed (rad/s)
 * @param lat Latitude, expressed as * 1E7
 * @param lon Longitude, expressed as * 1E7
 * @param alt Altitude in meters, expressed as * 1000 (millimeters)
 * @param vx Ground X Speed (Latitude), expressed as m/s * 100
 * @param vy Ground Y Speed (Longitude), expressed as m/s * 100
 * @param vz Ground Z Speed (Altitude), expressed as m/s * 100
 * @param xacc X acceleration (mg)
 * @param yacc Y acceleration (mg)
 * @param zacc Z acceleration (mg)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hil_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint64_t time_us,float roll,float pitch,float yaw,float rollspeed,float pitchspeed,float yawspeed,int32_t lat,int32_t lon,int32_t alt,int16_t vx,int16_t vy,int16_t vz,int16_t xacc,int16_t yacc,int16_t zacc)
{
	msg->msgid = MAVLINK_MSG_ID_HIL_STATE;

	put_uint64_t_by_index(time_us, 0,  msg->payload); // Timestamp (microseconds since UNIX epoch or microseconds since system boot)
	put_float_by_index(roll, 8,  msg->payload); // Roll angle (rad)
	put_float_by_index(pitch, 12,  msg->payload); // Pitch angle (rad)
	put_float_by_index(yaw, 16,  msg->payload); // Yaw angle (rad)
	put_float_by_index(rollspeed, 20,  msg->payload); // Roll angular speed (rad/s)
	put_float_by_index(pitchspeed, 24,  msg->payload); // Pitch angular speed (rad/s)
	put_float_by_index(yawspeed, 28,  msg->payload); // Yaw angular speed (rad/s)
	put_int32_t_by_index(lat, 32,  msg->payload); // Latitude, expressed as * 1E7
	put_int32_t_by_index(lon, 36,  msg->payload); // Longitude, expressed as * 1E7
	put_int32_t_by_index(alt, 40,  msg->payload); // Altitude in meters, expressed as * 1000 (millimeters)
	put_int16_t_by_index(vx, 44,  msg->payload); // Ground X Speed (Latitude), expressed as m/s * 100
	put_int16_t_by_index(vy, 46,  msg->payload); // Ground Y Speed (Longitude), expressed as m/s * 100
	put_int16_t_by_index(vz, 48,  msg->payload); // Ground Z Speed (Altitude), expressed as m/s * 100
	put_int16_t_by_index(xacc, 50,  msg->payload); // X acceleration (mg)
	put_int16_t_by_index(yacc, 52,  msg->payload); // Y acceleration (mg)
	put_int16_t_by_index(zacc, 54,  msg->payload); // Z acceleration (mg)

	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 56, 224);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

/**
 * @brief Pack a hil_state message on a channel and send
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_us Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param roll Roll angle (rad)
 * @param pitch Pitch angle (rad)
 * @param yaw Yaw angle (rad)
 * @param rollspeed Roll angular speed (rad/s)
 * @param pitchspeed Pitch angular speed (rad/s)
 * @param yawspeed Yaw angular speed (rad/s)
 * @param lat Latitude, expressed as * 1E7
 * @param lon Longitude, expressed as * 1E7
 * @param alt Altitude in meters, expressed as * 1000 (millimeters)
 * @param vx Ground X Speed (Latitude), expressed as m/s * 100
 * @param vy Ground Y Speed (Longitude), expressed as m/s * 100
 * @param vz Ground Z Speed (Altitude), expressed as m/s * 100
 * @param xacc X acceleration (mg)
 * @param yacc Y acceleration (mg)
 * @param zacc Z acceleration (mg)
 */
static inline void mavlink_msg_hil_state_pack_chan_send(mavlink_channel_t chan,
							   mavlink_message_t* msg,
						           uint64_t time_us,float roll,float pitch,float yaw,float rollspeed,float pitchspeed,float yawspeed,int32_t lat,int32_t lon,int32_t alt,int16_t vx,int16_t vy,int16_t vz,int16_t xacc,int16_t yacc,int16_t zacc)
{
	msg->msgid = MAVLINK_MSG_ID_HIL_STATE;

	put_uint64_t_by_index(time_us, 0,  msg->payload); // Timestamp (microseconds since UNIX epoch or microseconds since system boot)
	put_float_by_index(roll, 8,  msg->payload); // Roll angle (rad)
	put_float_by_index(pitch, 12,  msg->payload); // Pitch angle (rad)
	put_float_by_index(yaw, 16,  msg->payload); // Yaw angle (rad)
	put_float_by_index(rollspeed, 20,  msg->payload); // Roll angular speed (rad/s)
	put_float_by_index(pitchspeed, 24,  msg->payload); // Pitch angular speed (rad/s)
	put_float_by_index(yawspeed, 28,  msg->payload); // Yaw angular speed (rad/s)
	put_int32_t_by_index(lat, 32,  msg->payload); // Latitude, expressed as * 1E7
	put_int32_t_by_index(lon, 36,  msg->payload); // Longitude, expressed as * 1E7
	put_int32_t_by_index(alt, 40,  msg->payload); // Altitude in meters, expressed as * 1000 (millimeters)
	put_int16_t_by_index(vx, 44,  msg->payload); // Ground X Speed (Latitude), expressed as m/s * 100
	put_int16_t_by_index(vy, 46,  msg->payload); // Ground Y Speed (Longitude), expressed as m/s * 100
	put_int16_t_by_index(vz, 48,  msg->payload); // Ground Z Speed (Altitude), expressed as m/s * 100
	put_int16_t_by_index(xacc, 50,  msg->payload); // X acceleration (mg)
	put_int16_t_by_index(yacc, 52,  msg->payload); // Y acceleration (mg)
	put_int16_t_by_index(zacc, 54,  msg->payload); // Z acceleration (mg)

	mavlink_finalize_message_chan_send(msg, chan, 56, 224);
}
#endif // MAVLINK_USE_CONVENIENCE_FUNCTIONS


/**
 * @brief Encode a hil_state struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param hil_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hil_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_hil_state_t* hil_state)
{
	return mavlink_msg_hil_state_pack(system_id, component_id, msg, hil_state->time_us, hil_state->roll, hil_state->pitch, hil_state->yaw, hil_state->rollspeed, hil_state->pitchspeed, hil_state->yawspeed, hil_state->lat, hil_state->lon, hil_state->alt, hil_state->vx, hil_state->vy, hil_state->vz, hil_state->xacc, hil_state->yacc, hil_state->zacc);
}

/**
 * @brief Send a hil_state message
 * @param chan MAVLink channel to send the message
 *
 * @param time_us Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param roll Roll angle (rad)
 * @param pitch Pitch angle (rad)
 * @param yaw Yaw angle (rad)
 * @param rollspeed Roll angular speed (rad/s)
 * @param pitchspeed Pitch angular speed (rad/s)
 * @param yawspeed Yaw angular speed (rad/s)
 * @param lat Latitude, expressed as * 1E7
 * @param lon Longitude, expressed as * 1E7
 * @param alt Altitude in meters, expressed as * 1000 (millimeters)
 * @param vx Ground X Speed (Latitude), expressed as m/s * 100
 * @param vy Ground Y Speed (Longitude), expressed as m/s * 100
 * @param vz Ground Z Speed (Altitude), expressed as m/s * 100
 * @param xacc X acceleration (mg)
 * @param yacc Y acceleration (mg)
 * @param zacc Z acceleration (mg)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_hil_state_send(mavlink_channel_t chan, uint64_t time_us, float roll, float pitch, float yaw, float rollspeed, float pitchspeed, float yawspeed, int32_t lat, int32_t lon, int32_t alt, int16_t vx, int16_t vy, int16_t vz, int16_t xacc, int16_t yacc, int16_t zacc)
{
	MAVLINK_ALIGNED_MESSAGE(msg, 56);
	mavlink_msg_hil_state_pack_chan_send(chan, msg, time_us, roll, pitch, yaw, rollspeed, pitchspeed, yawspeed, lat, lon, alt, vx, vy, vz, xacc, yacc, zacc);
}

#endif

// MESSAGE HIL_STATE UNPACKING


/**
 * @brief Get field time_us from hil_state message
 *
 * @return Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 */
static inline uint64_t mavlink_msg_hil_state_get_time_us(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field roll from hil_state message
 *
 * @return Roll angle (rad)
 */
static inline float mavlink_msg_hil_state_get_roll(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  8);
}

/**
 * @brief Get field pitch from hil_state message
 *
 * @return Pitch angle (rad)
 */
static inline float mavlink_msg_hil_state_get_pitch(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  12);
}

/**
 * @brief Get field yaw from hil_state message
 *
 * @return Yaw angle (rad)
 */
static inline float mavlink_msg_hil_state_get_yaw(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  16);
}

/**
 * @brief Get field rollspeed from hil_state message
 *
 * @return Roll angular speed (rad/s)
 */
static inline float mavlink_msg_hil_state_get_rollspeed(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  20);
}

/**
 * @brief Get field pitchspeed from hil_state message
 *
 * @return Pitch angular speed (rad/s)
 */
static inline float mavlink_msg_hil_state_get_pitchspeed(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  24);
}

/**
 * @brief Get field yawspeed from hil_state message
 *
 * @return Yaw angular speed (rad/s)
 */
static inline float mavlink_msg_hil_state_get_yawspeed(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  28);
}

/**
 * @brief Get field lat from hil_state message
 *
 * @return Latitude, expressed as * 1E7
 */
static inline int32_t mavlink_msg_hil_state_get_lat(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_int32_t(msg,  32);
}

/**
 * @brief Get field lon from hil_state message
 *
 * @return Longitude, expressed as * 1E7
 */
static inline int32_t mavlink_msg_hil_state_get_lon(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_int32_t(msg,  36);
}

/**
 * @brief Get field alt from hil_state message
 *
 * @return Altitude in meters, expressed as * 1000 (millimeters)
 */
static inline int32_t mavlink_msg_hil_state_get_alt(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_int32_t(msg,  40);
}

/**
 * @brief Get field vx from hil_state message
 *
 * @return Ground X Speed (Latitude), expressed as m/s * 100
 */
static inline int16_t mavlink_msg_hil_state_get_vx(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_int16_t(msg,  44);
}

/**
 * @brief Get field vy from hil_state message
 *
 * @return Ground Y Speed (Longitude), expressed as m/s * 100
 */
static inline int16_t mavlink_msg_hil_state_get_vy(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_int16_t(msg,  46);
}

/**
 * @brief Get field vz from hil_state message
 *
 * @return Ground Z Speed (Altitude), expressed as m/s * 100
 */
static inline int16_t mavlink_msg_hil_state_get_vz(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_int16_t(msg,  48);
}

/**
 * @brief Get field xacc from hil_state message
 *
 * @return X acceleration (mg)
 */
static inline int16_t mavlink_msg_hil_state_get_xacc(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_int16_t(msg,  50);
}

/**
 * @brief Get field yacc from hil_state message
 *
 * @return Y acceleration (mg)
 */
static inline int16_t mavlink_msg_hil_state_get_yacc(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_int16_t(msg,  52);
}

/**
 * @brief Get field zacc from hil_state message
 *
 * @return Z acceleration (mg)
 */
static inline int16_t mavlink_msg_hil_state_get_zacc(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_int16_t(msg,  54);
}

/**
 * @brief Decode a hil_state message into a struct
 *
 * @param msg The message to decode
 * @param hil_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_hil_state_decode(const mavlink_message_t* msg, mavlink_hil_state_t* hil_state)
{
#if MAVLINK_NEED_BYTE_SWAP
	hil_state->time_us = mavlink_msg_hil_state_get_time_us(msg);
	hil_state->roll = mavlink_msg_hil_state_get_roll(msg);
	hil_state->pitch = mavlink_msg_hil_state_get_pitch(msg);
	hil_state->yaw = mavlink_msg_hil_state_get_yaw(msg);
	hil_state->rollspeed = mavlink_msg_hil_state_get_rollspeed(msg);
	hil_state->pitchspeed = mavlink_msg_hil_state_get_pitchspeed(msg);
	hil_state->yawspeed = mavlink_msg_hil_state_get_yawspeed(msg);
	hil_state->lat = mavlink_msg_hil_state_get_lat(msg);
	hil_state->lon = mavlink_msg_hil_state_get_lon(msg);
	hil_state->alt = mavlink_msg_hil_state_get_alt(msg);
	hil_state->vx = mavlink_msg_hil_state_get_vx(msg);
	hil_state->vy = mavlink_msg_hil_state_get_vy(msg);
	hil_state->vz = mavlink_msg_hil_state_get_vz(msg);
	hil_state->xacc = mavlink_msg_hil_state_get_xacc(msg);
	hil_state->yacc = mavlink_msg_hil_state_get_yacc(msg);
	hil_state->zacc = mavlink_msg_hil_state_get_zacc(msg);
#else
	memcpy(hil_state, msg->payload, 56);
#endif
}
