#!/bin/bash

# This requires the python future library
# On MS windows, cygwin usually does not provide that.
# The workaround is then to issue on a cygwin prompt:
#   easy_install-2.7 pip
#   pip install future

set -e
set -x

test -z "$MDEF" && MDEF="../message_definitions"

# MAVLINK_DIALECT=ardupilotmega python setup.py clean build install --user

mavgen.py --lang C $MDEF/v1.0/ardupilotmega.xml -o generator/C/include_v1.0 --wire-protocol=1.0
mavgen.py --lang C $MDEF/v1.0/ardupilotmega.xml -o generator/C/include_v2.0 --wire-protocol=2.0

mavgen.py --lang C++11 $MDEF/v1.0/ardupilotmega.xml -o generator/CPP11/include_v2.0 --wire-protocol=2.0

pushd generator/C/test/posix
make clean testmav1.0_common testmav2.0_common testmav1.0_ardupilotmega testmav2.0_ardupilotmega
./testmav1.0_common
./testmav2.0_common
./testmav1.0_ardupilotmega
./testmav2.0_ardupilotmega
popd

pushd generator/CPP11/test/posix
make clean all
popd
