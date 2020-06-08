/*   
Dongsin Kim (ds.justin.kim @ gmail.com)
*/


/*
MIT License

Copyright (c) [2020] [Dongsin Kim]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


/*
This is a simple MAVLink example for beginners who are interested in receiving and 
sending some messages from/to the ArduPilot SITL (Simulation In The Loop) virtual
vehicle, specifically focus on ArduCopter, using the MAVLink C libraries.

This code contains three parts:
1. Configure the connection with the vehicle using the address "127.0.0.1:14550".
2. Send a SET_MODE command to the vehicle.
3. Receive a GLOBAL_POSITION_INT message from the vehicle.
*/


/* 
REFERENCE
1. https://discuss.ardupilot.org/t/mavlink-step-by-step/
2. https://discuss.ardupilot.org/t/connecting-to-arducopter-through-udp
3. https://github.com/mavlink/mavlink/blob/master/examples/linux/mavlink_udp.c
*/


#include <stdio.h>      // printf()
#include <stdlib.h>     // EXIT_FAILURE
#include <unistd.h>     // close()
#include <sys/types.h>  // socket()
#include <sys/socket.h> // socket()
#include <netinet/in.h> // IPPROTO_UDP
#include <string.h>     // strcpy()
#include <arpa/inet.h>  // inet_ntoa()
/* This assumes you have the mavlink headers on your include path
 or in the same folder as this source file */
#include <mavlink.h>

#define BUFFER_LENGTH 2041 // minimum buffer size that can be used with qnx (I don't know why)


