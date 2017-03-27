CFLAGS = -g -Wall -Werror -O0
TESTPROTOCOL = common
ALLPROTOCOLS = minimal test common pixhawk ardupilotmega slugs ualberta

all: 
	for p in ${ALLPROTOCOLS}; do make -f Makefile build TESTPROTOCOL=$$p; done

test:
	for p in ${ALLPROTOCOLS}; do make -f Makefile testprogs TESTPROTOCOL=$$p || exit 1; done

valgrindtest:
	for p in ${ALLPROTOCOLS}; do make -f Makefile valgrindprogs TESTPROTOCOL=$$p || exit 1; done

build: testmav2.0_${TESTPROTOCOL} testmav1.0_${TESTPROTOCOL}

testprogs: testmav2.0_${TESTPROTOCOL} testmav1.0_${TESTPROTOCOL}
	./testmav2.0_${TESTPROTOCOL}
	./testmav1.0_${TESTPROTOCOL}

valgrindprogs: testmav2.0_${TESTPROTOCOL} testmav1.0_${TESTPROTOCOL}
	valgrind -q ./testmav2.0_${TESTPROTOCOL}
	valgrind -q ./testmav1.0_${TESTPROTOCOL}

clean:
	rm -rf *.o *~ testmav1.0* testmav2.0* sha256_test

testmav1.0_${TESTPROTOCOL}: testmav.c $(COMMON)
	$(CC) $(CFLAGS) -I../../include_v1.0 -I../../include_v1.0/${TESTPROTOCOL} -o $@ testmav.c

testmav2.0_${TESTPROTOCOL}: testmav.c
	$(CC) $(CFLAGS) -I../../include_v2.0 -I../../include_v2.0/${TESTPROTOCOL} -o $@ testmav.c

testmav1.0_ardupilotmega: testmav.c
	$(CC) $(CFLAGS) -I../../include_v1.0 -I../../include_v1.0/ardupilotmega -o $@ testmav.c

testmav2.0_ardupilotmega: testmav.c
	$(CC) $(CFLAGS) -I../../include_v2.0 -I../../include_v2.0/ardupilotmega -o $@ testmav.c

sha256_test: sha256_test.c
	$(CC) $(CFLAGS) -I../../include_v2.0 -o $@ sha256_test.c
