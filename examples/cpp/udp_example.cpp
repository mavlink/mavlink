// Simple example receiving and sending MAVLink v2 over UDP using the C++11
// MAVLink library, based on POSIX APIs (e.g. Linux, BSD, macOS).
//
// This is the C++11 counterpart to ../c/udp_example.c. Instead of the C
// pack/decode functions it uses the generated message structs (e.g.
// mavlink::minimal::msg::HEARTBEAT) together with mavlink::MsgMap to
// serialize and deserialize to and from a mavlink_message_t.

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <ctime>

// The C++11 library is header-only and lives in the `mavlink` namespace. It
// also pulls in the C library (functions like mavlink_parse_char) into that
// same namespace.
#include <mavlink/common/common.hpp>

// The C++11 library does not ship a message registry, so the user has to
// provide this lookup. mavlink_parse_char() calls it (indirectly) to find the
// length and CRC extra for an incoming message id. We build it from the
// dialect's MESSAGE_ENTRIES table, which is sorted by message id.
namespace mavlink {

const mavlink_msg_entry_t* mavlink_get_msg_entry(uint32_t msgid)
{
    for (const auto& entry : common::MESSAGE_ENTRIES) {
        if (entry.msgid == msgid) {
            return &entry;
        }
    }
    return nullptr;
}

} // namespace mavlink

static void handle_heartbeat(const mavlink::mavlink_message_t& message);
static void receive_some(
    int socket_fd, struct sockaddr_in& src_addr, socklen_t& src_addr_len, bool& src_addr_set);
static void send_some(
    int socket_fd, const struct sockaddr_in& src_addr, socklen_t src_addr_len);
static void send_heartbeat(
    int socket_fd, const struct sockaddr_in& src_addr, socklen_t src_addr_len);

int main(int /*argc*/, char* /*argv*/[])
{
    // Open UDP socket
    const int socket_fd = socket(PF_INET, SOCK_DGRAM, 0);

    if (socket_fd < 0) {
        printf("socket error: %s\n", strerror(errno));
        return -1;
    }

    // Bind to port
    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &(addr.sin_addr)); // listen on all network interfaces
    addr.sin_port = htons(14550); // default port on the ground

    if (bind(socket_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) != 0) {
        printf("bind error: %s\n", strerror(errno));
        return -2;
    }

    // We set a timeout at 100ms to prevent being stuck in recvfrom for too
    // long and missing our chance to send some stuff.
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        printf("setsockopt error: %s\n", strerror(errno));
        return -3;
    }

    struct sockaddr_in src_addr {};
    socklen_t src_addr_len = sizeof(src_addr);
    bool src_addr_set = false;

    while (true) {
        // For illustration purposes we don't bother with threads or async here
        // and just interleave receiving and sending.
        // This only works if receive_some returns every now and then.
        receive_some(socket_fd, src_addr, src_addr_len, src_addr_set);

        if (src_addr_set) {
            send_some(socket_fd, src_addr, src_addr_len);
        }
    }

    return 0;
}

void receive_some(
    int socket_fd, struct sockaddr_in& src_addr, socklen_t& src_addr_len, bool& src_addr_set)
{
    // We just receive one UDP datagram and then return again.
    char buffer[2048]; // enough for MTU 1500 bytes

    const int ret = recvfrom(
        socket_fd, buffer, sizeof(buffer), 0, reinterpret_cast<struct sockaddr*>(&src_addr),
        &src_addr_len);

    if (ret < 0) {
        printf("recvfrom error: %s\n", strerror(errno));
        return;
    } else if (ret == 0) {
        // timeout, try again later
        return;
    }

    src_addr_set = true;

    mavlink::mavlink_message_t message;
    mavlink::mavlink_status_t status;
    for (int i = 0; i < ret; ++i) {
        if (mavlink::mavlink_parse_char(mavlink::MAVLINK_COMM_0, buffer[i], &message, &status) == 1) {

            // printf(
            //     "Received message %d from %d/%d\n",
            //     message.msgid, message.sysid, message.compid);

            switch (message.msgid) {
            case mavlink::minimal::msg::HEARTBEAT::MSG_ID:
                handle_heartbeat(message);
                break;
            }
        }
    }
}

void handle_heartbeat(const mavlink::mavlink_message_t& message)
{
    // Deserialize the raw message into the generated HEARTBEAT struct.
    mavlink::minimal::msg::HEARTBEAT heartbeat;
    mavlink::MsgMap map(&message);
    heartbeat.deserialize(map);

    printf("Got heartbeat from ");
    switch (static_cast<mavlink::minimal::MAV_AUTOPILOT>(heartbeat.autopilot)) {
    case mavlink::minimal::MAV_AUTOPILOT::GENERIC:
        printf("generic");
        break;
    case mavlink::minimal::MAV_AUTOPILOT::ARDUPILOTMEGA:
        printf("ArduPilot");
        break;
    case mavlink::minimal::MAV_AUTOPILOT::PX4:
        printf("PX4");
        break;
    default:
        printf("other");
        break;
    }
    printf(" autopilot\n");
}

void send_some(int socket_fd, const struct sockaddr_in& src_addr, socklen_t src_addr_len)
{
    // Whenever a second has passed, we send a heartbeat.
    static time_t last_time = 0;
    time_t current_time = time(NULL);
    if (current_time - last_time >= 1) {
        send_heartbeat(socket_fd, src_addr, src_addr_len);
        last_time = current_time;
    }
}

void send_heartbeat(int socket_fd, const struct sockaddr_in& src_addr, socklen_t src_addr_len)
{
    // Fill in the generated HEARTBEAT struct.
    mavlink::minimal::msg::HEARTBEAT heartbeat;
    heartbeat.type = static_cast<uint8_t>(mavlink::minimal::MAV_TYPE::GENERIC);
    heartbeat.autopilot = static_cast<uint8_t>(mavlink::minimal::MAV_AUTOPILOT::GENERIC);
    heartbeat.base_mode = 0;
    heartbeat.custom_mode = 0;
    heartbeat.system_status = static_cast<uint8_t>(mavlink::minimal::MAV_STATE::STANDBY);

    // Serialize it into a mavlink_message_t ...
    mavlink::mavlink_message_t message;
    mavlink::MsgMap map(message);
    heartbeat.serialize(map);

    // ... and finalize it by adding the header and CRC. This is where the
    // system and component id as well as the sequence number get filled in.
    const uint8_t system_id = 42;
    mavlink::mavlink_finalize_message_chan(
        &message,
        system_id,
        static_cast<uint8_t>(mavlink::minimal::MAV_COMPONENT::COMP_ID_PERIPHERAL),
        mavlink::MAVLINK_COMM_0,
        heartbeat.MIN_LENGTH,
        heartbeat.LENGTH,
        heartbeat.CRC_EXTRA);

    // Now we can convert the message into a byte buffer to send it out.
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    const int len = mavlink::mavlink_msg_to_send_buffer(buffer, &message);

    const int ret = sendto(
        socket_fd, buffer, len, 0, reinterpret_cast<const struct sockaddr*>(&src_addr),
        src_addr_len);
    if (ret != len) {
        printf("sendto error: %s\n", strerror(errno));
    } else {
        printf("Sent heartbeat\n");
    }
}
