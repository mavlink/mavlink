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



static void mavlink_test_px4_shift(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{

}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PX4_SHIFT_TESTSUITE_H
