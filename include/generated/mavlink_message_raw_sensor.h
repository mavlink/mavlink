// MESSAGE RAW_SENSOR PACKING

#define MAVLINK_MSG_ID_RAW_SENSOR 100

typedef struct __raw_sensor_t 
{
	uint32_t msec; ///< Timestamp (milliseconds)
	int32_t xacc; ///< X acceleration (mg raw)
	int32_t yacc; ///< Y acceleration (mg raw)
	int32_t zacc; ///< Z acceleration (mg raw)
	uint32_t xgyro; ///< Angular speed around X axis (adc units)
	uint32_t ygyro; ///< Angular speed around Y axis (adc units)
	uint32_t zgyro; ///< Angular speed around Z axis (adc units)
	uint32_t xmag; ///< X Magnetic field (milli tesla)
	uint32_t ymag; ///< Y Magnetic field (milli tesla)
	uint32_t zmag; ///< Z Magnetic field (milli tesla)
	int32_t baro; ///< Barometric pressure (hecto Pascal)
	uint32_t gdist; ///< Ground distance (meters)
	int32_t temp; ///< Temperature (degrees celcius)

} raw_sensor_t;

/**
 * @brief Send a raw_sensor message
 *
 * @param msec Timestamp (milliseconds)
 * @param xacc X acceleration (mg raw)
 * @param yacc Y acceleration (mg raw)
 * @param zacc Z acceleration (mg raw)
 * @param xgyro Angular speed around X axis (adc units)
 * @param ygyro Angular speed around Y axis (adc units)
 * @param zgyro Angular speed around Z axis (adc units)
 * @param xmag X Magnetic field (milli tesla)
 * @param ymag Y Magnetic field (milli tesla)
 * @param zmag Z Magnetic field (milli tesla)
 * @param baro Barometric pressure (hecto Pascal)
 * @param gdist Ground distance (meters)
 * @param temp Temperature (degrees celcius)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_raw_sensor_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint32_t msec, int32_t xacc, int32_t yacc, int32_t zacc, uint32_t xgyro, uint32_t ygyro, uint32_t zgyro, uint32_t xmag, uint32_t ymag, uint32_t zmag, int32_t baro, uint32_t gdist, int32_t temp)
{
	msg->msgid = MAVLINK_MSG_ID_RAW_SENSOR;
	uint16_t i = 0;

	i += put_uint32_t_by_index(msec, i, msg->payload); //Timestamp (milliseconds)
	i += put_int32_t_by_index(xacc, i, msg->payload); //X acceleration (mg raw)
	i += put_int32_t_by_index(yacc, i, msg->payload); //Y acceleration (mg raw)
	i += put_int32_t_by_index(zacc, i, msg->payload); //Z acceleration (mg raw)
	i += put_uint32_t_by_index(xgyro, i, msg->payload); //Angular speed around X axis (adc units)
	i += put_uint32_t_by_index(ygyro, i, msg->payload); //Angular speed around Y axis (adc units)
	i += put_uint32_t_by_index(zgyro, i, msg->payload); //Angular speed around Z axis (adc units)
	i += put_uint32_t_by_index(xmag, i, msg->payload); //X Magnetic field (milli tesla)
	i += put_uint32_t_by_index(ymag, i, msg->payload); //Y Magnetic field (milli tesla)
	i += put_uint32_t_by_index(zmag, i, msg->payload); //Z Magnetic field (milli tesla)
	i += put_int32_t_by_index(baro, i, msg->payload); //Barometric pressure (hecto Pascal)
	i += put_uint32_t_by_index(gdist, i, msg->payload); //Ground distance (meters)
	i += put_int32_t_by_index(temp, i, msg->payload); //Temperature (degrees celcius)

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_raw_sensor_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const raw_sensor_t* raw_sensor)
{
	return message_raw_sensor_pack(system_id, component_id, msg, raw_sensor->msec, raw_sensor->xacc, raw_sensor->yacc, raw_sensor->zacc, raw_sensor->xgyro, raw_sensor->ygyro, raw_sensor->zgyro, raw_sensor->xmag, raw_sensor->ymag, raw_sensor->zmag, raw_sensor->baro, raw_sensor->gdist, raw_sensor->temp);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_raw_sensor_send(mavlink_channel_t chan, uint32_t msec, int32_t xacc, int32_t yacc, int32_t zacc, uint32_t xgyro, uint32_t ygyro, uint32_t zgyro, uint32_t xmag, uint32_t ymag, uint32_t zmag, int32_t baro, uint32_t gdist, int32_t temp)
{
	mavlink_message_t msg;
	message_raw_sensor_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, msec, xacc, yacc, zacc, xgyro, ygyro, zgyro, xmag, ymag, zmag, baro, gdist, temp);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE RAW_SENSOR UNPACKING

/**
 * @brief Get field msec from raw_sensor message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint32_t message_raw_sensor_get_msec(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload)[0];
	r.b[2] = (msg->payload)[1];
	r.b[1] = (msg->payload)[2];
	r.b[0] = (msg->payload)[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field xacc from raw_sensor message
 *
 * @return X acceleration (mg raw)
 */
