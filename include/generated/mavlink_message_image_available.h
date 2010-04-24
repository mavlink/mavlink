// MESSAGE IMAGE_AVAILABLE PACKING

#define MAVLINK_MSG_ID_IMAGE_AVAILABLE 72

typedef struct __image_available_t 
{
	uint32_t cam_id; ///< Camera id
	uint8_t cam_no; ///< Camera # (starts with 0)
	uint32_t timestamp; ///< Timestamp
	uint32_t img_seq; ///< The image sequence number
	uint32_t img_buf_index; ///< Position of the image in the buffer, starts with 0
	uint16_t width; ///< Image width
	uint16_t height; ///< Image height
	uint16_t depth; ///< Image depth
	uint8_t channels; ///< Image channels
	int32_t key; ///< Shared memory area key
	uint16_t exposure; ///< Exposure time, in microseconds
	float gain; ///< Camera gain

} image_available_t;

/**
 * @brief Send a image_available message
 *
 * @param cam_id Camera id
 * @param cam_no Camera # (starts with 0)
 * @param timestamp Timestamp
 * @param img_seq The image sequence number
 * @param img_buf_index Position of the image in the buffer, starts with 0
 * @param width Image width
 * @param height Image height
 * @param depth Image depth
 * @param channels Image channels
 * @param key Shared memory area key
 * @param exposure Exposure time, in microseconds
 * @param gain Camera gain
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t message_image_available_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint32_t cam_id, uint8_t cam_no, uint32_t timestamp, uint32_t img_seq, uint32_t img_buf_index, uint16_t width, uint16_t height, uint16_t depth, uint8_t channels, int32_t key, uint16_t exposure, float gain)
{
	msg->msgid = MAVLINK_MSG_ID_IMAGE_AVAILABLE;
	uint16_t i = 0;

	i += put_uint32_t_by_index(cam_id, i, msg->payload); //Camera id
	i += put_uint8_t_by_index(cam_no, i, msg->payload); //Camera # (starts with 0)
	i += put_uint32_t_by_index(timestamp, i, msg->payload); //Timestamp
	i += put_uint32_t_by_index(img_seq, i, msg->payload); //The image sequence number
	i += put_uint32_t_by_index(img_buf_index, i, msg->payload); //Position of the image in the buffer, starts with 0
	i += put_uint16_t_by_index(width, i, msg->payload); //Image width
	i += put_uint16_t_by_index(height, i, msg->payload); //Image height
	i += put_uint16_t_by_index(depth, i, msg->payload); //Image depth
	i += put_uint8_t_by_index(channels, i, msg->payload); //Image channels
	i += put_int32_t_by_index(key, i, msg->payload); //Shared memory area key
	i += put_uint16_t_by_index(exposure, i, msg->payload); //Exposure time, in microseconds
	i += put_float_by_index(gain, i, msg->payload); //Camera gain

	return finalize_message(msg, system_id, component_id, i);
}

static inline uint16_t message_image_available_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const image_available_t* image_available)
{
	return message_image_available_pack(system_id, component_id, msg, image_available->cam_id, image_available->cam_no, image_available->timestamp, image_available->img_seq, image_available->img_buf_index, image_available->width, image_available->height, image_available->depth, image_available->channels, image_available->key, image_available->exposure, image_available->gain);
}

#if !defined(_WIN32) && !defined(__linux) && !defined(__APPLE__)

#include "global_data.h"

static inline void message_image_available_send(mavlink_channel_t chan, uint32_t cam_id, uint8_t cam_no, uint32_t timestamp, uint32_t img_seq, uint32_t img_buf_index, uint16_t width, uint16_t height, uint16_t depth, uint8_t channels, int32_t key, uint16_t exposure, float gain)
{
	mavlink_message_t msg;
	message_image_available_pack(global_data.param[PARAM_SYSTEM_ID], global_data.param[PARAM_COMPONENT_ID], &msg, cam_id, cam_no, timestamp, img_seq, img_buf_index, width, height, depth, channels, key, exposure, gain);
	mavlink_send_uart(chan, &msg);
}

#endif
// MESSAGE IMAGE_AVAILABLE UNPACKING

/**
 * @brief Get field cam_id from image_available message
 *
 * @return Camera id
 */
static inline uint32_t message_image_available_get_cam_id(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload)[0];
	r.b[2] = (msg->payload)[1];
	r.b[1] = (msg->payload)[2];
	r.b[0] = (msg->payload)[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field cam_no from image_available message
 *
 * @return Camera # (starts with 0)
 */
static inline uint8_t message_image_available_get_cam_no(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint32_t))[0];
}

/**
 * @brief Get field timestamp from image_available message
 *
 * @return Timestamp
 */
static inline uint32_t message_image_available_get_timestamp(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field img_seq from image_available message
 *
 * @return The image sequence number
 */
static inline uint32_t message_image_available_get_img_seq(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field img_buf_index from image_available message
 *
 * @return Position of the image in the buffer, starts with 0
 */
static inline uint32_t message_image_available_get_img_buf_index(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t))[3];
	return (uint32_t)r.i;
}

/**
 * @brief Get field width from image_available message
 *
 * @return Image width
 */
static inline uint16_t message_image_available_get_width(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[0];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field height from image_available message
 *
 * @return Image height
 */
static inline uint16_t message_image_available_get_height(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t))[0];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field depth from image_available message
 *
 * @return Image depth
 */
static inline uint16_t message_image_available_get_depth(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field channels from image_available message
 *
 * @return Image channels
 */
static inline uint8_t message_image_available_get_channels(const mavlink_message_t* msg)
{
	return (uint8_t)(msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t))[0];
}

/**
 * @brief Get field key from image_available message
 *
 * @return Shared memory area key
 */
static inline int32_t message_image_available_get_key(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t))[3];
	return (int32_t)r.i;
}

/**
 * @brief Get field exposure from image_available message
 *
 * @return Exposure time, in microseconds
 */
static inline uint16_t message_image_available_get_exposure(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t)+sizeof(int32_t))[0];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t)+sizeof(int32_t))[1];
	return (uint16_t)r.s;
}

/**
 * @brief Get field gain from image_available message
 *
 * @return Camera gain
 */
static inline float message_image_available_get_gain(const mavlink_message_t* msg)
{
	generic_32bit r;
	r.b[3] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t)+sizeof(int32_t)+sizeof(uint16_t))[0];
	r.b[2] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t)+sizeof(int32_t)+sizeof(uint16_t))[1];
	r.b[1] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t)+sizeof(int32_t)+sizeof(uint16_t))[2];
	r.b[0] = (msg->payload+sizeof(uint32_t)+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t)+sizeof(int32_t)+sizeof(uint16_t))[3];
	return (float)r.f;
}

static inline void message_image_available_decode(const mavlink_message_t* msg, image_available_t* image_available)
{
	image_available->cam_id = message_image_available_get_cam_id(msg);
	image_available->cam_no = message_image_available_get_cam_no(msg);
	image_available->timestamp = message_image_available_get_timestamp(msg);
	image_available->img_seq = message_image_available_get_img_seq(msg);
	image_available->img_buf_index = message_image_available_get_img_buf_index(msg);
	image_available->width = message_image_available_get_width(msg);
	image_available->height = message_image_available_get_height(msg);
	image_available->depth = message_image_available_get_depth(msg);
	image_available->channels = message_image_available_get_channels(msg);
	image_available->key = message_image_available_get_key(msg);
	image_available->exposure = message_image_available_get_exposure(msg);
	image_available->gain = message_image_available_get_gain(msg);
}
