// MESSAGE RAW_IMU PACKING

#define MAVLINK_MSG_ID_RAW_IMU 101

typedef struct __raw_imu_t 
{
	uint32_t msec; ///< Timestamp (milliseconds)
	int32_t xacc; ///< X acceleration (mg raw)
	int32_t yacc; ///< Y acceleration (mg raw)
	int32_t zacc; ///< Z acceleration (mg raw)
	uint32_t xgyro; ///< Angular speed around X axis (adc units)
	uint32_t ygyro; ///< Angular speed around Y axis (adc units)
	uint32_t zgyro; ///< Angular speed around Z axis (adc units)
	int32_t xmag; ///< X Magnetic field (milli tesla)
	int32_t ymag; ///< Y Magnetic field (milli tesla)
	int32_t zmag; ///< Z Magnetic field (milli tesla)

} raw_imu_t;

/**
 * @brief Send a raw_imu message
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
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_raw_imu_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint32_t msec, int32_t xacc, int32_t yacc, int32_t zacc, uint32_t xgyro, uint32_t ygyro, uint32_t zgyro, int32_t xmag, int32_t ymag, int32_t zmag)
{
	msg->msgid = MAVLINK_MSG_ID_RAW_IMU;
	uint16_t i = 0;

	i += put_uint32_t_by_index(msec, i, msg->payload); //Timestamp (milliseconds)
	i += put_int32_t_by_index(xacc, i, msg->payload); //X acceleration (mg raw)
	i += put_int32_t_by_index(yacc, i, msg->payload); //Y acceleration (mg raw)
	i += put_int32_t_by_index(zacc, i, msg->payload); //Z acceleration (mg raw)
	i += put_uint32_t_by_index(xgyro, i, msg->payload); //Angular speed around X axis (adc units)
	i += put_uint32_t_by_index(ygyro, i, msg->payload); //Angular speed around Y axis (adc units)
	i += put_uint32_t_by_index(zgyro, i, msg->payload); //Angular speed around Z axis (adc units)
	i += put_int32_t_by_index(xmag, i, msg->payload); //X Magnetic field (milli tesla)
	i += put_int32_t_by_index(ymag, i, msg->payload); //Y Magnetic field (milli tesla)
	i += put_int32_t_by_index(zmag, i, msg->payload); //Z Magnetic field (milli tesla)

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_raw_imu_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const raw_imu_t* raw_imu)
{
	return message_raw_imu_pack(system_id, component_id, msg, raw_imu->msec, raw_imu->xacc, raw_imu->yacc, raw_imu->zacc, raw_imu->xgyro, raw_imu->ygyro, raw_imu->zgyro, raw_imu->xmag, raw_imu->ymag, raw_imu->zmag);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_raw_imu_send(mavlink_channel_t chan, uint32_t msec, int32_t xacc, int32_t yacc, int32_t zacc, uint32_t xgyro, uint32_t ygyro, uint32_t zgyro, int32_t xmag, int32_t ymag, int32_t zmag)
{
	mavlink_message_t msg;
	message_raw_imu_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, msec, xacc, yacc, zacc, xgyro, ygyro, zgyro, xmag, ymag, zmag);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE RAW_IMU UNPACKING

/**
 * @brief Get field msec from raw_imu message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint32_t message_raw_imu_get_msec(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload)[0];
	r.b[2] = (msg->payload)[1];
	r.b[1] = (msg->payload)[2];
	r.b[0] = (msg->payload)[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field xacc from raw_imu message
 *
 * @return X acceleration (mg raw)
 */
static inline int32_t message_raw_imu_get_xacc(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field yacc from raw_imu message
 *
 * @return Y acceleration (mg raw)
 */
static inline int32_t message_raw_imu_get_yacc(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field zacc from raw_imu message
 *
 * @return Z acceleration (mg raw)
 */
static inline int32_t message_raw_imu_get_zacc(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field xgyro from raw_imu message
 *
 * @return Angular speed around X axis (adc units)
 */
static inline uint32_t message_raw_imu_get_xgyro(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field ygyro from raw_imu message
 *
 * @return Angular speed around Y axis (adc units)
 */
static inline uint32_t message_raw_imu_get_ygyro(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field zgyro from raw_imu message
 *
 * @return Angular speed around Z axis (adc units)
 */
static inline uint32_t message_raw_imu_get_zgyro(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field xmag from raw_imu message
 *
 * @return X Magnetic field (milli tesla)
 */
static inline int32_t message_raw_imu_get_xmag(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field ymag from raw_imu message
 *
 * @return Y Magnetic field (milli tesla)
 */
static inline int32_t message_raw_imu_get_ymag(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field zmag from raw_imu message
 *
 * @return Z Magnetic field (milli tesla)
 */
static inline int32_t message_raw_imu_get_zmag(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(int32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(int32_t)+sizeof(int32_t))[3];
	return (int32_t)r.i;
}

static inline void message_raw_imu_decode(const mavlink_message_t* msg, raw_imu_t* raw_imu)
{
	raw_imu->msec = message_raw_imu_get_msec(msg);
	raw_imu->xacc = message_raw_imu_get_xacc(msg);
	raw_imu->yacc = message_raw_imu_get_yacc(msg);
	raw_imu->zacc = message_raw_imu_get_zacc(msg);
	raw_imu->xgyro = message_raw_imu_get_xgyro(msg);
	raw_imu->ygyro = message_raw_imu_get_ygyro(msg);
	raw_imu->zgyro = message_raw_imu_get_zgyro(msg);
	raw_imu->xmag = message_raw_imu_get_xmag(msg);
	raw_imu->ymag = message_raw_imu_get_ymag(msg);
	raw_imu->zmag = message_raw_imu_get_zmag(msg);
}
