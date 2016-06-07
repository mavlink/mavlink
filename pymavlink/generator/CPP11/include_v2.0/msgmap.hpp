
#pragma once

#include <endian.h>
#include <type_traits>

namespace mavlink {

class MsgMap {
public:

	explicit MsgMap(mavlink_message_t *p) :
		msg(p), cmsg(p), pos(0)
	{ }

	explicit MsgMap(mavlink_message_t &p) :
		msg(&p), cmsg(&p), pos(0)
	{ }

	explicit MsgMap(const mavlink_message_t *p) :
		msg(nullptr), cmsg(p), pos(0)
	{ }

	inline void reset()
	{
		pos = 0;
	}

	inline void reset(uint32_t msgid)
	{
		assert(msg);

		msg->msgid = msgid;
		pos = 0;
	}

	template<typename _T>
	void operator<< (const _T data);

	template<class _T, size_t _Size>
	void operator<< (const std::array<_T, _Size> &data);

	template<typename _T>
	void operator>> (_T &data);

	template<class _T, size_t _Size>
	void operator>> (std::array<_T, _Size> &data);

private:
	mavlink_message_t *msg;		// for serialization
	const mavlink_message_t *cmsg;	// for deserialization
	size_t pos;
};

} // namespace mavlink

// implementation

template<typename _T>
void mavlink::MsgMap::operator<< (const _T data)
{
	assert(msg);
	assert(pos + sizeof(_T) <= MAVLINK_MAX_PAYLOAD_LEN);

	switch (sizeof(_T)) {
	case 1:
		_MAV_PAYLOAD_NON_CONST(msg)[pos] = data;
		break;

	case 2:
		uint16_t data_le16;
		if (std::is_floating_point<_T>::value)
			data_le16 = htole16(*static_cast<const uint16_t *>(static_cast<const void *>(&data)));
		else
			data_le16 = htole16(data);

		memcpy(&_MAV_PAYLOAD_NON_CONST(msg)[pos], &data_le16, sizeof(data_le16));
		break;

	case 4:
		uint32_t data_le32;
		if (std::is_floating_point<_T>::value)
			data_le32 = htole32(*static_cast<const uint32_t *>(static_cast<const void *>(&data)));
		else
			data_le32 = htole32(data);

		memcpy(&_MAV_PAYLOAD_NON_CONST(msg)[pos], &data_le32, sizeof(data_le32));
		break;

	case 8:
		uint64_t data_le64;
		if (std::is_floating_point<_T>::value)
			data_le64 = htole64(*static_cast<const uint64_t *>(static_cast<const void *>(&data)));
		else
			data_le64 = htole64(data);

		memcpy(&_MAV_PAYLOAD_NON_CONST(msg)[pos], &data_le64, sizeof(data_le64));
		break;

	default:
		assert(false);
	}

	//std::cout << "M< s: " << sizeof(_T) << " p: " << pos << " d: " << data << std::endl;

	pos += sizeof(_T);
}

template<class _T, size_t _Size>
void mavlink::MsgMap::operator<< (const std::array<_T, _Size> &data)
{
	for (auto &v : data)
		*this << v;
}

template<typename _T>
void mavlink::MsgMap::operator>> (_T &data)
{
	assert(cmsg);
	assert(pos + sizeof(_T) <= MAVLINK_MAX_PAYLOAD_LEN);

	switch (sizeof(_T)) {
	case 1:
		data = _MAV_PAYLOAD(msg)[pos];
		break;

	case 2:
		uint16_t data_le16;
		memcpy(&data_le16, &_MAV_PAYLOAD(cmsg)[pos], sizeof(data_le16));
		data_le16 = le16toh(data_le16);

		if (std::is_floating_point<_T>::value)
			data = *static_cast<_T *>(static_cast<void *>(&data_le16));
		else
			data = data_le16;

		break;

	case 4:
		uint32_t data_le32;
		memcpy(&data_le32, &_MAV_PAYLOAD(cmsg)[pos], sizeof(data_le32));
		data_le32 = le32toh(data_le32);

		if (std::is_floating_point<_T>::value)
			data = *static_cast<_T *>(static_cast<void *>(&data_le32));
		else
			data = data_le32;

		break;

	case 8:
		uint64_t data_le64;
		memcpy(&data_le64, &_MAV_PAYLOAD(cmsg)[pos], sizeof(data_le64));
		data_le64 = le64toh(data_le64);

		if (std::is_floating_point<_T>::value)
			data = *static_cast<_T *>(static_cast<void *>(&data_le64));
		else
			data = data_le64;

		break;

	default:
		assert(false);
	}

	//std::cout << "M> s: " << sizeof(_T) << " p: " << pos << " d: " << data << std::endl;

	pos += sizeof(_T);
}

template<class _T, size_t _Size>
void mavlink::MsgMap::operator>> (std::array<_T, _Size> &data)
{
	for (auto &v : data)
		*this >> v;
}

