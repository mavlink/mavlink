#!/bin/bash

# Do only build for Python 2.7
# as we only want to deploy for one
# unique generator.
PYTHONVER=`python -c 'import sys; print(".".join(map(str, sys.version_info[:3])))'`

if [[ $PYTHONVER != "2.7"* ]]
then
	echo -e "Skipping header generation for Python $PYTHONVER"
	exit 0
fi

# Do not build pull requests
if [[ $TRAVIS_PULL_REQUEST != "false" ]]
then
	exit 0
fi

# Do only build master branch
if [[ $TRAVIS_BRANCH != "master" ]]
then
	exit 0
fi

# Config for auto-building
git remote rename origin upstream
git config --global user.email "bot@pixhawk.org"
git config --global user.name "PX4BuildBot"
git config --global credential.helper "store --file=$HOME/.git-credentials"
echo "https://${GH_TOKEN}:@github.com" > $HOME/.git-credentials

# Build C library
mkdir -p include/mavlink/v1.0
cd include/mavlink/v1.0
git clone https://github.com/mavlink/c_library.git
cd ../../..
./scripts/update_c_library.sh

# XXX add build steps for other libraries as well
