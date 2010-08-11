#include <mavlink.h>
#include <stdio.h>

int main(int argc, char* argv[])
{

	uint8_t bitfield = 254; // 11111110

	uint8_t mask = 128; // 10000000

	uint8_t result = (bitfield & mask);

	printf("0x%02x\n", bitfield);

	// Transform into network order

	generic_32bit bin;
	bin.i = 1;
	printf("First byte in (little endian) 0x%02x\n", bin.b[0]);
	generic_32bit bout;
	bout.b[0] = bin.b[3];
	bout.b[1] = bin.b[2];
	bout.b[2] = bin.b[1];
	bout.b[3] = bin.b[0];
	printf("Last byte out (big endian) 0x%02x\n", bout.b[3]);
}
