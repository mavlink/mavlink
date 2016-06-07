
#pragma once

#include <array>

namespace mavlink {

class MsgMap {
public:
	inline void reset_header(uint32_t msgid)
	{

	}

	template<typename _T>
	void operator<< (const _T data);

	template<class _T, size_t _Size>
	void operator<< (const std::array<_T, _Size> &data);

	template<typename _T>
	void operator>> (_T &data);

	template<class _T, size_t _Size>
	void operator>> (std::array<_T, _Size> &data);
};

} // namespace mavlink

// implementation

template<typename _T>
void mavlink::MsgMap::operator<< (const _T data)
{
	// TODO
}

template<class _T, size_t _Size>
void mavlink::MsgMap::operator<< (const std::array<_T, _Size> &data)
{

}

template<typename _T>
void mavlink::MsgMap::operator>> (_T &data)
{

}

template<class _T, size_t _Size>
void mavlink::MsgMap::operator>> (std::array<_T, _Size> &data)
{

}

