/*
  simple MAVLink testsuite for C
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <stdbool.h>

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS
#define MAVLINK_USE_MESSAGE_INFO
#define MAVLINK_COMM_NUM_BUFFERS 2

// this trick allows us to make mavlink_message_t as small as possible
// for this dialect, which saves some memory
#include <version.h>
#define MAVLINK_MAX_PAYLOAD_LEN MAVLINK_MAX_DIALECT_PAYLOAD_SIZE

#include <mavlink_types.h>
static mavlink_system_t mavlink_system = {42,11,};

#define MAVLINK_ASSERT(x) assert(x)
static void comm_send_ch(mavlink_channel_t chan, uint8_t c);

static mavlink_message_t last_msg;

#include <mavlink.h>
#include <testsuite.h>

static unsigned chan_counts[MAVLINK_COMM_NUM_BUFFERS];

#ifndef MAVLINK_HAVE_EXPECTED_MESSAGE_LENGTH
static const uint8_t message_lengths[] = MAVLINK_MESSAGE_LENGTHS;
#define mavlink_expected_message_length(msg) message_lengths[(msg)->msgid]
#endif

#ifndef MAVLINK_HAVE_GET_MESSAGE_INFO
static const mavlink_message_info_t message_info[] = MAVLINK_MESSAGE_INFO;
#define mavlink_get_message_info(msg) &message_info[(msg)->msgid]
#endif

static unsigned error_count;


static void print_one_field(mavlink_message_t *msg, const mavlink_field_info_t *f, int idx)
{
#define PRINT_FORMAT(f, def) (f->print_format?f->print_format:def)
	switch (f->type) {
	case MAVLINK_TYPE_CHAR:
		printf(PRINT_FORMAT(f, "%c"), _MAV_RETURN_char(msg, f->wire_offset+idx*1));
		break;
	case MAVLINK_TYPE_UINT8_T:
		printf(PRINT_FORMAT(f, "%u"), _MAV_RETURN_uint8_t(msg, f->wire_offset+idx*1));
		break;
	case MAVLINK_TYPE_INT8_T:
		printf(PRINT_FORMAT(f, "%d"), _MAV_RETURN_int8_t(msg, f->wire_offset+idx*1));
		break;
	case MAVLINK_TYPE_UINT16_T:
		printf(PRINT_FORMAT(f, "%u"), _MAV_RETURN_uint16_t(msg, f->wire_offset+idx*2));
		break;
	case MAVLINK_TYPE_INT16_T:
		printf(PRINT_FORMAT(f, "%d"), _MAV_RETURN_int16_t(msg, f->wire_offset+idx*2));
		break;
	case MAVLINK_TYPE_UINT32_T:
		printf(PRINT_FORMAT(f, "%lu"), (unsigned long)_MAV_RETURN_uint32_t(msg, f->wire_offset+idx*4));
		break;
	case MAVLINK_TYPE_INT32_T:
		printf(PRINT_FORMAT(f, "%ld"), (long)_MAV_RETURN_int32_t(msg, f->wire_offset+idx*4));
		break;
	case MAVLINK_TYPE_UINT64_T:
		printf(PRINT_FORMAT(f, "%llu"), (unsigned long long)_MAV_RETURN_uint64_t(msg, f->wire_offset+idx*8));
		break;
	case MAVLINK_TYPE_INT64_T:
		printf(PRINT_FORMAT(f, "%lld"), (long long)_MAV_RETURN_int64_t(msg, f->wire_offset+idx*8));
		break;
	case MAVLINK_TYPE_FLOAT:
		printf(PRINT_FORMAT(f, "%f"), (double)_MAV_RETURN_float(msg, f->wire_offset+idx*4));
		break;
	case MAVLINK_TYPE_DOUBLE:
		printf(PRINT_FORMAT(f, "%f"), _MAV_RETURN_double(msg, f->wire_offset+idx*8));
		break;
	}
}

static void print_field(mavlink_message_t *msg, const mavlink_field_info_t *f)
{
	printf("%s: ", f->name);
	if (f->array_length == 0) {
		print_one_field(msg, f, 0);
		printf(" ");
	} else {
		unsigned i;
		/* print an array */
		if (f->type == MAVLINK_TYPE_CHAR) {
			printf("'%.*s'", f->array_length,
			       f->wire_offset+(const char *)_MAV_PAYLOAD(msg));
			
		} else {
			printf("[ ");
			for (i=0; i<f->array_length; i++) {
				print_one_field(msg, f, i);
				if (i < f->array_length) {
					printf(", ");
				}
			}
			printf("]");
		}
	}
	printf(" ");
}

