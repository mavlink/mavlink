#!/usr/bin/env bash

# Build C library
GEN_START_PATH=$PWD
mkdir -p include/mavlink/v2.0
cd include/mavlink/v2.0
git clone git@github.com:aviant-tech/c_library_v2.git
cd ../../..
./scripts/update_c_library.sh 2
