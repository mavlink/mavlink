[![Build Status](https://travis-ci.org/mavlink/mavlink.svg?branch=master)](https://travis-ci.org/mavlink/mavlink)

## MAVLink ##

*   Official Website: http://mavlink.org
*   Source: [Mavlink Generator](https://github.com/mavlink/mavlink)
*   Binaries (always up-to-date from master):
  * [C/C++ header-only library](https://github.com/mavlink/c_library)
*   Mailing list: [Google Groups](http://groups.google.com/group/mavlink)

MAVLink -- Micro Air Vehicle Message Marshalling Library.

This is a library for lightweight communication between Micro Air Vehicles (swarm) and/or ground control stations. It allows for defining messages within XML files, which then are converted into appropriate source code for different languages. These XML files are called dialects, most of which build on the *common* dialect provided in `common.xml`.

The MAVLink protocol performs byte-level serialization and so is appropriate for use with any type of radio modem.

This repository is largely Python scripts that convert XML files into language-specific libraries. There are additional Python scripts providing examples and utilities for working with MAVLink data. These scripts, as well as the generated Python code for MAVLink dialects, require Python 2.7 or greater.

Note that there are two incompatible versions of the MAVLink protocol: v0.9 and v1.0. Most programs, including [QGroundControl](https://github.com/mavlink/qgroundcontrol), have switched over to v1.0. The v0.9 protocol is **DEPRECATED** and should only be used to maintain backwards compatibility where necessary.

### Requirements ###
  * Python 2.7+
    * Tkinter (if GUI functionality is desired)

### Installation ###
  1. Clone into a user-writable directory.
  2. Add the repository directory to your `PYTHONPATH`
  3. Generate MAVLink parser files following the instructions in the next section *AND/OR* run included helper scripts described in the Scripts/Examples secion.

### Generating Language-specific Source Files ###

Language-specific files can be generated via a Python script from the command line or using a GUI. If a dialect XML file has a dependency on another XML file, they must be located in the same directory. Since most MAVLink dialects depend on the **common** messageset, it is recommend that you place your dialect with the others in `/message_definitions/v1.0/`.

Available languages are:

  * C
  * C#
  * Java
  * JavaScript
  * Lua
  * Python, version 2.7+

#### From a GUI (recommended) ####

mavgenerate.py is a header generation tool GUI written in Python. It requires Tkinter, which is only included with Python on Windows, so it will need to be installed separately on non-Windows platforms. It can be run from anywhere using Python's -m argument:

    $ python -m mavgenerate

#### From the command line ####

mavgen.py is a command-line interface for generating a language-specific MAVLink library. This is actually the backend used by `mavgenerate.py`. After the `mavlink` directory has been added to the Python path, it can be run by executing from the command line:

    $ python -m pymavlink.tools.mavgen

### Usage ###

Using the generated MAVLink dialect libraries varies depending on the language, with language-specific details below:

#### C ####
To use MAVLink, include the *mavlink.h* header file in your project:

    #include <mavlink.h>

Do not include the individual message files. In some cases you will have to add the main folder to the include search path as well. To be safe, we recommend these flags:

    $ gcc -I mavlink/include -I mavlink/include/<your message set, e.g. common>

The C MAVLink library utilizes a channels metaphor to allow for simultaneous processing of multiple MAVLink streams in the same program. It is therefore important to use the correct channel for each operation as all receiving and transmitting functions provided by MAVLink require a channel. If only one MAVLink stream exists, channel 0 should be used by using the `MAVLINK_COMM_0` constant.

##### Receiving ######
MAVLink reception is then done using `mavlink_helpers.h:mavlink_parse_char()`.

##### Transmitting #####
Transmitting can be done by using the `mavlink_msg_*_pack()` function, where one is defined for every message. The packed message can then be serialized with `mavlink_helpers.h:mavlink_msg_to_send_buffer()` and then writing the resultant byte array out over the appropriate serial interface.

It is possible to simplify the above by writing wrappers around the transmitting/receiving code. A multi-byte writing macro can be defined, `MAVLINK_SEND_UART_BYTES()`, or a single-byte function can be defined, `comm_send_ch()`, that wrap the low-level driver for transmitting the data. If this is done, `MAVLINK_USE_CONVENIENCE_FUNCTIONS` must be defined.

### Scripts/Examples ###
This MAVLink library also comes with supporting libraries and scripts for using, manipulating, and parsing MAVLink streams within the pymavlink, pymav
link/tools, and pymavlink/examples directories.

The scripts have the following requirements:
  * Python 2.7+
  * mavlink repository folder in `PYTHONPATH`
  * Write access to the entire `mavlink` folder.
  * Your dialect's XML file is in `message_definitions/*/`

Running these scripts can be done by running Python with the '-m' switch, which indicates that the given script exists on the PYTHONPATH. This is the proper way to run Python scripts that are part of a library as per PEP-328 (and the rejected PEP-3122). The following code runs `mavlogdump.py` in `/pymavlink/tools/` on the recorded MAVLink stream `test_run.mavlink` (other scripts in `/tools` and `/scripts` can be run in a similar fashion):

    $ python -m pymavlink.tools.mavlogdump test_run.mavlink

### License ###

MAVLink is licensed under the terms of the Lesser General Public License (version 3) of the Free Software Foundation (LGPLv3). The C-language version of MAVLink is a header-only library, and as such compiling an application with it is considered "using the library", not a derived work. MAVLink can therefore be used without limits in any closed-source application without publishing the source code of the closed-source application.

See the *COPYING* file for more info.

### Credits ###

&copy; 2009-2014 [Lorenz Meier](mailto:mail@qgroundcontrol.org)
