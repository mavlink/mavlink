[![Build Status](https://travis-ci.org/ArduPilot/mavlink)](https://travis-ci.org/ArduPilot/mavlink)

## MAVLink ##

MAVLink -- Micro Air Vehicle Message Marshalling Library. The current protocol version is v2.

*   Development Website: http://dev.ardupilot.org
*   Source: [Mavlink Generator](https://github.com/ArduPilot/pymavlink)
*   Discussion: [Gitter Channel](http://gitter.im/ArduPilot/pymavlink)

This is a library for lightweight communication between drones and/or ground control stations. It allows for defining messages within XML files, which then are converted into appropriate source code for different languages. These XML files are called dialects, most of which build on the *common* dialect provided in `common.xml`.

The initial experimental MAVLink was created 2008 when the term drone was not used yet to describe small vehicles for consumer use.

The MAVLink protocol performs byte-level serialization and so is appropriate for use with any type of radio modem.

This repository is largely Python scripts that convert XML files into language-specific libraries. There are additional Python scripts providing examples and utilities for working with MAVLink data. These scripts, as well as the generated Python code for MAVLink dialects, require Python 2.7 or greater.

### Requirements ###
  * Python 2.7+
    * Tkinter (if GUI functionality is desired)

### Installation ###
  1. Clone into a user-writable directory. Make sure to use the git "--recursive" option since pymavlink is a submodule. Alternately, run "git submodule init" and "git submodule update" after cloning to get pymavlink.
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

mavgen.py is a command-line interface for generating a language-specific MAVLink library. This is actually the backend used by `mavgenerate.py`. After the `mavlink` directory has been added to the Python path, it can be run by executing from the command line. For example:

    $ python -m pymavlink.tools.mavgen --lang=C --wire-protocol=2.0 --output=generated/include/mavlink/v2.0 message_definitions/v1.0/common.xml

### Usage ###

Using the generated MAVLink dialect libraries varies depending on the language, with language-specific details below:

#### C ####
To use MAVLink, include the *mavlink.h* header file in your project:

    #include <mavlink/mavlink.h>

If headers for multiple dialects and/or versions are installed, your include path might instead look similar to the following:

    #include <mavlink/v2.0/common/mavlink.h>

*Do not include the individual message files.* If you generate your own headers, you will have to add their output location to your C compiler's search path. For the greatest compatiblity with existing code and examples, we recommend that you specify the top-level output directory AND all generated dialects and versions:

    $ gcc ... -I generated/include -I generated/include/mavlink/v2.0/common ...

##### Multiple streams, a.k.a. "channels" #####

The C MAVLink library utilizes a "channel" metaphor to allow for simultaneous processing of multiple, independent MAVLink streams in the same program. It is therefore important to use the correct channel for each operation as all receiving and transmitting functions provided by MAVLink require a channel. If only one MAVLink stream exists, channel 0 should be used by using the `MAVLINK_COMM_0` constant.

##### Receiving ######
MAVLink reception is then done using `mavlink_helpers.h:mavlink_parse_char()`.

##### Transmitting #####
Transmitting can be done by using the `mavlink_msg_*_pack()` function, where one is defined for every message. The packed message can then be serialized with `mavlink_helpers.h:mavlink_msg_to_send_buffer()` and then writing the resultant byte array out over the appropriate serial interface.

It is possible to simplify the above by writing wrappers around the transmitting/receiving code. A multi-byte writing macro can be defined, `MAVLINK_SEND_UART_BYTES()`, or a single-byte function can be defined, `comm_send_ch()`, that wrap the low-level driver for transmitting the data. If this is done, `MAVLINK_USE_CONVENIENCE_FUNCTIONS` must be defined.

#### TODO: other supported languages ####

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

MAVLink is licensed under the terms of the Lesser General Public License (version 3) of the Free Software Foundation (LGPLv3). The C-language version of MAVLink is a header-only library which is generated as MIT-licensed code. MAVLink can therefore be used without limits in any closed-source application without publishing the source code of the closed-source application.

See the *COPYING* file for more info.
