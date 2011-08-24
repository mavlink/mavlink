/** @file
 *	@brief MAVLink comm protocol generated from pixhawk.xml
 *	@see http://qgroundcontrol.org/mavlink/
 *	Generated on Wed Aug 24 10:57:52 2011
 */
#ifndef PIXHAWK_H
#define PIXHAWK_H

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#undef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {8, 23, 12, 8, 14, 28, 3, 32, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 20, 2, 25, 23, 0, 30, 26, 101, 26, 16, 32, 32, 38, 32, 0, 17, 17, 16, 18, 36, 4, 4, 2, 2, 4, 2, 2, 3, 14, 12, 18, 16, 14, 27, 25, 18, 18, 20, 20, 0, 0, 0, 26, 16, 36, 0, 6, 4, 0, 21, 18, 0, 0, 20, 20, 20, 32, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 42, 33, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 52, 1, 92, 0, 32, 32, 20, 20, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 26, 16, 0, 0, 0, 0, 0, 0, 0, 4, 255, 12, 6, 0, 0, 0, 0, 0, 0, 106, 43, 55, 8, 255, 53, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 30, 14, 14, 51, 5}

#undef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {14, 233, 235, 160, 161, 5, 129, 181, 0, 0, 0, 204, 238, 0, 0, 0, 0, 0, 0, 0, 119, 16, 3, 27, 0, 210, 140, 88, 120, 233, 79, 2, 138, 220, 0, 1, 92, 223, 153, 3, 203, 110, 241, 68, 66, 232, 235, 46, 174, 180, 149, 40, 181, 91, 50, 252, 138, 1, 9, 0, 0, 0, 152, 67, 227, 0, 169, 126, 0, 116, 255, 0, 0, 54, 242, 21, 77, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 76, 62, 0, 0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 67, 253, 100, 132, 0, 36, 96, 45, 39, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 241, 159, 0, 0, 0, 0, 0, 0, 0, 91, 225, 79, 68, 0, 0, 0, 0, 0, 0, 4, 253, 103, 45, 184, 95, 0, 0, 0, 0, 221, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 174, 230, 12, 22, 55, 132}
    

#include "../protocol.h"

#define MAVLINK_ENABLED_PIXHAWK

#include "../common/common.h"


// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

// ENUM DEFINITIONS


/** @brief Content Types for data transmission handshake */
enum DATA_TYPES
{
	DATA_TYPE_JPEG_IMAGE=1, /*  | */
	DATA_TYPE_RAW_IMAGE=2, /*  | */
	DATA_TYPE_KINECT=3, /*  | */
};

// MESSAGE DEFINITIONS
#include "./mavlink_msg_set_cam_shutter.h"
#include "./mavlink_msg_image_triggered.h"
#include "./mavlink_msg_image_trigger_control.h"
#include "./mavlink_msg_image_available.h"
#include "./mavlink_msg_vision_position_estimate.h"
#include "./mavlink_msg_vicon_position_estimate.h"
#include "./mavlink_msg_vision_speed_estimate.h"
#include "./mavlink_msg_position_control_setpoint_set.h"
#include "./mavlink_msg_position_control_offset_set.h"
#include "./mavlink_msg_position_control_setpoint.h"
#include "./mavlink_msg_marker.h"
#include "./mavlink_msg_raw_aux.h"
#include "./mavlink_msg_watchdog_heartbeat.h"
#include "./mavlink_msg_watchdog_process_info.h"
#include "./mavlink_msg_watchdog_process_status.h"
#include "./mavlink_msg_watchdog_command.h"
#include "./mavlink_msg_pattern_detected.h"
#include "./mavlink_msg_point_of_interest.h"
#include "./mavlink_msg_point_of_interest_connection.h"
#include "./mavlink_msg_data_transmission_handshake.h"
#include "./mavlink_msg_encapsulated_data.h"
#include "./mavlink_msg_brief_feature.h"
#include "./mavlink_msg_attitude_control.h"

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PIXHAWK_H
