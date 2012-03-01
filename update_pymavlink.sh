#!/bin/bash
pymavlinkTag=master
topDir=$PWD
rm -rf pymavlink
git clone https://github.com/tridge/pymavlink.git
cp pymavlink/generator/message_definitions/v0.9/* message_definitions/v0.9
cp pymavlink/generator/message_definitions/v1.0/* message_definitions/v1.0
cd pymavlink && git checkout $pymavlinkTag && rm -rf .git

cd $topDir
patch -p1 < pymavlink.patch
