/** @file
 *  @brief MAVLink comm protocol generated from pixhawk.xml
 *  @see http://mavlink.org
 */
#pragma once
#ifndef MAVLINK_PIXHAWK_H
#define MAVLINK_PIXHAWK_H

#ifndef MAVLINK_H
    #error Wrong include order: MAVLINK_PIXHAWK.H MUST NOT BE DIRECTLY USED. Include mavlink.h from the same directory instead or set ALL AND EVERY defines from MAVLINK.H manually accordingly, including the #define MAVLINK_H call.
#endif

#undef MAVLINK_THIS_XML_IDX
#define MAVLINK_THIS_XML_IDX 2

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#ifndef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {}
#endif

#ifndef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {{151, 108, 11, 0, 0, 0}, {152, 86, 52, 0, 0, 0}, {153, 95, 1, 0, 0, 0}, {154, 224, 92, 0, 0, 0}, {160, 22, 18, 3, 16, 17}, {170, 28, 18, 0, 0, 0}, {171, 249, 26, 0, 0, 0}, {172, 182, 16, 0, 0, 0}, {180, 153, 4, 0, 0, 0}, {181, 16, 255, 0, 0, 0}, {182, 29, 12, 0, 0, 0}, {183, 162, 6, 0, 0, 0}, {190, 90, 106, 0, 0, 0}, {191, 95, 43, 0, 0, 0}, {192, 36, 55, 0, 0, 0}, {195, 88, 53, 0, 0, 0}, {200, 254, 21, 0, 0, 0}, {205, 87, 48, 0, 0, 0}, {206, 19, 39, 0, 0, 0}}
#endif

#include "../protocol.h"

#define MAVLINK_ENABLED_PIXHAWK

// ENUM DEFINITIONS


/** @brief Content Types for data transmission handshake */
#ifndef HAVE_ENUM_DATA_TYPES
#define HAVE_ENUM_DATA_TYPES
typedef enum DATA_TYPES
{
   DATA_TYPE_JPEG_IMAGE=1, /*  | */
   DATA_TYPE_RAW_IMAGE=2, /*  | */
   DATA_TYPE_KINECT=3, /*  | */
   DATA_TYPES_ENUM_END=4, /*  | */
} DATA_TYPES;
#endif

// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

// MESSAGE DEFINITIONS
#include "./mavlink_msg_set_cam_shutter.h"
#include "./mavlink_msg_image_triggered.h"
#include "./mavlink_msg_image_trigger_control.h"
#include "./mavlink_msg_image_available.h"
#include "./mavlink_msg_set_position_control_offset.h"
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
#include "./mavlink_msg_brief_feature.h"
#include "./mavlink_msg_attitude_control.h"
#include "./mavlink_msg_detection_stats.h"
#include "./mavlink_msg_onboard_health.h"

// base include
#include "../common/common.h"

#undef MAVLINK_THIS_XML_IDX
#define MAVLINK_THIS_XML_IDX 2

#if MAVLINK_THIS_XML_IDX == MAVLINK_PRIMARY_XML_IDX
# define MAVLINK_MESSAGE_INFO {MAVLINK_MESSAGE_INFO_SET_CAM_SHUTTER, MAVLINK_MESSAGE_INFO_IMAGE_TRIGGERED, MAVLINK_MESSAGE_INFO_IMAGE_TRIGGER_CONTROL, MAVLINK_MESSAGE_INFO_IMAGE_AVAILABLE, MAVLINK_MESSAGE_INFO_SET_POSITION_CONTROL_OFFSET, MAVLINK_MESSAGE_INFO_POSITION_CONTROL_SETPOINT, MAVLINK_MESSAGE_INFO_MARKER, MAVLINK_MESSAGE_INFO_RAW_AUX, MAVLINK_MESSAGE_INFO_WATCHDOG_HEARTBEAT, MAVLINK_MESSAGE_INFO_WATCHDOG_PROCESS_INFO, MAVLINK_MESSAGE_INFO_WATCHDOG_PROCESS_STATUS, MAVLINK_MESSAGE_INFO_WATCHDOG_COMMAND, MAVLINK_MESSAGE_INFO_PATTERN_DETECTED, MAVLINK_MESSAGE_INFO_POINT_OF_INTEREST, MAVLINK_MESSAGE_INFO_POINT_OF_INTEREST_CONNECTION, MAVLINK_MESSAGE_INFO_BRIEF_FEATURE, MAVLINK_MESSAGE_INFO_ATTITUDE_CONTROL, MAVLINK_MESSAGE_INFO_DETECTION_STATS, MAVLINK_MESSAGE_INFO_ONBOARD_HEALTH}
# if MAVLINK_COMMAND_24BIT
#  include "../mavlink_get_info.h"
# endif
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAVLINK_PIXHAWK_H
