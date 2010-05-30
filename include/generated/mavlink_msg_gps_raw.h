// MESSAGE GPS_RAW PACKING

#define MAVLINK_MSG_ID_GPS_RAW 32

typedef struct __mavlink_gps_raw_t 
{
	uint64_t usec; ///< Timestamp (microseconds since unix epoch)
	float lat; ///< X Position
	float lon; ///< Y Position
	float alt; ///< Z Position
	float vx; ///< X Speed
	float vy; ///< Y Speed
	float vz; ///< Z Speed

} mavlink_gps_raw_t;



/**
 * @brief Send a gps_raw message
 *
 * @param usec Timestamp (microseconds since unix epoch)
 * @param lat X Position
 * @param lon Y Position
 * @param alt Z Position
 * @param vx X Speed
 * @param vy Y Speed
 * @param vz Z Speed
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_gps_raw_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint64_t usec, float lat, float lon, float alt, float vx, float vy, float vz)
{
	msg->msgid = MAVLINK_MSG_ID_GPS_RAW;
	uint16_t i = 0;

	i += put_uint64_t_by_index(usec, i, msg->payload); //Timestamp (microseconds since unix epoch)
	i += put_float_by_index(lat, i, msg->payload); //X Position
	i += put_float_by_index(lon, i, msg->payload); //Y Position
	i += put_float_by_index(alt, i, msg->payload); //Z Position
	i += put_float_by_index(vx, i, msg->payload); //X Speed
	i += put_float_by_index(vy, i, msg->payload); //Y Speed
	i += put_float_by_index(vz, i, msg->payload); //Z Speed

	return mavlink_finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t mavlink_msg_gps_raw_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_gps_raw_t* gps_raw)
{
	return mavlink_msg_gps_raw_pack(system_id, component_id, msg, gps_raw->usec, gps_raw->lat, gps_raw->lon, gps_raw->alt, gps_raw->vx, gps_raw->vy, gps_raw->vz);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_gps_raw_send(mavlink_channel_t chan, uint64_t usec, float lat, float lon, float alt, float vx, float vy, float vz)
{
	mavlink_message_t msg;
	mavlink_msg_gps_raw_pack(mavlink_system.sysid, mavlink_system.compid, &msg, usec, lat, lon, alt, vx, vy, vz);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE GPS_RAW UNPACKING

/**
 * @brief Get field usec from gps_raw message
 *
 * @return Timestamp (microseconds since unix epoch)
 */
static inline uint64_t mavlink_msg_gps_raw_get_usec(const mavlink_message_t* msg)
{
	generic_64bit r;
	r.b[7] = (msg->payload)[0];
	r.b[6] = (msg->payload)[1];
	r.b[5] = (msg->payload)[2];
	r.b[4] = (msg->payload)[3];
	r.b[3] = (msg->payload)[4];
	r.b[2] = (msg->payload)[5];
	r.b[1] = (msg->payload)[6];
	r.b[0] = (msg->payload)[7];
	return (uint64_t)r.ll;
}

/**
 * @brief Get field lat from gps_raw message
 *
 * @return X Position
 */
static inline float mavlink_msg_gps_raw_get_lat(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t))[3];
	return (float)r.f;
}

/**
 * @brief Get field lon from gps_raw message
 *
 * @return Y Position
 */
static inline float mavlink_msg_gps_raw_get_lon(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field alt from gps_raw message
 *
 * @return Z Position
 */
static inline float mavlink_msg_gps_raw_get_alt(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vx from gps_raw message
 *
 * @return X Speed
 */
static inline float mavlink_msg_gps_raw_get_vx(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vy from gps_raw message
 *
 * @return Y Speed
 */
static inline float mavlink_msg_gps_raw_get_vy(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

/**
 * @brief Get field vz from gps_raw message
 *
 * @return Z Speed
 */
static inline float mavlink_msg_gps_raw_get_vz(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[0];
	r.b[2] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[1];
	r.b[1] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[2];
	r.b[0] = (msg->payload+sizeof(uint64_t)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float)+sizeof(float))[3];
	return (float)r.f;
}

static inline void mavlink_msg_gps_raw_decode(const mavlink_message_t* msg, mavlink_gps_raw_t* gps_raw)
{
	gps_raw->usec = mavlink_msg_gps_raw_get_usec(msg);
	gps_raw->lat = mavlink_msg_gps_raw_get_lat(msg);
	gps_raw->lon = mavlink_msg_gps_raw_get_lon(msg);
	gps_raw->alt = mavlink_msg_gps_raw_get_alt(msg);
	gps_raw->vx = mavlink_msg_gps_raw_get_vx(msg);
	gps_raw->vy = mavlink_msg_gps_raw_get_vy(msg);
	gps_raw->vz = mavlink_msg_gps_raw_get_vz(msg);
}
