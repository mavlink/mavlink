#ifndef MAVLINK_TYPES_H_
#define MAVLINK_TYPES_H_

#include "inttypes.h"

enum MAV_ACTION {
	MAV_ACTION_HOLD = 0,
			MAV_ACTION_MOTORS_START = 1,
			MAV_ACTION_LAUNCH = 2,
			MAV_ACTION_RETURN = 3,
			MAV_ACTION_EMCY_LAND = 4,
			MAV_ACTION_EMCY_KILL = 5,
			MAV_ACTION_CONFIRM_KILL = 6,
			MAV_ACTION_CONTINUE = 7,
			MAV_ACTION_MOTORS_STOP = 8,
			MAV_ACTION_HALT = 9,
			MAV_ACTION_SHUTDOWN = 10,
			MAV_ACTION_REBOOT = 11,
			MAV_ACTION_SET_MANUAL = 12,
			MAV_ACTION_SET_AUTO = 13
};

enum MAV_MODE
{
	MAV_MODE_UNINIT = 0,
			MAV_MODE_LOCKED,
			MAV_MODE_MANUAL,
			MAV_MODE_GUIDED,
			MAV_MODE_AUTO,
			MAV_MODE_TEST1,
			MAV_MODE_TEST2,
			MAV_MODE_TEST3
};

enum MAV_STATE
{
	MAV_STATE_UNINIT = 0,
			MAV_STATE_BOOT,
			MAV_STATE_CALIBRATING,
			MAV_STATE_STANDBY,
			MAV_STATE_ACTIVE,
			MAV_STATE_CRITICAL,
			MAV_STATE_EMERGENCY,
			MAV_STATE_POWEROFF
};

enum MAV_NAV
{
	MAV_NAV_GROUNDED = 0,
			MAV_NAV_LIFTOFF,
			MAV_NAV_HOLD,
			MAV_NAV_WAYPOINT,
			MAV_NAV_VECTOR,
			MAV_NAV_RETURNING,
			MAV_NAV_LANDING,
			MAV_NAV_LOST
};

enum MAV_TYPE
{
	MAV_GENERIC = 0,
			MAV_FIXED_WING = 1,
			MAV_QUADROTOR = 2,
			MAV_COAXIAL = 3,
			MAV_HELICOPTER = 4,
			MAV_GROUND = 5,
			OCU = 6
};

#define MAVLINK_STX 0x55 ///< Packet start sign
#define MAVLINK_STX_LEN 1 ///< Length of start sign
#define MAVLINK_MAX_PAYLOAD_LEN 255 ///< Maximum payload length

#define MAVLINK_CORE_HEADER_LEN 5 ///< Length of core header (of the comm. layer): message length (1 byte) + message sequence (1 byte) + message system id (1 byte) + message component id (1 byte) + message type id (1 byte)
#define MAVLINK_NUM_HEADER_BYTES (MAVLINK_CORE_HEADER_LEN + MAVLINK_STX_LEN) ///< Length of all header bytes, including core and checksum
#define MAVLINK_NUM_CHECKSUM_BYTES 2
#define MAVLINK_NUM_NON_PAYLOAD_BYTES (MAVLINK_NUM_HEADER_BYTES + MAVLINK_NUM_CHECKSUM_BYTES)
#define MAVLINK_NUM_NON_STX_PAYLOAD_BYTES (MAVLINK_NUM_NON_PAYLOAD_BYTES - MAVLINK_STX_LEN)

#define MAVLINK_MAX_PACKET_LEN (MAVLINK_MAX_PAYLOAD_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES) ///< Maximum packet length
//#define MAVLINK_MAX_DATA_LEN MAVLINK_MAX_PACKET_LEN - MAVLINK_STX_LEN

typedef struct __mavlink_message {
	uint8_t len;     ///< Length of payload
	uint8_t seq;     ///< Sequence of packet
	uint8_t sysid;   ///< ID of message sender system/aircraft
	uint8_t compid;  ///< ID of the message sender component
	uint8_t msgid;   ///< ID of message in payload
        uint8_t payload[MAVLINK_MAX_PAYLOAD_LEN]; ///< Payload data, ALIGNMENT IMPORTANT ON MCU
	uint8_t ck_a;    ///< Checksum high byte
	uint8_t ck_b;    ///< Checksum low byte
} mavlink_message_t;

typedef enum {
	MAVLINK_COMM_0,
	MAVLINK_COMM_1,
	MAVLINK_COMM_2,
	MAVLINK_COMM_3,
	MAVLINK_COMM_4,
	MAVLINK_COMM_5,
	MAVLINK_COMM_6,
	MAVLINK_COMM_7,
	MAVLINK_COMM_8,
	MAVLINK_COMM_NB,
	MAVLINK_COMM_NB_HIGH = 512
} mavlink_channel_t;

typedef enum {
	MAVLINK_PARSE_STATE_UNINIT=0,
			MAVLINK_PARSE_STATE_IDLE,
			MAVLINK_PARSE_STATE_GOT_STX,
			MAVLINK_PARSE_STATE_GOT_SEQ,
			MAVLINK_PARSE_STATE_GOT_LENGTH,
			MAVLINK_PARSE_STATE_GOT_SYSID,
			MAVLINK_PARSE_STATE_GOT_COMPID,
			MAVLINK_PARSE_STATE_GOT_MSGID,
			MAVLINK_PARSE_STATE_GOT_PAYLOAD,
			MAVLINK_PARSE_STATE_GOT_CRC1
} mavlink_parse_state_t; ///< The state machine for the comm parser

typedef struct __mavlink_status {
	uint8_t ck_a;                       ///< Checksum byte 1
	uint8_t ck_b;                       ///< Checksum byte 2
	uint8_t msg_received;               ///< Number of received messages
	uint8_t buffer_overrun;             ///< Number of buffer overruns
	uint8_t parse_error;                ///< Number of parse errors
	mavlink_parse_state_t parse_state;  ///< Parsing state machine
	uint8_t packet_idx;                 ///< Index in current packet
	uint8_t current_seq;                ///< Sequence number of last packet
	uint16_t packet_rx_success_count;   ///< Received packets
	uint16_t packet_rx_drop_count;      ///< Number of packet drops
} mavlink_status_t;

#endif /* MAVLINK_TYPES_H_ */
