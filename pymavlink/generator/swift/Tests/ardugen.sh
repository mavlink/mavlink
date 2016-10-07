#!/bin/sh

export PYTHONPATH="$PYTHONPATH:../../../../"

python -m pymavlink.tools.mavgen ../../../../message_definitions/v1.0/ardupilotmega.xml -o ./MAVLink/MAVLink/ --lang Swift