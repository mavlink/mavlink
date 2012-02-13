#!/bin/bash
pymavlinkTag=d04af92f82f28d1ed66c933f08ef0494ae6ff693
rm -rf pymavlink
git clone https://github.com/tridge/pymavlink.git
cd pymavlink && git checkout $pymavlinkTag && rm -rf .git
