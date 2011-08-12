/*******************************************************************************
 
 Copyright (C) 2011 Lorenz Meier lm ( a t ) inf.ethz.ch
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ****************************************************************************/

/* This assumes you have the mavlink headers on your include path
 or in the same folder as this source file */

// Disable auto-data structures
#define MAVLINK_NO_DATA

#include "mavlink.h"
#include <stdbool.h>

/* WAYPOINT MANAGER - MISSION LIB */

#ifndef MAVLINK_PM_MAX_PARAM_COUNT
#define MAVLINK_PM_MAX_PARAM_COUNT 20
#endif

#ifndef MAVLINK_PM_TEXT_FEEDBACK
#define MAVLINK_PM_TEXT_FEEDBACK 1						  ///< Report back status information as text
#endif

#define MAVLINK_PM_MAX_PARAM_NAME_LEN 16

struct mavlink_pm_storage {
	char param_names[MAVLINK_PM_MAX_PARAM_COUNT][MAVLINK_PM_MAX_PARAM_NAME_LEN];      ///< Parameter names
	float param_values[MAVLINK_PM_MAX_PARAM_COUNT];    ///< Parameter values
	uint16_t next_param;
};

typedef struct mavlink_pm_storage mavlink_pm_storage;

void mavlink_pm_init(mavlink_pm_storage* state);

void mavlink_pm_message_handler(const mavlink_channel_t chan, const mavlink_message_t* msg);