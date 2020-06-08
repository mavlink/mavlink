# MAVLink - ArduCopter SITL Example

# First Time Setup
1. In /mavlink/ directory, create a include folder.
'''
mkdir include
'''

2. In /include/ directory, clone the MAVLink C/C++ library v2 (or v1) from the links below.
https://github.com/mavlink/c_library_v1
https://github.com/mavlink/c_library_v2

# Running the example
1. Open a terminal and browse to the /mavlink/examples/ArduCopter/ directory.

2. Enter the command (to compile with GCC):
```
gcc -std=c99 -I ../../include/common -o mavlink_copter mavlink_copter.c
```

3. Type and run the executable:

```
./mavlink_copter
```

4. Install and run the ArduCopter SITL (usually use the command):
'''
sim_vehicle.py --map --console
'''

For more information regarding ArduCopter SITL, see the link below:
https://ardupilot.org/dev/docs/sitl-simulator-software-in-the-loop.html
