
#pragma once

#include <array>
#include <cassert>
#include <string>
#include <iostream>
#include <cstring>

#ifndef MAVLINK_HELPER
#define MAVLINK_HELPER static inline
#endif

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

//! Message ID type
using msgid_t = uint32_t;

/**
 * MAVLink Message base class.
 */
struct Message {
	static constexpr msgid_t MSG_ID = UINT32_MAX;
	static constexpr size_t LENGTH = 0;
	static constexpr size_t MIN_LENGTH = 0;
	static constexpr uint8_t CRC_EXTRA = 0;
	static constexpr auto NAME = "BASE";

	struct Info {
		msgid_t id;
		size_t length;
		size_t min_length;
		uint8_t crc_extra;
	};

	/**
	 * Get NAME constant. Helper for overloaded class access.
	 */
	virtual std::string get_name(void) const = 0;

	/**
	 * Get info needed for mavlink_finalize_message_xxx()
	 */
	virtual Info&& get_message_info(void) const = 0;

	/**
	 * Make YAML-string from message content.
	 */
	virtual std::string to_yaml(void) const = 0;

	/**
	 * Serialize message.
	 *
	 * @param[out] map
	 */
	virtual void serialize(MsgMap &map) const = 0;

	/**
	 * Deserialize message.
	 *
	 * @param[in] map
	 */
	virtual void deserialize(MsgMap &msp) = 0;
};

/**
 * Converts std::array<char, N> to std::string.
 *
 * Array treated as null-terminated string up to _N chars.
 */
template<size_t _N>
std::string to_string(const std::array<char, _N> &a)
{
	return std::string(a.data(), strnlen(a.data(), a.size()));
}

} // namespace mavlink
