/** @file
 *    @brief MAVLink comm protocol testsuite generated from px4_shift.xml
 *    @see http://qgroundcontrol.org/mavlink/
 */
#pragma once
#ifndef PX4_SHIFT_TESTSUITE_H
#define PX4_SHIFT_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_px4_shift(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_px4_shift(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"


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
        93372036854775807ULL,93372036854776311ULL,93372036854776815ULL,963498712,18691,223
    };
    mavlink_mav_shift_radiodog_ranging_request_data_msg_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.t1 = packet_in.t1;
        packet1.t2 = packet_in.t2;
        packet1.t5 = packet_in.t5;
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
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_pack(system_id, component_id, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.t1 , packet1.t2 , packet1.t5 );
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.t1 , packet1.t2 , packet1.t5 );
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
    mavlink_msg_mav_shift_radiodog_ranging_request_data_msg_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.t1 , packet1.t2 , packet1.t5 );
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
        963497464,963497672,963497880,963498088,18067,187,254
    };
    mavlink_mav_shift_radiodog_module_cfg_msg_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.moduleId = packet_in.moduleId;
        packet1.newXposition = packet_in.newXposition;
        packet1.newYposition = packet_in.newYposition;
        packet1.newZposition = packet_in.newZposition;
        packet1.newRadioId = packet_in.newRadioId;
        packet1.newModuleType = packet_in.newModuleType;
        packet1.storeInEeprom = packet_in.storeInEeprom;
        
        
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
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_pack(system_id, component_id, &msg , packet1.moduleId , packet1.newRadioId , packet1.newModuleType , packet1.newXposition , packet1.newYposition , packet1.newZposition , packet1.storeInEeprom );
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.newRadioId , packet1.newModuleType , packet1.newXposition , packet1.newYposition , packet1.newZposition , packet1.storeInEeprom );
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
    mavlink_msg_mav_shift_radiodog_module_cfg_msg_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.newRadioId , packet1.newModuleType , packet1.newXposition , packet1.newYposition , packet1.newZposition , packet1.storeInEeprom );
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
        packet1.radioId = packet_in.radioId;
        packet1.moduleType = packet_in.moduleType;
        
        
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
    mavlink_msg_mav_shift_radiodog_px4_position_small_pack(system_id, component_id, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.positionTimeStamp , packet1.Xposition , packet1.Yposition , packet1.Zposition );
    mavlink_msg_mav_shift_radiodog_px4_position_small_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_px4_position_small_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.positionTimeStamp , packet1.Xposition , packet1.Yposition , packet1.Zposition );
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
    mavlink_msg_mav_shift_radiodog_px4_position_small_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.positionTimeStamp , packet1.Xposition , packet1.Yposition , packet1.Zposition );
    mavlink_msg_mav_shift_radiodog_px4_position_small_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mav_shift_radiodog_debug_tag_info(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_mav_shift_radiodog_debug_tag_info_t packet_in = {
        963497464,963497672,963497880,963498088,963498296,963498504,963498712,963498920,18899,235
    };
    mavlink_mav_shift_radiodog_debug_tag_info_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.moduleId = packet_in.moduleId;
        packet1.positionTimeStamp = packet_in.positionTimeStamp;
        packet1.Anchor_A_Ranging_Raw = packet_in.Anchor_A_Ranging_Raw;
        packet1.Anchor_A_Ranging_Filtered = packet_in.Anchor_A_Ranging_Filtered;
        packet1.Anchor_B_Ranging_Raw = packet_in.Anchor_B_Ranging_Raw;
        packet1.Anchor_B_Ranging_Filtered = packet_in.Anchor_B_Ranging_Filtered;
        packet1.Anchor_C_Ranging_Raw = packet_in.Anchor_C_Ranging_Raw;
        packet1.Anchor_C_Ranging_Filtered = packet_in.Anchor_C_Ranging_Filtered;
        packet1.radioId = packet_in.radioId;
        packet1.moduleType = packet_in.moduleType;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_MAV_SHIFT_RADIODOG_DEBUG_TAG_INFO_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_debug_tag_info_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_mav_shift_radiodog_debug_tag_info_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_debug_tag_info_pack(system_id, component_id, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.positionTimeStamp , packet1.Anchor_A_Ranging_Raw , packet1.Anchor_A_Ranging_Filtered , packet1.Anchor_B_Ranging_Raw , packet1.Anchor_B_Ranging_Filtered , packet1.Anchor_C_Ranging_Raw , packet1.Anchor_C_Ranging_Filtered );
    mavlink_msg_mav_shift_radiodog_debug_tag_info_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_debug_tag_info_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.positionTimeStamp , packet1.Anchor_A_Ranging_Raw , packet1.Anchor_A_Ranging_Filtered , packet1.Anchor_B_Ranging_Raw , packet1.Anchor_B_Ranging_Filtered , packet1.Anchor_C_Ranging_Raw , packet1.Anchor_C_Ranging_Filtered );
    mavlink_msg_mav_shift_radiodog_debug_tag_info_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_mav_shift_radiodog_debug_tag_info_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_mav_shift_radiodog_debug_tag_info_send(MAVLINK_COMM_1 , packet1.moduleId , packet1.radioId , packet1.moduleType , packet1.positionTimeStamp , packet1.Anchor_A_Ranging_Raw , packet1.Anchor_A_Ranging_Filtered , packet1.Anchor_B_Ranging_Raw , packet1.Anchor_B_Ranging_Filtered , packet1.Anchor_C_Ranging_Raw , packet1.Anchor_C_Ranging_Filtered );
    mavlink_msg_mav_shift_radiodog_debug_tag_info_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_shift_navigator_cmd(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_shift_navigator_cmd_t packet_in = {
        93372036854775807ULL,963497880,41,108
    };
    mavlink_shift_navigator_cmd_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_usec = packet_in.time_usec;
        packet1.pathIndex = packet_in.pathIndex;
        packet1.target_system = packet_in.target_system;
        packet1.cmd = packet_in.cmd;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SHIFT_NAVIGATOR_CMD_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_cmd_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_shift_navigator_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_cmd_pack(system_id, component_id, &msg , packet1.target_system , packet1.time_usec , packet1.pathIndex , packet1.cmd );
    mavlink_msg_shift_navigator_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_cmd_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.time_usec , packet1.pathIndex , packet1.cmd );
    mavlink_msg_shift_navigator_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_shift_navigator_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_cmd_send(MAVLINK_COMM_1 , packet1.target_system , packet1.time_usec , packet1.pathIndex , packet1.cmd );
    mavlink_msg_shift_navigator_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_shift_att_pos(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SHIFT_ATT_POS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_shift_att_pos_t packet_in = {
        93372036854775807ULL,{ 73.0, 74.0, 75.0, 76.0 },185.0,213.0,241.0,113
    };
    mavlink_shift_att_pos_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_usec = packet_in.time_usec;
        packet1.x = packet_in.x;
        packet1.y = packet_in.y;
        packet1.z = packet_in.z;
        packet1.target_system = packet_in.target_system;
        
        mav_array_memcpy(packet1.q, packet_in.q, sizeof(float)*4);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SHIFT_ATT_POS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_att_pos_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_shift_att_pos_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_att_pos_pack(system_id, component_id, &msg , packet1.target_system , packet1.time_usec , packet1.q , packet1.x , packet1.y , packet1.z );
    mavlink_msg_shift_att_pos_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_att_pos_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.time_usec , packet1.q , packet1.x , packet1.y , packet1.z );
    mavlink_msg_shift_att_pos_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_shift_att_pos_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_att_pos_send(MAVLINK_COMM_1 , packet1.target_system , packet1.time_usec , packet1.q , packet1.x , packet1.y , packet1.z );
    mavlink_msg_shift_att_pos_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_shift_navigator_point(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_shift_navigator_point_t packet_in = {
        17.0,45.0,73.0,101.0,129.0,157.0,185.0,18691,18795,101,168,235
    };
    mavlink_shift_navigator_point_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.positionX = packet_in.positionX;
        packet1.positionY = packet_in.positionY;
        packet1.positionZ = packet_in.positionZ;
        packet1.positionRoll = packet_in.positionRoll;
        packet1.positionPitch = packet_in.positionPitch;
        packet1.positionYaw = packet_in.positionYaw;
        packet1.duration = packet_in.duration;
        packet1.pathIndex = packet_in.pathIndex;
        packet1.pointIndex = packet_in.pointIndex;
        packet1.target_system = packet_in.target_system;
        packet1.yawBehaviour = packet_in.yawBehaviour;
        packet1.type = packet_in.type;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SHIFT_NAVIGATOR_POINT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_point_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_shift_navigator_point_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_point_pack(system_id, component_id, &msg , packet1.target_system , packet1.pathIndex , packet1.pointIndex , packet1.positionX , packet1.positionY , packet1.positionZ , packet1.positionRoll , packet1.positionPitch , packet1.positionYaw , packet1.duration , packet1.yawBehaviour , packet1.type );
    mavlink_msg_shift_navigator_point_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_point_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.pathIndex , packet1.pointIndex , packet1.positionX , packet1.positionY , packet1.positionZ , packet1.positionRoll , packet1.positionPitch , packet1.positionYaw , packet1.duration , packet1.yawBehaviour , packet1.type );
    mavlink_msg_shift_navigator_point_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_shift_navigator_point_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_point_send(MAVLINK_COMM_1 , packet1.target_system , packet1.pathIndex , packet1.pointIndex , packet1.positionX , packet1.positionY , packet1.positionZ , packet1.positionRoll , packet1.positionPitch , packet1.positionYaw , packet1.duration , packet1.yawBehaviour , packet1.type );
    mavlink_msg_shift_navigator_point_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_shift_navigator_sim_point(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_shift_navigator_sim_point_t packet_in = {
        93372036854775807ULL,73.0,101.0,129.0,157.0,185.0,213.0
    };
    mavlink_shift_navigator_sim_point_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_usec = packet_in.time_usec;
        packet1.positionX = packet_in.positionX;
        packet1.positionY = packet_in.positionY;
        packet1.positionZ = packet_in.positionZ;
        packet1.positionRoll = packet_in.positionRoll;
        packet1.positionPitch = packet_in.positionPitch;
        packet1.positionYaw = packet_in.positionYaw;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SHIFT_NAVIGATOR_SIM_POINT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_sim_point_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_shift_navigator_sim_point_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_sim_point_pack(system_id, component_id, &msg , packet1.time_usec , packet1.positionX , packet1.positionY , packet1.positionZ , packet1.positionRoll , packet1.positionPitch , packet1.positionYaw );
    mavlink_msg_shift_navigator_sim_point_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_sim_point_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_usec , packet1.positionX , packet1.positionY , packet1.positionZ , packet1.positionRoll , packet1.positionPitch , packet1.positionYaw );
    mavlink_msg_shift_navigator_sim_point_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_shift_navigator_sim_point_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_navigator_sim_point_send(MAVLINK_COMM_1 , packet1.time_usec , packet1.positionX , packet1.positionY , packet1.positionZ , packet1.positionRoll , packet1.positionPitch , packet1.positionYaw );
    mavlink_msg_shift_navigator_sim_point_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_shift_system_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_shift_system_status_t packet_in = {
        17235,17339,17,84,151,218,29
    };
    mavlink_shift_system_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.currentPathIndex = packet_in.currentPathIndex;
        packet1.currentPointIndex = packet_in.currentPointIndex;
        packet1.batteryRemaining = packet_in.batteryRemaining;
        packet1.navigatorMode = packet_in.navigatorMode;
        packet1.navigatorSubMode = packet_in.navigatorSubMode;
        packet1.objectMode = packet_in.objectMode;
        packet1.detailField = packet_in.detailField;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SHIFT_SYSTEM_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_system_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_shift_system_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_system_status_pack(system_id, component_id, &msg , packet1.currentPathIndex , packet1.currentPointIndex , packet1.batteryRemaining , packet1.navigatorMode , packet1.navigatorSubMode , packet1.objectMode , packet1.detailField );
    mavlink_msg_shift_system_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_system_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.currentPathIndex , packet1.currentPointIndex , packet1.batteryRemaining , packet1.navigatorMode , packet1.navigatorSubMode , packet1.objectMode , packet1.detailField );
    mavlink_msg_shift_system_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_shift_system_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_system_status_send(MAVLINK_COMM_1 , packet1.currentPathIndex , packet1.currentPointIndex , packet1.batteryRemaining , packet1.navigatorMode , packet1.navigatorSubMode , packet1.objectMode , packet1.detailField );
    mavlink_msg_shift_system_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_shift_system_cmd(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_shift_system_cmd_t packet_in = {
        17235,17339,17443,17547,17651,17755,41,108
    };
    mavlink_shift_system_cmd_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.cmd1 = packet_in.cmd1;
        packet1.cmd2 = packet_in.cmd2;
        packet1.cmd3 = packet_in.cmd3;
        packet1.cmd4 = packet_in.cmd4;
        packet1.cmd5 = packet_in.cmd5;
        packet1.cmd6 = packet_in.cmd6;
        packet1.target_system = packet_in.target_system;
        packet1.type = packet_in.type;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SHIFT_SYSTEM_CMD_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_system_cmd_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_shift_system_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_system_cmd_pack(system_id, component_id, &msg , packet1.target_system , packet1.cmd1 , packet1.cmd2 , packet1.cmd3 , packet1.cmd4 , packet1.cmd5 , packet1.cmd6 , packet1.type );
    mavlink_msg_shift_system_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_system_cmd_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.cmd1 , packet1.cmd2 , packet1.cmd3 , packet1.cmd4 , packet1.cmd5 , packet1.cmd6 , packet1.type );
    mavlink_msg_shift_system_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_shift_system_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shift_system_cmd_send(MAVLINK_COMM_1 , packet1.target_system , packet1.cmd1 , packet1.cmd2 , packet1.cmd3 , packet1.cmd4 , packet1.cmd5 , packet1.cmd6 , packet1.type );
    mavlink_msg_shift_system_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_shiftw_system_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_shiftw_system_status_t packet_in = {
        5,72,139
    };
    mavlink_shiftw_system_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.status = packet_in.status;
        packet1.serialNumber = packet_in.serialNumber;
        packet1.detailField = packet_in.detailField;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SHIFTW_SYSTEM_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shiftw_system_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_shiftw_system_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shiftw_system_status_pack(system_id, component_id, &msg , packet1.status , packet1.serialNumber , packet1.detailField );
    mavlink_msg_shiftw_system_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shiftw_system_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.status , packet1.serialNumber , packet1.detailField );
    mavlink_msg_shiftw_system_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_shiftw_system_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_shiftw_system_status_send(MAVLINK_COMM_1 , packet1.status , packet1.serialNumber , packet1.detailField );
    mavlink_msg_shiftw_system_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_px4_shift(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_mav_shift_radiodog_identification_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_ranging_request_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_ranging_request_ack_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_ranging_request_data_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_module_cfg_msg(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_px4_position_small(system_id, component_id, last_msg);
    mavlink_test_mav_shift_radiodog_debug_tag_info(system_id, component_id, last_msg);
    mavlink_test_shift_navigator_cmd(system_id, component_id, last_msg);
    mavlink_test_shift_att_pos(system_id, component_id, last_msg);
    mavlink_test_shift_navigator_point(system_id, component_id, last_msg);
    mavlink_test_shift_navigator_sim_point(system_id, component_id, last_msg);
    mavlink_test_shift_system_status(system_id, component_id, last_msg);
    mavlink_test_shift_system_cmd(system_id, component_id, last_msg);
    mavlink_test_shiftw_system_status(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PX4_SHIFT_TESTSUITE_H
