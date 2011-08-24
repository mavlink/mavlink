// MESSAGE GPS_RAW PACKING

#define MAVLINK_MSG_ID_GPS_RAW 32

typedef struct __mavlink_gps_raw_t
{
 uint64_t usec; ///< Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 float lat; ///< Latitude in degrees
 float lon; ///< Longitude in degrees
 float alt; ///< Altitude in meters
 float eph; ///< GPS HDOP
 float epv; ///< GPS VDOP
 float v; ///< GPS ground speed
 float hdg; ///< Compass heading in degrees, 0..360 degrees
 uint8_t fix_type; ///< 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
 uint8_t satellites_visible; ///< Number of satellites visible
} mavlink_gps_raw_t;

/**
 * @brief Pack a gps_raw message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param usec Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param fix_type 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
 * @param lat Latitude in degrees
 * @param lon Longitude in degrees
 * @param alt Altitude in meters
 * @param eph GPS HDOP
 * @param epv GPS VDOP
 * @param v GPS ground speed
 * @param hdg Compass heading in degrees, 0..360 degrees
 * @param satellites_visible Number of satellites visible
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_gps_raw_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint64_t usec, uint8_t fix_type, float lat, float lon, float alt, float eph, float epv, float v, float hdg, uint8_t satellites_visible)
{
	msg->msgid = MAVLINK_MSG_ID_GPS_RAW;

	put_uint64_t_by_index(usec, 0,  msg->payload); // Timestamp (microseconds since UNIX epoch or microseconds since system boot)
	put_float_by_index(lat, 8,  msg->payload); // Latitude in degrees
	put_float_by_index(lon, 12,  msg->payload); // Longitude in degrees
	put_float_by_index(alt, 16,  msg->payload); // Altitude in meters
	put_float_by_index(eph, 20,  msg->payload); // GPS HDOP
	put_float_by_index(epv, 24,  msg->payload); // GPS VDOP
	put_float_by_index(v, 28,  msg->payload); // GPS ground speed
	put_float_by_index(hdg, 32,  msg->payload); // Compass heading in degrees, 0..360 degrees
	put_uint8_t_by_index(fix_type, 36,  msg->payload); // 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
	put_uint8_t_by_index(satellites_visible, 37,  msg->payload); // Number of satellites visible

	return mavlink_finalize_message(msg, system_id, component_id, 38, 138);
}

/**
 * @brief Pack a gps_raw message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param usec Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param fix_type 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
 * @param lat Latitude in degrees
 * @param lon Longitude in degrees
 * @param alt Altitude in meters
 * @param eph GPS HDOP
 * @param epv GPS VDOP
 * @param v GPS ground speed
 * @param hdg Compass heading in degrees, 0..360 degrees
 * @param satellites_visible Number of satellites visible
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_gps_raw_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint64_t usec,uint8_t fix_type,float lat,float lon,float alt,float eph,float epv,float v,float hdg,uint8_t satellites_visible)
{
	msg->msgid = MAVLINK_MSG_ID_GPS_RAW;

	put_uint64_t_by_index(usec, 0,  msg->payload); // Timestamp (microseconds since UNIX epoch or microseconds since system boot)
	put_float_by_index(lat, 8,  msg->payload); // Latitude in degrees
	put_float_by_index(lon, 12,  msg->payload); // Longitude in degrees
	put_float_by_index(alt, 16,  msg->payload); // Altitude in meters
	put_float_by_index(eph, 20,  msg->payload); // GPS HDOP
	put_float_by_index(epv, 24,  msg->payload); // GPS VDOP
	put_float_by_index(v, 28,  msg->payload); // GPS ground speed
	put_float_by_index(hdg, 32,  msg->payload); // Compass heading in degrees, 0..360 degrees
	put_uint8_t_by_index(fix_type, 36,  msg->payload); // 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
	put_uint8_t_by_index(satellites_visible, 37,  msg->payload); // Number of satellites visible

	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 38, 138);
}

#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

/**
 * @brief Pack a gps_raw message on a channel and send
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param usec Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param fix_type 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
 * @param lat Latitude in degrees
 * @param lon Longitude in degrees
 * @param alt Altitude in meters
 * @param eph GPS HDOP
 * @param epv GPS VDOP
 * @param v GPS ground speed
 * @param hdg Compass heading in degrees, 0..360 degrees
 * @param satellites_visible Number of satellites visible
 */
static inline void mavlink_msg_gps_raw_pack_chan_send(mavlink_channel_t chan,
							   mavlink_message_t* msg,
						           uint64_t usec,uint8_t fix_type,float lat,float lon,float alt,float eph,float epv,float v,float hdg,uint8_t satellites_visible)
{
	msg->msgid = MAVLINK_MSG_ID_GPS_RAW;

	put_uint64_t_by_index(usec, 0,  msg->payload); // Timestamp (microseconds since UNIX epoch or microseconds since system boot)
	put_float_by_index(lat, 8,  msg->payload); // Latitude in degrees
	put_float_by_index(lon, 12,  msg->payload); // Longitude in degrees
	put_float_by_index(alt, 16,  msg->payload); // Altitude in meters
	put_float_by_index(eph, 20,  msg->payload); // GPS HDOP
	put_float_by_index(epv, 24,  msg->payload); // GPS VDOP
	put_float_by_index(v, 28,  msg->payload); // GPS ground speed
	put_float_by_index(hdg, 32,  msg->payload); // Compass heading in degrees, 0..360 degrees
	put_uint8_t_by_index(fix_type, 36,  msg->payload); // 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
	put_uint8_t_by_index(satellites_visible, 37,  msg->payload); // Number of satellites visible

	mavlink_finalize_message_chan_send(msg, chan, 38, 138);
}
#endif // MAVLINK_USE_CONVENIENCE_FUNCTIONS


