# Guide to MAVLink2

MAVLink2 is a new variant of the MAVLink protocol designed to bring
more flexibility and security to MAVLink communication.

A background document on the MAVLink2 design is available here:

  https://docs.google.com/document/d/1XtbD0ORNkhZ8eKrsbSIZNLyg9sFRXMXbsR2mp37KbIg/edit?usp=sharing

it is suggested that you start by reading that document.

The key features of MAVLink2 are:

* support for more than 256 message IDs
* support for multiple message dialects in one implementation
* support for packet signing
* support for extending existing MAVLink messages
* support for variable length arrays

The first 3 features in this list are implemented now and ready for
testing. The last two features are not implemented yet.

Most of the rest of this document is a guide for programmers
interested in using MAVLink2 in their applications. The two language
bindings that are covered are C and Python. Bindings for MAVLink2 for
other languages are not implemented yet.

## Using the C implementation

Using the C implementation of MAVLink2 is very similar to using the
existing MAVLink1 implementation. You start by generating the MAVLink2
headers using mavgen.py, but passing the --wire-protocol=2.0
option. For example:

```
 mavgen.py --lang C message_definitions/v1.0/ardupilotmega.xml -o generator/C/include_v2.0 --wire-protocol=2.0
```

This will generate a set of C headers in the generator/C/include_v2.0
directory. These headers offer the same range of APIs as was offered
by MAVLink1. The major changes from an API perspective are:

* all methods that take a uint8_t msgId as an argument now take both a
  uint8_t dialect and a uint16_t msgId. This reflects to change in the
  MAVLink2 protocol to extend the msgId to 16 bits and to add the
  dialect in the message header.

* you don't need to provide a message CRC table any more, or message
  length table. These have been folded into a single packed table,
  replacing the old table which was indexed by msgId. That was
  necessary to cope with the much larger 24 bit namespace of message
  IDs and dialects.

For convenience in converting code a new MAVLINK_MSG_ID_name_TUPLE
macro is generated for every message which gives the tuple of
"dialect, msgId". This can be used in the argument to functions that
take a dialect and msgId. For example:

```
     _mav_finalize_message_chan_send(chan, 
                                     MAVLINK_MSG_ID_MISSION_ITEM_TUPLE,
                                     (const char *)&ret_packet,
                                     MAVLINK_MSG_ID_MISSION_ITEM_LEN,
                                     MAVLINK_MSG_ID_MISSION_ITEM_CRC);
```

### Sending and receiving MAVLink1 packets

The C implementation of MAVLink2 supports sending and receiving
MAVLink1 packets as well. To force sending MAVLink1 packets on a
particular channel you change the flags field of the status
object. For example:

```
   mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
   status->flags |= MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
```

that will cause subsequent packets on the given channel to be sent as
MAVLink1.

Incoming MAVLink1 packets will be automatically handled as
MAVLink1. If you need to determine if a particular message was received as
MAVLink1 or MAVLink2 then you can use the magic field of the message,
like this:

```
  if (msg->magic = MAVLINK_STX_MAVLINK1) {
     printf("This is a MAVLink1 message\n");
  }
```

In most cases this should not be necessary as the xml message
definition files for MAVLink1 and MAVLink2 are the same, so you can
treat incoming MAVLink1 messages the same as MAVLink2 messages.

Note that MAVLink1 is restricted to messageIDs less than 256, so any
messages with a higher messageID won't be received as MAVLink1.

### Handling message signing

One of the key features of MAVLink2 is support for signing of
messages. To enable signing in your application you will need to add
some additional code. In particular you will need to add:

* code to handle the SETUP_SIGNING message
* code to setup and teardown signing on a link
* code to save and load the secret key and timestamp in persistent storage
* a callback to allow for accepting of certain kinds of unsigned messages

Example code in ArduPilot for each of these pieces if available here:

 https://github.com/tridge/ardupilot/blob/mavlink2-wip/libraries/GCS_MAVLink/GCS_Signing.cpp

### Handling SETUP_SIGNING

The SETUP_SIGNING message is the mechanism for a GCS to setup a
signing key on a MAVLink2 device. It takes a 32 byte secret key and an
initial timestamp. The method of generating the 32 byte secret key is
up to the GCS implementation, although it is suggested that all GCS
implementations should support the use of a sha256 hash of a user
provided passphrase.

