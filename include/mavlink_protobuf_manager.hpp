#ifndef MAVLINKPROTOBUFMANAGER_HPP
#define MAVLINKPROTOBUFMANAGER_HPP

#include <google/protobuf/message.h>
#include <iostream>
#include <tr1/memory>

#include <checksum.h>
#include <common/mavlink.h>
#include <mavlink_types.h>
#include <pixhawk/pixhawk.pb.h>

namespace mavlink
{

class ProtobufManager
{
public:
	ProtobufManager()
	 : mRegisteredTypeCount(0)
	{
		// register PointCloudXYZI
		{
			std::tr1::shared_ptr<px::PointCloudXYZI> msg(new px::PointCloudXYZI);
			registerType(msg);
		}

		// register PointCloudXYZRGB
		{
			std::tr1::shared_ptr<px::PointCloudXYZRGB> msg(new px::PointCloudXYZRGB);
			registerType(msg);
		}
	}

	static std::tr1::shared_ptr<ProtobufManager>& instance(void)
	{
		if (mInstance.get() == 0)
		{
			mInstance.reset(new ProtobufManager);
		}
		return mInstance;
	}

	bool convert(uint8_t system_id, uint8_t component_id,
				 uint8_t target_system, uint8_t target_component,
				 const google::protobuf::Message& protobuf_msg,
				 mavlink_extended_message_t& mavlink_msg)
	{
		TypeMap::iterator it = mTypeMap.find(protobuf_msg.GetTypeName());
		if (it == mTypeMap.end())
		{
			std::cout << "# WARNING: Protobuf message with type "
					  << protobuf_msg.GetTypeName() << " is not registered."
					  << std::endl;
			return false;
		}

		uint8_t typecode = it->second;

		std::string data = protobuf_msg.SerializeAsString();

		mavlink_msg.base_msg.magic = MAVLINK_STX;
		mavlink_msg.base_msg.len = kExtendedHeaderLength;
		mavlink_msg.base_msg.seq = mavlink_get_channel_status(MAVLINK_COMM_0)->current_tx_seq;
		++mavlink_get_channel_status(MAVLINK_COMM_0)->current_tx_seq;
		mavlink_msg.base_msg.sysid = system_id;
		mavlink_msg.base_msg.compid = component_id;
		mavlink_msg.base_msg.msgid = MAVLINK_MSG_ID_EXTENDED_MESSAGE;

		uint8_t* payload = reinterpret_cast<uint8_t*>(mavlink_msg.base_msg.payload64);
		memcpy(payload, &target_system, 1);
		memcpy(payload + 1, &target_component, 1);
		memcpy(payload + 2, &typecode, 1);

		mavlink_msg.extended_payload_len = protobuf_msg.ByteSize();
		mavlink_msg.extended_payload = reinterpret_cast<uint8_t*>(realloc(mavlink_msg.extended_payload, protobuf_msg.ByteSize()));
		memcpy(mavlink_msg.extended_payload, &data[0], protobuf_msg.ByteSize());

		return true;
	}

	bool convert(const mavlink_extended_message_t& mavlink_msg,
				 std::tr1::shared_ptr<google::protobuf::Message>& protobuf_msg)
	{
		const uint8_t* payload = reinterpret_cast<const uint8_t*>(mavlink_msg.base_msg.payload64);

		uint8_t typecode = payload[2];
		if (typecode >= mTypeMap.size())
		{
			std::cout << "# WARNING: Protobuf message with type code "
					  << typecode << " is not registered." << std::endl;
			return false;
		}

		std::tr1::shared_ptr<google::protobuf::Message>& msg = mMessages.at(typecode);
		msg->ParseFromArray(mavlink_msg.extended_payload, mavlink_msg.extended_payload_len);

		protobuf_msg = msg;

		return true;
	}

private:
	void registerType(const std::tr1::shared_ptr<google::protobuf::Message>& msg)
	{
		mTypeMap[msg->GetTypeName()] = mRegisteredTypeCount;
		++mRegisteredTypeCount;
		mMessages.push_back(msg);
	}

	int mRegisteredTypeCount;

	typedef std::map<std::string, uint8_t> TypeMap;
	TypeMap mTypeMap;
	std::vector< std::tr1::shared_ptr<google::protobuf::Message> > mMessages;

	static std::tr1::shared_ptr<ProtobufManager> mInstance;

	static const int kExtendedHeaderLength;
	/**
	 * Extended header structure
	 * =========================
	 *   byte 0 - target_system
	 *   byte 1 - target_component
	 *   byte 2 - extended message id
	 */
};

std::tr1::shared_ptr<ProtobufManager> ProtobufManager::mInstance;
const int ProtobufManager::kExtendedHeaderLength = 3;

}

#endif
