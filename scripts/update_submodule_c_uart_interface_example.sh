#!/bin/bash

# Update Submodule c_library in mavlink/c_uart_interface_example
# Author: Stefan Weber <stefan-weber.k@gmx.ch>
#
# This script can be used together with a github webhook to automatically
# update the submodule c_library in mavlink/c_uart_interface_example and push
# to repository whenever the mavlink message specifications are updated.

# settings
C_UART_INTERFACE_EXAMPLE_GIT_REMOTENAME=origin
C_UART_INTERFACE_EXAMPLE_GIT_BRANCHNAME=master
C_UART_INTERFACE_EXAMPLE_PATH=$PWD/c_uart_interface_example
SUBMODULE_PATH=mavlink/include/mavlink/v1.0

# clone 
git clone --recursive https://github.com/mavlink/c_uart_interface_example.git
cd c_uart_interface_example
git submodule foreach git pull origin master

# save git hash of updated submodule
cd $SUBMODULE_PATH
MAVLINK_GITHASH=$(git rev-parse HEAD)

# change back to main git repo, add and commit changes
cd $C_UART_INTERFACE_EXAMPLE_PATH
git add --all :/ || exit 1
COMMIT_MESSAGE="Mavlink c_library Submodule updated to rev https://github.com/mavlink/c_library/tree/"$MAVLINK_GITHASH
git commit -m "$COMMIT_MESSAGE" || exit 1

# push to c_uart_interface_example repository
git push $C_UART_INTERFACE_EXAMPLE_GIT_REMOTENAME $C_UART_INTERFACE_EXAMPLE_GIT_BRANCHNAME || exit 1
echo -e "\0033[34mMavlink c_library Submodule updated and pushed successfully\0033[0m"

# delete c_uart_interface_example repository
rm -rf c_uart_interface_example