### Handling timestamps

The timestamp is a 64 bit number, and is in units of 10 microseconds
since 1st January 2015. For systems where the time since 1/1/1970 is
available (the unix epoch) you can use an offset in seconds of
1420070400.

Storage and handling of the timestamp is critical to the security of
the signing system. The rules are:

* the current timestamp should be stored regularly in persistent
  storage (suggested at least once a minute)
* the timestamp used on startup should be the maximum of the timestamp
  implied by the system clock and the stored timestamp
* if the system does not have a RTC mechanism then it should update
  its timestamp when GPS lock is achieved. The maximum of the
  timestamp from the GPS and the stored timestamp should be used
* the timestamp should be incremented by one on each message send.
  This is done for you by the generated headers.
* when a correctly signed message is decoded the timestamp should be
  replaced by the timestamp of the incoming message if that timestamp
  is greater than the current timestamp. This is done for you by the
  generated headers
* the timestamp on incoming signed messages should be checked against
  the previous timestamp for the incoming
  (linkID,srcSystem,SrcComponent) tuple and the message rejected if it
  is smaller. This is done for you by generated headers.
* if there is no previous message with the given
  (linkID,srcSystem,SrcComponent) then the timestamp should be
  accepted if it not more than 6 million (one minute) behind the
  current timestamp

### Enabling signing on a channel

To enable signing on a channel you need to fill in two pointers in the
status structure for the cnannel. The two pointed are:

```
   mavlink_signing_t *signing;
   mavlink_signing_streams_t *signing_streams;
```

The signing pointer controls signing for this stream. It is
per-stream, and contains the secret key, the timestamp and a set of
flags, plus an optional callback function for accepting unsigned
packets. Typical setup would be:

```
    memcpy(signing.secret_key, key.secret_key, 32);
    signing.link_id = (uint8_t)chan;
    signing.timestamp = key.timestamp;
    signing.flags = MAVLINK_SIGNING_FLAG_SIGN_OUTGOING;
    signing.accept_unsigned_callback = accept_unsigned_callback;
    mavlink_status_t *status = mavlink_get_channel_status(chan);
    status.signing = &signing;
```

The signing_streams pointer is a structure used to record the previous
timestamp for a (linkId,srcSystem,SrcComponent) tuple. This must point
to a structure that is common to all channels in order to prevent
inter-channel replay attacks. Typical setup is:

```
    mavlink_status_t *status = mavlink_get_channel_status(chan);
    status.signing_streams = &signing_streams;
```

The maximum number of signing streams supported is given by the
MAVLINK_MAX_SIGNING_STREAMS macro. This defaults to 16, but it may be
worth raising this for GCS implementations. If the C implementation
runs out of signing streams then new streams will be rejected.

### Using the accept_unsigned_callback

In the signing structure there is an optional accept_unsigned_callback
function pointer. The C prototype for this function is:

```
    bool accept_unsigned_callback(const mavlink_status_t *status,
                                  uint8_t dialect, uint16_t msgId);
```                                     

If set in the signing structure then this function will be called on
any unsigned packet (including all MAVLink1 packets) or any packet
where the signature is incorrect. The function offers a way for the
implementation to allow unsigned packets to be accepted.

The rules for what unsigned packets should be accepted is
implementation specific, but it is suggested the following rules be
considered:

* have a mechanism for marking a particular communication channel as
  being secure (such as a USB connection) to allow for signing
  setup.
* always accept RADIO_STATUS packets for feedback from 3DR radios
  (which don't do signing)

For example:

```
static const struct {
    uint8_t dialect;
    uint16_t msgId;
} accept_list[] = {
    { MAVLINK_MSG_ID_RADIO_STATUS_TUPLE }
};
    
static bool accept_unsigned_callback(const mavlink_status_t *status,
                                     uint8_t dialect, uint16_t msgId)
{
    if (status == mavlink_get_channel_status(MAVLINK_COMM_0)) {
        // always accept channel 0, assumed to be secure channel. This
        // is USB on PX4 boards
        return true;
    }
    for (uint8_t i=0; i<ARRAY_SIZE(accept_list); i++) {
        if (accept_list[i].dialect == dialect &&
            accept_list[i].msgId == msgId) {
            return true;
        }
    }
    return false;
}
```
