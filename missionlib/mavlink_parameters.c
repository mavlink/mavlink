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

#include "mavlink_parameters.h"

extern mavlink_system_t mavlink_system;
extern mavlink_pm_storage pm;

extern void mavlink_missionlib_send_message(mavlink_message_t* msg);
extern void mavlink_missionlib_send_gcs_string(const char* string);


void mavlink_pm_init(mavlink_pm_storage* state)
{
	// Set emit counter to 0
	state->next_param = MAVLINK_PM_MAX_PARAM_COUNT;
	for (int i = 0; i < MAVLINK_PM_MAX_PARAM_COUNT; ++i)
	{
		// Write an empty name as init value
		state->param_names[i][0] = '\0';
	}
}

void mavlink_pm_message_handler(const mavlink_channel_t chan, const mavlink_message_t* msg)
{
//	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
//	uint32_t len;
//	switch (chan)
//	{
//		case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
//		{
//			// Start sending parameters
//			m_parameter_i = 0;
//		}
//			break;
//		case MAVLINK_MSG_ID_PARAM_SET:
//		{
//			mavlink_param_set_t set;
//			mavlink_msg_param_set_decode(msg, &set);
//			
//			// Check if this message is for this system
//			if (set.target_system == mavlink_system.sysid && set.target_component == mavlink_system.compid)
//			{
//				char* key = set.param_id;
//				
//				for (uint16_t i = 0; i < MAVLINK_PM_MAX_PARAM_NAME_LEN; i++)
//				{
//					bool match = true;
//					for (uint16_t j = 0; j < MAVLINK_PM_MAX_PARAM_NAME_LEN; j++)
//					{
//						// Compare
//						if (pm.param_name[i][j] != key[j])
//						{
//							match = false;
//						}
//						
//						// End matching if null termination is reached
//						if (pm.param_name[i][j] == '\0')
//						{
//							break;
//						}
//					}
//					
//					// Check if matched
//					if (match)
//					{
//						// Only write and emit changes if there is actually a difference
//						// AND only write if new value is NOT "not-a-number"
//						// AND is NOT infinity
//						if (global_data.param[i] != set.param_value
//							&& !isnan(set.param_value)
//							&& !isinf(set.param_value))
//						{
//							global_data.param[i] = set.param_value;
//							// Report back new value
//							mavlink_msg_param_value_send(MAVLINK_COMM_0,
//														 (int8_t*) global_data.param_name[i],
//														 global_data.param[i]);
//							mavlink_msg_param_value_send(MAVLINK_COMM_1,
//														 (int8_t*) global_data.param_name[i],
//														 global_data.param[i]);
//							
//							mavlink_missionlib_send_gcs_string("Parameter received param");
//						}
//					}
//				}
//			}
//		}
//			break;
	}
	
	/**
	 * @brief Send low-priority messages at a maximum rate of xx Hertz
	 *
	 * This function sends messages at a lower rate to not exceed the wireless
	 * bandwidth. It sends one message each time it is called until the buffer is empty.
	 * Call this function with xx Hertz to increase/decrease the bandwidth.
	 */
	void mavlink_pm_queued_send(void)
	{
		//send parameters one by one
//		if (m_parameter_i < MAVLINK_PM_MAX_PARAM_COUNT)
//		{
//			for (int i.. all active comm links)
//			mavlink_msg_param_value_send(MAVLINK_COMM_0,
//										 (int8_t*) global_data.param_name[m_parameter_i],
//										 global_data.param[m_parameter_i]);
//			mavlink_msg_param_value_send(MAVLINK_COMM_1,
//										 (int8_t*) global_data.param_name[m_parameter_i],
//										 global_data.param[m_parameter_i]);
//			m_parameter_i++;
//		}
	}