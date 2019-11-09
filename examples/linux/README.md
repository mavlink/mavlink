# MAVLink UDP Quickstart Instructions

MAVLink UDP Example for *nix system (Linux, MacOS, BSD, etc.)

To compile with GCC, just enter:

```
gcc -std=c99 -I ../../include/common -o mavlink_udp mavlink_udp.c
```

The MAVLink header directory must be added to the include path, as shown above. 
Be sure to use version 1.0 of the MAVLink headers for this example, otherwise
the ground control software may be unable to connect.

To run, type:

```
./mavlink_udp
```

If you run *QGroundControl* on the same machine, checkout received message in MAVLink Inspector widget.
