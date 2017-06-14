# Pymavlink
This is a python implementation of the MAVLink protocol.


# Documentation

Please see http://www.qgroundcontrol.org/mavlink/pymavlink for
documentation

# Installation 

## Recommendation

Pymavlink is currently a Python 2 package. It is recommended to install and use it with Python 2. 

However an initial Python 3 support is given
The following instruction assume your are using Python 2 and an Ubuntu installation

## Dependencies

Pymavlink requires two dependencies :

    - future : for python 2 and python 3 interoperability
    - lxml : for checking and parsing xml file

### On linux

Using pip you can install the required dependencies

```bash
sudo pip2 install -r requirements.txt
```

Or
```bash
sudo pip2 install -U future lxml
```

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

### For developpers

On the pymavlink directory, prefer use of
```bash
sudo pip2 install .
```
instead of
```bash
sudo python2 setup.py install
```

That way pip should auto install dependencies and allow to keep them up-to-date with pip. 

### Advance usage

Please see Pip documentation : https://pip.pypa.io/en/stable/

# License
-------

pymavlink is released under the GNU Lesser General Public License v3 or later

Join the chat at https://gitter.im/ArduPilot/pymavlink