int main (int argc, char* argv[])
{
	/* Initialize */
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in gcAddr;
	struct sockaddr_in locAddr;
	uint8_t buf[BUFFER_LENGTH];
	__socklen_t fromlen = sizeof(gcAddr);
	ssize_t recsize;
	mavlink_status_t status;
	mavlink_message_t msg;
	int comm_channel = MAVLINK_COMM_0;
	uint16_t len;
	int bytes_sent;
	int i = 0;
	int k = 0;

	/* Configure locAddr */
	memset(&locAddr, 0, sizeof(locAddr));
	locAddr.sin_family = AF_INET;
	locAddr.sin_addr.s_addr = INADDR_ANY;
	locAddr.sin_port = htons(14551);


	/* Bind the socket to port 14551 */ 
	if (-1 == bind(sock,(struct sockaddr *)&locAddr, sizeof(struct sockaddr)))
    {
		perror("error bind failed");
		close(sock);
		exit(EXIT_FAILURE);
    }


	/* Configure gcAddr */
	memset(&gcAddr, 0, sizeof(gcAddr));
	gcAddr.sin_family = AF_INET;
	gcAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	gcAddr.sin_port = htons(14550);


	/* Set mode GUIDED */
	memset(buf, 0, BUFFER_LENGTH); 																  // needed before set_mode
	recsize = recvfrom(sock, (void *)buf, BUFFER_LENGTH, 0, (struct sockaddr*)&gcAddr, &fromlen); // needed before set_mode
	mavlink_msg_set_mode_pack(
		1,				// uint8_t system_id       (of the heartbeat sender)
		108,			// uint8_t component_id    (of the heartbeat sender)
		&msg,			// mavlink_message_t* msg
		1,				// uint8_t target_system
		1,				// uint8_t base_mode
		4);				// uint32_t custom_mode
	len = mavlink_msg_to_send_buffer(buf, &msg); // Pack a message to send it over a serial byte stream
	bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in)); // Send a message on a socket
	

	/* Loop */
	while (1)
	{
		/* Send Heartbeat */
		mavlink_msg_heartbeat_pack(
			1,                            // uint8_t system_id       (of the heartbeat sender)
			108,                          // uint8_t component_id    (of the heartbeat sender)
			&msg,                         // mavlink_message_t* msg
			MAV_TYPE_ONBOARD_CONTROLLER,  // uint8_t type
			MAV_AUTOPILOT_INVALID,        // uint8_t autopilot
			0, 			                  // uint8_t base_mode
			0,                            // uint32_t custom_mode
			0);                           // uint8_t system_status
		len = mavlink_msg_to_send_buffer(buf, &msg); // Pack a message to send it over a serial byte stream
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in)); // Send a message on a socket


		/* Listen for a message from MAV */
		memset(buf, 0, BUFFER_LENGTH);
		recsize = recvfrom(sock, (void *)buf, BUFFER_LENGTH, 0, (struct sockaddr*)&gcAddr, &fromlen);
		if (recsize > 0) // Something received
		{
			for (i = 0; i < recsize; ++i)
			{
				if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) // Parse packet
				{
					/* Packet received */
					// printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
					switch(msg.msgid)
					{
						// case MAVLINK_MSG_ID_HEARTBEAT: // #0
						// {
						// 	mavlink_heartbeat_t hb;
						// 	mavlink_msg_heartbeat_decode(&msg, &hb);
						// 	printf("\n");
						// 	printf("Custom mode: %u\n", hb.custom_mode);
						// 	printf("Type: %u\n", hb.type);
						// 	printf("Autopilot: %u\n", hb.autopilot);
						// 	printf("Base mode: %u\n", hb.base_mode);
						// 	printf("System status: %u\n", hb.system_status);
						// 	printf("Mavlink version: %u\n", hb.mavlink_version);
						// }
						// case MAVLINK_MSG_ID_ATTITUDE: // #30
						// {
						// 	mavlink_attitude_t attitude;
						// 	mavlink_msg_attitude_decode(&msg, &attitude);
						// 	printf("\n");
						// 	printf("Time boot: %u [ms]\n", attitude.time_boot_ms);
						// 	printf("Roll: %f [rad]\n", attitude.roll);   // angle (-pi..+pi)
						// 	printf("Pitch: %f [rad]\n", attitude.pitch); // angle (-pi..+pi) 
						// 	printf("Yaw: %f [rad]\n", attitude.yaw);     // angle (-pi..+pi)
						// 	printf("Rollspeed: %f [rad/s]\n", attitude.rollspeed);
						// 	printf("Pitchspeed: %f [rad/s]\n", attitude.pitchspeed);
						// 	printf("Yawspeed: %f [rad/s]\n", attitude.yawspeed);																												
						// }
						case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: // #33
						{
							mavlink_global_position_int_t gp;
							mavlink_msg_global_position_int_decode(&msg, &gp);
							printf("\n");
							printf("Time boot: %u [ms]\n", gp.time_boot_ms);
							printf("Latitude: %d [degE7]\n", gp.lat);
							printf("Longitude: %d [degE7]\n", gp.lon);
							printf("Relative altitude: %d [degE7]\n", gp.relative_alt);					
							printf("vx: %d [cm/s]\n", gp.vx);
							printf("vy: %d [cm/s]\n", gp.vy);
							printf("vz: %d [cm/s]\n", gp.vz);
							break;
						}
						// case MAVLINK_MSG_ID_RC_CHANNELS_RAW: // #35
						// {
						// 	mavlink_rc_channels_raw_t chs;
						// 	mavlink_msg_rc_channels_raw_decode(&msg, &chs);
						// 	printf("\n");
						// 	printf("RC channel 1: %u\n", chs.chan1_raw);
						// 	printf("RC channel 2: %u\n", chs.chan2_raw);
						// 	printf("RC channel 3: %u\n", chs.chan3_raw);
						// 	printf("RC channel 4: %u\n", chs.chan4_raw);
						// 	printf("RC channel 5: %u\n", chs.chan5_raw);
						// 	printf("RC channel 6: %u\n", chs.chan6_raw);
						// 	printf("RC channel 7: %u\n", chs.chan7_raw);
						// 	printf("RC channel 8: %u\n", chs.chan8_raw);
						// 	printf("Receive siganl strength indicator: %u [units/scale]\n", chs.rssi); // Receive signal strength indicator in device-dependent units/scale. Values: [0-254], 255: invalid/unknown.
						// }
						default:
						{
							break;
						}
					} // switch ()
				} // if (mavlink_parse_char())
			} // for ()
		} // if (recsize > 0)
	} // while (1)
} // int main ()
