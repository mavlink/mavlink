#!/bin/bash

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

git remote rename origin upstream
git config --global user.email "bot@pixhawk.org"
git config --global user.name "PX4BuildBot"
git config credential.helper "store --file=.git/credentials"
echo "https://${GH_TOKEN}:@github.com" > .git/credentials

# Build C library
mkdir -p include/mavlink/v1.0
cd include/mavlink/v1.0 && git clone https://github.com:mavlink/c_library.git
./scripts/update_c_library.sh

# XXX add build steps for other libraries as well
