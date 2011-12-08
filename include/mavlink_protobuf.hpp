#ifndef MAVLINKPROTOBUF_HPP
#define MAVLINKPROTOBUF_HPP

#include <google/protobuf/message.h>
#include <iostream>
#include <tr1/memory>

#include <checksum.h>
#include <common/mavlink.h>
#include <mavlink_types.h>

const int kExtendedHeaderLength = 2;

bool mavlink_protobuf_convert(uint8_t system_id, uint8_t component_id,
							  uint8_t target_system, uint8_t target_component,
							  google::protobuf::Message& protobuf_msg,
							  mavlink_extended_message_t& mavlink_msg)
{
	std::string data;
	protobuf_msg.SerializeToString(&data);

	mavlink_msg.base_msg.magic = MAVLINK_STX;
	mavlink_msg.base_msg.len = kExtendedHeaderLength;
	mavlink_msg.base_msg.seq = mavlink_get_channel_status(MAVLINK_COMM_0)->current_tx_seq;
	++mavlink_get_channel_status(MAVLINK_COMM_0)->current_tx_seq;
	mavlink_msg.base_msg.sysid = system_id;
	mavlink_msg.base_msg.compid = component_id;
	mavlink_msg.base_msg.msgid = 255;

	uint8_t* payload = reinterpret_cast<uint8_t*>(mavlink_msg.base_msg.payload64);
	memcpy(payload, &target_system, 1);
	memcpy(payload + 1, &target_component, 1);

	mavlink_msg.extended_payload_len = protobuf_msg.ByteSize();
	mavlink_msg.extended_payload = reinterpret_cast<uint8_t*>(realloc(mavlink_msg.extended_payload, protobuf_msg.ByteSize()));
	memcpy(mavlink_msg.extended_payload, &data[0], protobuf_msg.ByteSize());

	return true;
}

#endif
