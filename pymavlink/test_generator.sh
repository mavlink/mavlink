#!/bin/bash

set -e
set -x

test -z "$MDEF" && MDEF="../message_definitions"

MAVLINK_DIALECT=ardupilotmega python setup.py clean build install --user
mavgen.py --lang C $MDEF/v1.0/ardupilotmega.xml -o generator/C/include_v1.0 --wire-protocol=1.0
mavgen.py --lang C $MDEF/v1.0/ardupilotmega.xml -o generator/C/include_v2.0 --wire-protocol=2.0
pushd generator/C/test/posix
make clean testmav1.0_common testmav2.0_common testmav1.0_ardupilotmega testmav2.0_ardupilotmega
./testmav1.0_common
./testmav2.0_common
./testmav1.0_ardupilotmega
./testmav2.0_ardupilotmega

