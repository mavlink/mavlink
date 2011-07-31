VERSION = 3.02
CC      = gcc
CFLAGS  = -I ../../include/common -Wall -Werror -DVERSION=\"$(VERSION)\"
LDFLAGS = ""

OBJ = udp.o

udptest: $(OBJ)
	$(CC) $(CFLAGS) -o udptest $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<