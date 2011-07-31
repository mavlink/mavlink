MAVLINK_VERSION = 1.00
CC      = gcc
CFLAGS  = -I ../../include/common -Wall -Werror -DMAVLINK_VERSION=\"$(MAVLINK_VERSION)\" -std=C99
LDFLAGS = ""

OBJ = udp.o

udptest: $(OBJ)
	$(CC) $(CFLAGS) -o udptest $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<