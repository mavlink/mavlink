#include <deque>
#include <google/protobuf/message.h>
#include <tr1/memory>

namespace mavlink
{

class ProtobufFactory
{
public:
	ProtobufFactory();

	static std::tr1::shared_ptr<ProtobufFactory>& instance(void)
	{
		if (mInstance.get() == 0)
		{
			mInstance.reset(new ProtobufFactory);
		}
		return mInstance;
	}

	bool fragmentProtobufMessage(google::protobuf::Message& message,
								 std::vector<mavlink_message_t>& fragments)
	{
		std::string data;
		message.SerializeToString(&data);

		int dataSize = message.ByteSize();
		std::string msgTypeName = message.GetTypeName();

		return true;
	}

	void addFragment(mavlink_message_t& msg)
	{
		mDeque.push_back(msg);
	}

	bool getCompleteMessage(const ::google::protobuf::Message& msg)
	{
		return true;
	}

private:
	static std::tr1::shared_ptr<ProtobufFactory> mInstance;

	std::deque<mavlink_message_t> mFragmentQueue;
};

}
