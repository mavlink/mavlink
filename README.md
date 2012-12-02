## MAVLink ##

*   Website: http://qgroundcontrol.org/mavlink/
*   Source: https://github.com/mavlink/mavlink
*   Mailing list: [Google Groups](http://groups.google.com/group/mavlink)

MAVLink -- Micro Air Vehicle Message Marshalling Library.

This is a library for lightweight communication between Micro Air Vehicles (swarm) and/or ground control stations.
It serializes C-structs for serial channels and can be used with any type of radio modem.

Messages definitions are created in XML, and then converted into C header files.

### Generating Headers ###

Header files can be generated either with mavgen, or within QGroundControl.

##### With mavgen  #####

mavgen is a header generation tool written in python, which is included with MAVLink. It can be used directly or via the *generator.py* GUI. To use the GUI, run:

    python generate.py

If you would rather use mavgen from the command line see *pymavlink\generator\mavgen.py*.

##### With QGroundControl #####

To generate/update packets, select *mavlink_standard_message.xml* in the QGroundControl station settings view, select *mavlink/include* as the output directory and click on "Save and Generate". You will find the newly *generated/updated message_xx.h* files in the *mavlink/include/generated* folder.

### Usage ###

To use MAVLink, include the *mavlink.h* header file in your project:

    #include <mavlink.h>
    
Do not include the individual message files. In some cases you will have to add the main folder to the include search path as well. To be safe, we recommend these flags:

    gcc -I mavlink/include -I mavlink/include/<your message set, e.g. common>

### License ###

MAVLink is licensed under the terms of the Lesser General Public License of the Free Software Foundation (LGPL). As MAVLink is a header-only library, compiling an application with it is considered "using the libary", not a derived work. MAVLink can therefore be used without limits in any closed-source application without publishing the source code of the closed-source application.

See the *COPYING* file for more info.

### Credits ###

&copy; 2009-2012 [Lorenz Meier](mailto:mail@qgroundcontrol.org)
