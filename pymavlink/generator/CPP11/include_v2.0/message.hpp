
#pragma once

#include <array>
#include <cassert>
#include <string>
#include <iostream>

#define MAVLINK_USE_CXX_NAMESPACE	// put C-lib into namespace
#include "mavlink_types.h"

#define  _MAVLINK_CONVERSIONS_H_	// do not include mavlink_conversions.h
#define MAVLINK_GET_MSG_ENTRY		// user should provide mavlink_get_msg_entry()
namespace mavlink {
const mavlink_msg_entry_t *mavlink_get_msg_entry(uint32_t msgid);
} // namespace mavlink

#include "mavlink_helpers.h"

#include "msgmap.hpp"

namespace mavlink {

struct Message {
	static constexpr uint32_t MSG_ID = UINT32_MAX;
	static constexpr size_t LENGTH = 0;
	static constexpr size_t MIN_LENGTH = 0;
	static constexpr uint8_t CRC_EXTRA = 0;
	static constexpr auto NAME = "BASE";

	virtual std::string to_yaml(void) = 0;
	virtual void serialize(MsgMap &map) = 0;
	virtual void deserialize(MsgMap &msp) = 0;
};

} // namespace mavlink
