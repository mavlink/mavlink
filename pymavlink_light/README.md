[![Build Status](https://travis-ci.org/ArduPilot/pymavlink.svg?branch=master)](https://travis-ci.org/ArduPilot/pymavlink)
# Pymavlink
This is a python implementation of the MAVLink protocol.

# Documentation

Please see http://ardupilot.org/dev/docs/mavlink-commands.html for documentation.

For realtime discussion please see the pymavlink gitter channel here
https://gitter.im/ArduPilot/pymavlink


# Installation 

## Recommendation

Pymavlink is currently a Python 2 package. It is recommended to install and use it with Python 2. 

However an initial Python 3 support is given.
The following instructions assume you are using Python 2 and a Debian-based (like Ubuntu) installation.

## Dependencies

Pymavlink requires 3 dependencies :

    - future : for python 2 and python 3 interoperability (http://python-future.org/)
    - lxml : for checking and parsing xml file (http://lxml.de/installation.html)
    - python-dev : for mavnative

### On linux

lxml has some additional dependencies that can be installed with your package manager (here with apt-get) :

```bash
sudo apt-get install libxml2-dev libxslt-dev python-dev

```

Using pip you can install the required dependencies for pymavlink :

```bash
sudo pip2 install -U future lxml
```

The -U parameter allows updating future and lxml version if it is already installed.

### On Windows

Use pip to install future as for linux.
Lxml can be installed with a windows installer from here : https://pypi.python.org/pypi/lxml/3.6.0


## Installation

### For users

It is recommended to install pymavlink from PyPi with pip, that way dependencies should be auto install by pip.
```bash
sudo pip2 install -U pymavlink
```
The -U parameter allow to update pymavlink version if it is already installed.

### For developers

On the pymavlink directory, you can use :
```bash
sudo MDEF=PATH_TO_message_definitions pip2 install . -v
```
The -v parameter will output the installation commands on the terminal.
The MDEF usage is require as pip install is done from /tmp directory, so it is necessary to use MDEF variable to 
point on the message_definitions directory.
Use pip should auto install dependencies and allow to keep them up-to-date with pip. 

Or 
```bash
sudo python2 setup.py install
```


### Advance usage

Please see Pip documentation : https://pip.pypa.io/en/stable/

# License
-------

pymavlink is released under the GNU Lesser General Public License v3 or later.

Join the chat at https://gitter.im/ArduPilot/pymavlink
