/** @file
 *	@brief MAVLink comm protocol.
 *	@see http://pixhawk.ethz.ch/software/mavlink
 *	 Generated on Thursday, September 16 2010, 16:06 UTC
 */
#ifndef MAVLINK_H
#define MAVLINK_H

#ifdef __cplusplus
extern "C" {
#endif


#include "protocol.h"

#define MAVLINK_ENABLED_UALBERTA_MESSAGES

#include "generated/mavlink_msg_heartbeat.h"
#include "generated/mavlink_msg_boot.h"
#include "generated/mavlink_msg_system_time.h"
#include "generated/mavlink_msg_ping.h"
#include "generated/mavlink_msg_action.h"
#include "generated/mavlink_msg_action_ack.h"
#include "generated/mavlink_msg_set_mode.h"
#include "generated/mavlink_msg_set_nav_mode.h"
#include "generated/mavlink_msg_param_request_read.h"
#include "generated/mavlink_msg_param_request_list.h"
#include "generated/mavlink_msg_param_value.h"
#include "generated/mavlink_msg_param_set.h"
#include "generated/mavlink_msg_raw_imu.h"
#include "generated/mavlink_msg_raw_pressure.h"
#include "generated/mavlink_msg_attitude.h"
#include "generated/mavlink_msg_local_position.h"
#include "generated/mavlink_msg_gps_raw.h"
#include "generated/mavlink_msg_gps_status.h"
#include "generated/mavlink_msg_global_position.h"
#include "generated/mavlink_msg_sys_status.h"
#include "generated/mavlink_msg_rc_channels.h"
#include "generated/mavlink_msg_waypoint.h"
#include "generated/mavlink_msg_waypoint_request.h"
#include "generated/mavlink_msg_waypoint_set_current.h"
#include "generated/mavlink_msg_waypoint_current.h"
#include "generated/mavlink_msg_waypoint_request_list.h"
#include "generated/mavlink_msg_waypoint_count.h"
#include "generated/mavlink_msg_waypoint_clear_all.h"
#include "generated/mavlink_msg_waypoint_reached.h"
#include "generated/mavlink_msg_waypoint_ack.h"
#include "generated/mavlink_msg_waypoint_set_global_reference.h"
#include "generated/mavlink_msg_local_position_setpoint_set.h"
#include "generated/mavlink_msg_local_position_setpoint.h"
#include "generated/mavlink_msg_attitude_controller_output.h"
#include "generated/mavlink_msg_position_controller_output.h"
#include "generated/mavlink_msg_statustext.h"
#include "generated/mavlink_msg_debug.h"
#include "generated/mavlink_msg_nav_filter_bias.h"
#include "generated/mavlink_msg_request_rc_channels.h"
#ifdef __cplusplus
}
#endif
#endif
