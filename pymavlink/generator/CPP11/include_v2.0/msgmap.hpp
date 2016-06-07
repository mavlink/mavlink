
#pragma once

namespace mavlink {

class MsgMap {
public:
	inline void reset(uint32_t msgid)
	{
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
	mavlink_message_t *msg;
	size_t pos;
};

} // namespace mavlink

// implementation

template<typename _T>
void mavlink::MsgMap::operator<< (const _T data)
{
	std::cout << sizeof(_T) << " " << data << std::endl;


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

}

template<class _T, size_t _Size>
void mavlink::MsgMap::operator>> (std::array<_T, _Size> &data)
{

}