static void print_message(mavlink_message_t *msg)
{
	const mavlink_message_info_t *m = mavlink_get_message_info(msg);
	if (m == NULL) {
		printf("ERROR: no message info for %u\n", msg->msgid);
		error_count++;
		return;
	}
	const mavlink_field_info_t *f = m->fields;
	unsigned i;
	printf("%s { ", m->name);
	for (i=0; i<m->num_fields; i++) {
		print_field(msg, &f[i]);
	}
	printf("}\n");
}

#ifdef MAVLINK_SIGNING_FLAG_SIGN_OUTGOING
static mavlink_signing_t signing_in[MAVLINK_COMM_NUM_BUFFERS];
static mavlink_signing_streams_t signing_streams_in;
#endif

static void comm_send_ch(mavlink_channel_t chan, uint8_t c)
{
	mavlink_status_t status;
	memset(&status, 0, sizeof(status));
#ifdef MAVLINK_SIGNING_FLAG_SIGN_OUTGOING
	status.signing = &signing_in[chan];
        status.signing_streams = &signing_streams_in;
#endif
	if (mavlink_parse_char(chan, c, &last_msg, &status)) {
		print_message(&last_msg);
		chan_counts[chan]++;
		/* channel 0 gets 3 messages per message, because of
		   the channel defaults for _pack() and _encode() */
		if (chan == MAVLINK_COMM_0 && status.current_rx_seq != (uint8_t)(chan_counts[chan]*3)) {
			printf("Channel 0 sequence mismatch error at packet %u (rx_seq=%u)\n", 
			       chan_counts[chan], status.current_rx_seq);
			error_count++;
		} else if (chan > MAVLINK_COMM_0 && status.current_rx_seq != (uint8_t)chan_counts[chan]) {
			printf("Channel %u sequence mismatch error at packet %u (rx_seq=%u)\n", 
			       (unsigned)chan, chan_counts[chan], status.current_rx_seq);
			error_count++;
		}
                // we only check the lengtth for MAVLink1. In MAVLink2 packets are zero trimmed
		if (mavlink_expected_message_length(&last_msg) > last_msg.len && last_msg.magic == 254) {
			printf("Incorrect message length %u for message %u - expected %u\n", 
			       (unsigned)last_msg.len, (unsigned)last_msg.msgid,
                               mavlink_expected_message_length(&last_msg));
			error_count++;
		}
	}
	if (status.packet_rx_drop_count != 0) {
		printf("Parse error at packet %u\n", chan_counts[chan]);
		error_count++;
	}
}

int main(void)
{
	mavlink_channel_t chan;

	mavlink_test_all(11, 10, &last_msg);
	for (chan=MAVLINK_COMM_0; chan<=MAVLINK_COMM_1; chan++) {
		printf("Received %u messages on channel %u OK\n", 
		       chan_counts[chan], (unsigned)chan);
	}
	if (error_count != 0) {
		printf("Error count %u\n", error_count);
		exit(1);
	}
	printf("No errors detected\n");

#ifdef MAVLINK_SIGNING_FLAG_SIGN_OUTGOING
	mavlink_status_t *status;

	printf("Testing signing\n");
	mavlink_signing_t signing;
	mavlink_signing_streams_t signing_streams;
        memset(&signing, 0, sizeof(signing));
        memset(&signing_streams, 0, sizeof(signing_streams));
	signing.flags = MAVLINK_SIGNING_FLAG_SIGN_OUTGOING;
	signing.link_id = 0;
	signing.timestamp = 1;
	memset(signing.secret_key, 42, sizeof(signing.secret_key));

        status = mavlink_get_channel_status(MAVLINK_COMM_1);
	status->signing = &signing;
        status->signing_streams = &signing_streams;

	mavlink_test_all(11, 10, &last_msg);
	for (chan=MAVLINK_COMM_0; chan<=MAVLINK_COMM_1; chan++) {
		printf("Received %u messages on channel %u OK\n", 
		       chan_counts[chan], (unsigned)chan);
	}
	if (error_count != 0) {
		printf("Error count %u\n", error_count);
		exit(1);
	}
	printf("No errors detected\n");	
#endif

#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        status = mavlink_get_channel_status(MAVLINK_COMM_0);
        status->flags |= MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
        status->signing = NULL;
        status = mavlink_get_channel_status(MAVLINK_COMM_1);
        status->flags |= MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
        status->signing = NULL;
        printf("Testing sending as MAVLink1\n");
        
	mavlink_test_all(11, 10, &last_msg);
	for (chan=MAVLINK_COMM_0; chan<=MAVLINK_COMM_1; chan++) {
		printf("Received %u messages on channel %u OK\n", 
		       chan_counts[chan], (unsigned)chan);
	}
	if (error_count != 0) {
		printf("Error count %u\n", error_count);
		exit(1);
	}
	printf("No errors detected\n");
#endif

	
	return 0;
}

