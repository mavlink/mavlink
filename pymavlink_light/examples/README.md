## pymavlink Examples

| File            | Description   |
| --------------- |:--------------|
| mavtest.py      |  Demonstrates creating a mavlink message, putting it on a buffer and then decoding from said buffer. |
| bwtest.py       |  Check bandwith of link. Display results on terminal. |
| mav_accel.py    |  Show accelerometer calibration for a set of logs.  |
| apmsetrate.py   |  Set the Stream Rate on an APM. |
| magtest.py      |  Rotate APMs on bench to test magnetometers.  |
| mavtcpsniff.py  |  Connect as a client to two tcpip ports on localhost with mavlink packets. Pass them both directions, and show packets in human-readable format on-screen.  |
| mav2pcap.py     |  Packetizes a binary MAVLink stream. The resulting packets are stored into a PCAP file, which is compatible to tools like Wireshark.  |
| wptogpx.py      |  Extract GPS data from a waypoint file, and create a GPX file, for loading into Google Earth.  |
| mavgps.py       |  Allows connection of the uBlox u-Center software to a uBlox GPS device connected to a PX4 or Pixhawk device, using Mavlink's SERIAL_CONTROL support to route serial traffic to/from the GPS, and exposing the data to u-Center via a local TCP connection.  |
| mavtester.py    |  Test mavlink messages.