static inline int32_t message_raw_sensor_get_xacc(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field yacc from raw_sensor message
 *
 * @return Y acceleration (mg raw)
 */
static inline int32_t message_raw_sensor_get_yacc(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field zacc from raw_sensor message
 *
 * @return Z acceleration (mg raw)
 */
static inline int32_t message_raw_sensor_get_zacc(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field xgyro from raw_sensor message
 *
 * @return Angular speed around X axis (adc units)
 */
static inline uint32_t message_raw_sensor_get_xgyro(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field ygyro from raw_sensor message
 *
 * @return Angular speed around Y axis (adc units)
 */
static inline uint32_t message_raw_sensor_get_ygyro(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field zgyro from raw_sensor message
 *
 * @return Angular speed around Z axis (adc units)
 */
static inline uint32_t message_raw_sensor_get_zgyro(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field xmag from raw_sensor message
 *
 * @return X Magnetic field (milli tesla)
 */
static inline uint32_t message_raw_sensor_get_xmag(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field ymag from raw_sensor message
 *
 * @return Y Magnetic field (milli tesla)
 */
static inline uint32_t message_raw_sensor_get_ymag(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field zmag from raw_sensor message
 *
 * @return Z Magnetic field (milli tesla)
 */
static inline uint32_t message_raw_sensor_get_zmag(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field baro from raw_sensor message
 *
 * @return Barometric pressure (hecto Pascal)
 */
static inline int32_t message_raw_sensor_get_baro(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field gdist from raw_sensor message
 *
 * @return Ground distance (meters)
 */
static inline uint32_t message_raw_sensor_get_gdist(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field temp from raw_sensor message
 *
 * @return Temperature (degrees celcius)
 */
static inline int32_t message_raw_sensor_get_temp(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t)+sizeof(uint32_t))[3];
	return (int32_t)r.i;
}

static inline void message_raw_sensor_decode(const mavlink_message_t* msg, raw_sensor_t* raw_sensor)
{
	raw_sensor->msec = message_raw_sensor_get_msec(msg);
	raw_sensor->xacc = message_raw_sensor_get_xacc(msg);
	raw_sensor->yacc = message_raw_sensor_get_yacc(msg);
	raw_sensor->zacc = message_raw_sensor_get_zacc(msg);
	raw_sensor->xgyro = message_raw_sensor_get_xgyro(msg);
	raw_sensor->ygyro = message_raw_sensor_get_ygyro(msg);
	raw_sensor->zgyro = message_raw_sensor_get_zgyro(msg);
	raw_sensor->xmag = message_raw_sensor_get_xmag(msg);
	raw_sensor->ymag = message_raw_sensor_get_ymag(msg);
	raw_sensor->zmag = message_raw_sensor_get_zmag(msg);
	raw_sensor->baro = message_raw_sensor_get_baro(msg);
	raw_sensor->gdist = message_raw_sensor_get_gdist(msg);
	raw_sensor->temp = message_raw_sensor_get_temp(msg);
}
