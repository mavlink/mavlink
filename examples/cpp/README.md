# Simple C++ example

Simple example receiving and sending MAVLink v2 over UDP based on POSIX APIs (e.g. Linux, BSD, macOS).

This is the C++11 counterpart to the [C example](../c). Instead of the C pack/decode functions it uses the generated message structs (e.g. `mavlink::minimal::msg::HEARTBEAT`) together with `mavlink::MsgMap` to serialize and deserialize to and from a `mavlink_message_t`.

> **Note** The C++11 library requires the user to provide a `mavlink::mavlink_get_msg_entry()` function so that `mavlink_parse_char()` can look up the length and CRC extra of incoming messages.
> This example builds it from the dialect's `MESSAGE_ENTRIES` table (see the top of `udp_example.cpp`).

## Install MAVLink

In top level directory, build and install the MAVLink headers locally into the install folder:

```
cmake -Bbuild -H. -DCMAKE_INSTALL_PREFIX=install
cmake --build build --target install
```

This installs both the C (`*.h`) and C++11 (`*.hpp`) headers.

## Build example

In the example directory, build the example while passing the local install directory:

```
cd examples/cpp
cmake -Bbuild -H. -DCMAKE_PREFIX_PATH=$(pwd)/../../install
cmake --build build
```
