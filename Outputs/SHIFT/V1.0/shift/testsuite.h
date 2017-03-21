/** @file
 *    @brief MAVLink comm protocol testsuite generated from shift.xml
 *    @see http://qgroundcontrol.org/mavlink/
 */
#pragma once
#ifndef SHIFT_TESTSUITE_H
#define SHIFT_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_pixhawk(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_shift(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_pixhawk(system_id, component_id, last_msg);
    mavlink_test_shift(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"
#include "../pixhawk/testsuite.h"


static void mavlink_test_mav_shift_radiodog_identification_msg(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_mav_shift_radiodog_identification_msg_t packet_in = {
        963497464,963497672,963497880,963498088,18067,187,254
    };
    mavlink_mav_shift_radiodog_identification_msg_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.moduleId = packet_in.moduleId;
        packet1.currentXposition = packet_in.currentXposition;
        packet1.currentYposition = packet_in.currentYposition;
        packet1.currentZposition = packet_in.currentZposition;
        packet1.radioId = packet_in.radioId;
        packet1.moduleType = packet_in.moduleType;
        packet1.moduleIsActive = packet_in.moduleIsActive;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_IDENTIFICATION_MSG_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_identification_msg_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_mav_shift_radiodog_identification_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_identification_msg_pack(system_id, component_id, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.moduleIsActive , packet1.currentXposition , packet1.currentYposition , packet1.currentZposition );
    mavlink_msg_mav_shift_radiodog_identification_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_identification_msg_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.moduleIsActive , packet1.currentXposition , packet1.currentYposition , packet1.currentZposition );
    mavlink_msg_mav_shift_radiodog_identification_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_mav_shift_radiodog_identification_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_identification_msg_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.moduleIsActive , packet1.currentXposition , packet1.currentYposition , packet1.currentZposition );
    mavlink_msg_mav_shift_radiodog_identification_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mav_shift_radiodog_ranging_request_msg(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_mav_shift_radiodog_ranging_request_msg_t packet_in = {
        963497464,17443,151
    };
    mavlink_mav_shift_radiodog_ranging_request_msg_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.moduleId = packet_in.moduleId;
        packet1.radioId = packet_in.radioId;
        packet1.moduleType = packet_in.moduleType;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_MSG_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_pack(system_id, component_id, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType );
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType );
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.radioId , packet1.moduleType );
    mavlink_msg_mav_shift_radiodog_ranging_request_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mav_shift_radiodog_ranging_request_ack_msg(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_ACK_MSG >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_mav_shift_radiodog_ranging_request_ack_msg_t packet_in = {
        963497464,17443,151
    };
    mavlink_mav_shift_radiodog_ranging_request_ack_msg_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.moduleId = packet_in.moduleId;
        packet1.radioId = packet_in.radioId;
        packet1.moduleType = packet_in.moduleType;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_ACK_MSG_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_ACK_MSG_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_ack_msg_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_mav_shift_radiodog_ranging_request_ack_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_ack_msg_pack(system_id, component_id, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType );
    mavlink_msg_mav_shift_radiodog_ranging_request_ack_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_ack_msg_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType );
    mavlink_msg_mav_shift_radiodog_ranging_request_ack_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_mav_shift_radiodog_ranging_request_ack_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_ack_msg_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.radioId , packet1.moduleType );
    mavlink_msg_mav_shift_radiodog_ranging_request_ack_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mav_shift_radiodog_ranging_request_data_msg(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_mav_shift_radiodog_ranging_request_data_msg_t packet_in = {
        93372036854775807ULL,93372036854776311ULL,963498296,18275,199
    };
    mavlink_mav_shift_radiodog_ranging_request_data_msg_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.t1 = packet_in.t1;
        packet1.t2 = packet_in.t2;
        packet1.moduleId = packet_in.moduleId;
        packet1.radioId = packet_in.radioId;
        packet1.moduleType = packet_in.moduleType;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_RANGING_REQUEST_DATA_MSG_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_pack(system_id, component_id, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.t1 , packet1.t2 );
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.t1 , packet1.t2 );
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.t1 , packet1.t2 );
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mav_shift_radiodog_module_cfg_msg(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_mav_shift_radiodog_module_cfg_msg_t packet_in = {
        963497464,963497672,963497880,963498088,18067,187
    };
    mavlink_mav_shift_radiodog_module_cfg_msg_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.moduleId = packet_in.moduleId;
        packet1.newXposition = packet_in.newXposition;
        packet1.newYposition = packet_in.newYposition;
        packet1.newZposition = packet_in.newZposition;
        packet1.newRadioId = packet_in.newRadioId;
        packet1.newModuleType = packet_in.newModuleType;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_MODULE_CFG_MSG_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_pack(system_id, component_id, &msg , packet1.moduleId , packet1.newRadioId , packet1.newModuleType , packet1.newXposition , packet1.newYposition , packet1.newZposition );
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.newRadioId , packet1.newModuleType , packet1.newXposition , packet1.newYposition , packet1.newZposition );
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.newRadioId , packet1.newModuleType , packet1.newXposition , packet1.newYposition , packet1.newZposition );
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mav_shift_radiodog_px4_position_small(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_mav_shift_radiodog_px4_position_small_t packet_in = {
        963497464,963497672,963497880,963498088,963498296,18275,199
    };
    mavlink_mav_shift_radiodog_px4_position_small_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.moduleId = packet_in.moduleId;
        packet1.positionTimeStamp = packet_in.positionTimeStamp;
        packet1.Xposition = packet_in.Xposition;
        packet1.Yposition = packet_in.Yposition;
        packet1.Zposition = packet_in.Zposition;
        packet1.newRadioId = packet_in.newRadioId;
        packet1.newModuleType = packet_in.newModuleType;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_PX4_POSITION_SMALL_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_px4_position_small_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_mav_shift_radiodog_px4_position_small_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_px4_position_small_pack(system_id, component_id, &msg , packet1.moduleId , packet1.newRadioId , packet1.newModuleType , packet1.positionTimeStamp , packet1.Xposition , packet1.Yposition , packet1.Zposition );
    mavlink_msg_mav_shift_radiodog_px4_position_small_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_px4_position_small_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.newRadioId , packet1.newModuleType , packet1.positionTimeStamp , packet1.Xposition , packet1.Yposition , packet1.Zposition );
    mavlink_msg_mav_shift_radiodog_px4_position_small_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_mav_shift_radiodog_px4_position_small_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_px4_position_small_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.newRadioId , packet1.newModuleType , packet1.positionTimeStamp , packet1.Xposition , packet1.Yposition , packet1.Zposition );
    mavlink_msg_mav_shift_radiodog_px4_position_small_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_shift(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_mav_shift_radiodog_identification_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_ranging_request_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_ranging_request_ack_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_ranging_request_data_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_module_cfg_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_px4_position_small(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SHIFT_TESTSUITE_H
