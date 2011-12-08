#include <deque>
#include <google/protobuf/message.h>
#include <iostream>
#include <map>
#include <tr1/memory>

#ifdef MAVLINK_MAX_PAYLOAD_LEN
#error MAVLINK_MAX_PAYLOAD_LEN has been defined.
#else
#define MAVLINK_MAX_PAYLOAD_LEN 65499
#endif

#include <checksum.h>
#include <common/mavlink.h>
#include <mavlink_types.h>
#include <pixhawk/pixhawk.pb.h>

namespace mavlink
{

class ProtobufFactory
{
public:
	ProtobufFactory(bool verbose = false)
	 : kHeaderLength(13)
	 , kPayloadLength(MAVLINK_MAX_PAYLOAD_LEN - kHeaderLength)
	 , mStreamID(0)
	 , mVerbose(verbose)
	{

	}

	static std::tr1::shared_ptr<ProtobufFactory>& instance(void)
	{
		if (mInstance.get() == 0)
		{
			srand(time(NULL));
			mInstance.reset(new ProtobufFactory);
		}
		return mInstance;
	}

	bool fragmentMessage(uint8_t system_id, uint8_t component_id,
						 uint8_t target_system, uint8_t target_component,
						 google::protobuf::Message& message,
						 std::vector<mavlink_message_t>& fragments)
	{
		if (mStreamID == 0)
		{
			mStreamID = rand() + 1;
		}

		std::string data;
		message.SerializeToString(&data);

		int fragmentCount = (message.ByteSize() + MAVLINK_MAX_PAYLOAD_LEN - 1) / MAVLINK_MAX_PAYLOAD_LEN;
		unsigned int offset = 0;
		for (int i = 0; i < fragmentCount; ++i)
		{
			mavlink_message_t fragment;

			fragment.magic = MAVLINK_STX;
			fragment.len = kHeaderLength;
			fragment.seq = mavlink_get_channel_status(MAVLINK_COMM_0)->current_tx_seq;
			++mavlink_get_channel_status(MAVLINK_COMM_0)->current_tx_seq;
			fragment.sysid = system_id;
			fragment.compid = component_id;
			fragment.msgid = MAVLINK_MSG_ID_EXTENDED_MESSAGE;

			char* payload = reinterpret_cast<char*>(fragment.payload64);

			unsigned int length = kPayloadLength;
			uint8_t flags = 0;
			if (i < fragmentCount - 1)
			{
				length = kPayloadLength;
				flags |= 0x1;
			}
			if (i == fragmentCount - 1)
			{
				length = message.ByteSize() - kPayloadLength * (fragmentCount - 1);
				flags |= 0x0;
			}

			// insert header
			memcpy(payload, &target_system, 1);
			memcpy(payload + 1, &target_component, 1);
			memcpy(payload + 2, &length, 4);
			memcpy(payload + 6, &mStreamID, 2);
			memcpy(payload + 8, &offset, 4);
			memcpy(payload + 12, &flags, 1);

			// insert data
			memcpy(payload + kHeaderLength, &data[offset], length);

			// calculate crc
			fragment.checksum = crc_calculate(reinterpret_cast<uint8_t*>(&data[offset]), length);

			offset += length;
		}

		if (mVerbose)
		{
			std::cerr << "# INFO: Split extended message into "
					  << fragmentCount << " fragments." << std::endl;
		}

		return true;
	}

	void addFragment(mavlink_message_t& msg)
	{
		if (!validFragment(msg))
		{
			if (mVerbose)
			{
				std::cerr << "# WARNING: Message is not a valid fragment. "
						  << "Dropping message..." << std::endl;
			}
			return;
		}

		// read header
		char* payload = reinterpret_cast<char*>(msg.payload64);
		unsigned int length = 0;
		unsigned short streamID = 0;
		unsigned int offset = 0;
		uint8_t flags = 0;

		memcpy(&length, payload + 2, 4);
		memcpy(&streamID, payload + 6, 2);
		memcpy(&offset, payload + 8, 4);
		memcpy(&flags, payload + 12, 1);

		bool reassemble = false;

		FragmentQueue::iterator it = mFragmentQueue.find(streamID);
		if (it == mFragmentQueue.end())
		{
			if (offset == 0)
			{
				mFragmentQueue[streamID].push_back(msg);

				if (flags && 0x1 != 0x1)
				{
					reassemble = true;
				}
			}
			else
			{
				if (mVerbose)
				{
					std::cerr << "# WARNING: Message is not a valid fragment. "
							  << "Dropping message..." << std::endl;
				}
			}
		}
		else
		{
			std::deque<mavlink_message_t>& queue = it->second;

			if (queue.empty())
			{
				if (offset == 0)
				{
					queue.push_back(msg);
				}
				else
				{
					if (mVerbose)
					{
						std::cerr << "# WARNING: Message is not a valid fragment. "
								  << "Dropping message..." << std::endl;
					}
				}
			}
			else
			{
				if (fragmentLength(queue.back()) + fragmentOffset(queue.back()) != offset)
				{
					if (mVerbose)
					{
						std::cerr << "# WARNING: Previous fragment(s) have been lost. "
								  << "Dropping message..." << std::endl;
					}
					queue.clear();
				}
				else
				{
					queue.push_back(msg);

					if (flags && 0x1 != 0x1)
					{
						reassemble = true;
					}
				}
			}
		}

		if (reassemble)
		{
			std::deque<mavlink_message_t>& queue = mFragmentQueue[streamID];

			std::string data;
			for (size_t i = 0; i < queue.size(); ++i)
			{
				data.append(payload + kHeaderLength, length);
			}

			std::tr1::shared_ptr<google::protobuf::Message> protobufMsg;

			protobufMsg->ParseFromString(data);

			mMessageQueue.push_back(protobufMsg);

			if (mVerbose)
			{
				std::cerr << "# INFO: Added reassembled message to queue." << std::endl;
			}
		}
	}

	bool getMessage(std::tr1::shared_ptr<google::protobuf::Message>& msg)
	{
		if (mMessageQueue.empty())
		{
			return false;
		}

		msg = mMessageQueue.front();
		mMessageQueue.pop_front();

		return true;
	}

private:
	bool validFragment(const mavlink_message_t& msg) const
	{
		if (msg.magic != MAVLINK_STX ||
			msg.len != kHeaderLength ||
			msg.msgid != MAVLINK_MSG_ID_EXTENDED_MESSAGE)
		{
			return false;
		}

		const uint8_t* payload = reinterpret_cast<const uint8_t*>(msg.payload64);

		uint16_t crc = crc_calculate(payload + kHeaderLength, fragmentLength(msg));
		if (msg.checksum != crc)
		{
			return false;
		}

		return true;
	}

	unsigned int fragmentLength(const mavlink_message_t& msg) const
	{
		const char* payload = reinterpret_cast<const char*>(msg.payload64);

		return *(reinterpret_cast<const unsigned int*>(payload + 2));
	}

	unsigned int fragmentOffset(const mavlink_message_t& msg) const
	{
		const char* payload = reinterpret_cast<const char*>(msg.payload64);

		return *(reinterpret_cast<const unsigned int*>(payload + 8));
	}

	const int kHeaderLength;
	const int kPayloadLength;
	unsigned short mStreamID;

	static std::tr1::shared_ptr<ProtobufFactory> mInstance;

	typedef std::map< unsigned short, std::deque<mavlink_message_t> > FragmentQueue;
	typedef std::deque< std::tr1::shared_ptr<google::protobuf::Message> > MessageQueue;

	FragmentQueue mFragmentQueue;
	MessageQueue mMessageQueue;

	bool mVerbose;
};

}
