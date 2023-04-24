#!/usr/bin/env bash

# This script generates the mavlink C library.

function generate_headers() {
python pymavlink/tools/mavgen.py \
    --output $CLIBRARY_PATH \
    --lang C \
    --wire-protocol $2.0 \
    message_definitions/v1.0/$1.xml
}

MAVLINK_PATH=$PWD
CLIBRARY_PATH=$MAVLINK_PATH/include/mavlink/v$1.0/c_library_v$1

echo -e "\0033[34mStarting to generate c headers\0033[0m\n"
generate_headers development $1
generate_headers ardupilotmega $1
generate_headers matrixpilot $1
generate_headers ASLUAV $1
generate_headers standard $1
echo -e "\0033[34mFinished generating c headers\0033[0m\n"