/**
 * @brief Encode a gps_raw struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param gps_raw C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_gps_raw_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_gps_raw_t* gps_raw)
{
	return mavlink_msg_gps_raw_pack(system_id, component_id, msg, gps_raw->usec, gps_raw->fix_type, gps_raw->lat, gps_raw->lon, gps_raw->alt, gps_raw->eph, gps_raw->epv, gps_raw->v, gps_raw->hdg, gps_raw->satellites_visible);
}

/**
 * @brief Send a gps_raw message
 * @param chan MAVLink channel to send the message
 *
 * @param usec Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 * @param fix_type 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
 * @param lat Latitude in degrees
 * @param lon Longitude in degrees
 * @param alt Altitude in meters
 * @param eph GPS HDOP
 * @param epv GPS VDOP
 * @param v GPS ground speed
 * @param hdg Compass heading in degrees, 0..360 degrees
 * @param satellites_visible Number of satellites visible
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_gps_raw_send(mavlink_channel_t chan, uint64_t usec, uint8_t fix_type, float lat, float lon, float alt, float eph, float epv, float v, float hdg, uint8_t satellites_visible)
{
	MAVLINK_ALIGNED_MESSAGE(msg, 38);
	mavlink_msg_gps_raw_pack_chan_send(chan, msg, usec, fix_type, lat, lon, alt, eph, epv, v, hdg, satellites_visible);
}

#endif

// MESSAGE GPS_RAW UNPACKING


/**
 * @brief Get field usec from gps_raw message
 *
 * @return Timestamp (microseconds since UNIX epoch or microseconds since system boot)
 */
static inline uint64_t mavlink_msg_gps_raw_get_usec(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field fix_type from gps_raw message
 *
 * @return 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
 */
static inline uint8_t mavlink_msg_gps_raw_get_fix_type(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint8_t(msg,  36);
}

/**
 * @brief Get field lat from gps_raw message
 *
 * @return Latitude in degrees
 */
static inline float mavlink_msg_gps_raw_get_lat(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  8);
}

/**
 * @brief Get field lon from gps_raw message
 *
 * @return Longitude in degrees
 */
static inline float mavlink_msg_gps_raw_get_lon(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  12);
}

/**
 * @brief Get field alt from gps_raw message
 *
 * @return Altitude in meters
 */
static inline float mavlink_msg_gps_raw_get_alt(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  16);
}

/**
 * @brief Get field eph from gps_raw message
 *
 * @return GPS HDOP
 */
static inline float mavlink_msg_gps_raw_get_eph(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  20);
}

/**
 * @brief Get field epv from gps_raw message
 *
 * @return GPS VDOP
 */
static inline float mavlink_msg_gps_raw_get_epv(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  24);
}

/**
 * @brief Get field v from gps_raw message
 *
 * @return GPS ground speed
 */
static inline float mavlink_msg_gps_raw_get_v(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  28);
}

/**
 * @brief Get field hdg from gps_raw message
 *
 * @return Compass heading in degrees, 0..360 degrees
 */
static inline float mavlink_msg_gps_raw_get_hdg(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_float(msg,  32);
}

/**
 * @brief Get field satellites_visible from gps_raw message
 *
 * @return Number of satellites visible
 */
static inline uint8_t mavlink_msg_gps_raw_get_satellites_visible(const mavlink_message_t* msg)
{
	return MAVLINK_MSG_RETURN_uint8_t(msg,  37);
}

/**
 * @brief Decode a gps_raw message into a struct
 *
 * @param msg The message to decode
 * @param gps_raw C-struct to decode the message contents into
 */
static inline void mavlink_msg_gps_raw_decode(const mavlink_message_t* msg, mavlink_gps_raw_t* gps_raw)
{
#if MAVLINK_NEED_BYTE_SWAP
	gps_raw->usec = mavlink_msg_gps_raw_get_usec(msg);
	gps_raw->lat = mavlink_msg_gps_raw_get_lat(msg);
	gps_raw->lon = mavlink_msg_gps_raw_get_lon(msg);
	gps_raw->alt = mavlink_msg_gps_raw_get_alt(msg);
	gps_raw->eph = mavlink_msg_gps_raw_get_eph(msg);
	gps_raw->epv = mavlink_msg_gps_raw_get_epv(msg);
	gps_raw->v = mavlink_msg_gps_raw_get_v(msg);
	gps_raw->hdg = mavlink_msg_gps_raw_get_hdg(msg);
	gps_raw->fix_type = mavlink_msg_gps_raw_get_fix_type(msg);
	gps_raw->satellites_visible = mavlink_msg_gps_raw_get_satellites_visible(msg);
#else
	memcpy(gps_raw, msg->payload, 38);
#endif
}
