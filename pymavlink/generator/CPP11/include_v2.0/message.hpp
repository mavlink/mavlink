
#pragma once

#include <array>
#include <string>
#include <iostream>

#include "msgmap.hpp"

namespace mavlink {

struct Message {
	static constexpr uint32_t MSG_ID = UINT32_MAX;
	static constexpr size_t LENGTH = 0;
	static constexpr size_t MIN_LENGTH = 0;
	static constexpr uint8_t CRC_EXTRA = 0;
	static constexpr auto NAME = "BASE";

	virtual std::string to_yaml(void);
	virtual void serialize(MsgMap &map);
	virtual void deserialize(MsgMap &msp);
};

} // namespace mavlink
