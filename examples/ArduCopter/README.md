# MAVLink - ArduCopter SITL Example

## First Time Setup
1. In ***/mavlink/*** directory, create a *include* folder.
   ```
   mkdir include
   ```

1. In ***/mavlink/include/*** directory, clone the MAVLink C / C++ library [v2](https://github.com/mavlink/c_library_v2) (or [v1](https://github.com/mavlink/c_library_v1)).

## Running the example
1. Open a terminal and browse to the ***/mavlink/examples/ArduCopter/*** directory.
1. Enter the command (to compile with GCC):
   ```
   gcc -std=c99 -I ../../include/common -o mavlink_copter mavlink_copter.c
   ```
1. Type and run the executable:
   ```
   ./mavlink_copter
   ```
1. Run the ArduCopter SITL (usually use this command):
   ```
   sim_vehicle.py --map --console
   ```

For more information regarding how to install, build, and run the ArduCopter SITL, please see [the link](https://ardupilot.org/dev/docs/sitl-simulator-software-in-the-loop.html).

The expected output will be:
```
...
Time boot: XXXXXX
Latitude:  XXXXXX
Longitude: XXXXXX
Relative altitude: XXXXXX
vx: XXXXXX
vy: XXXXXX
vz: XXXXXX
...
Time boot: XXXXXX
Latitude:  XXXXXX
Longitude: XXXXXX
Relative altitude: XXXXXX
vx: XXXXXX
vy: XXXXXX
vz: XXXXXX
...
```
