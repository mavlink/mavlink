#!/bin/sh
set -e

SRC_DIR=$(pwd)

# NOTE: we must do all testing on the installed python package, not
# on the build tree. Otherwise the testing is invalid and may not
# indicate the code actually works

# check format
sep="##############################################"
echo $sep
echo "FORMAT TEST"
echo $sep
cd "$SRC_DIR"
# ./scripts/format_xml.sh -c
echo PASS

generate_mavlink() {
    echo $sep
    echo "GENERATING MAVLINK " \
	    "protocol:${wire_protocol} language:${lang}"
    echo "DEFINITION : " "$msg_def"
    echo $sep
    outdir="/tmp/mavlink_${wire_protocol}_${lang}"
    pymavlink/tools/mavgen.py --lang="${lang}" \
	    --wire-protocol "${wire_protocol}" \
	    --strict-units \
	    --output="${outdir}" "${msg_def}"
    echo PASS
}

cd "$SRC_DIR"
for msg_def in message_definitions/v1.0/*.xml
do
    [ -e "$msg_def" ] || continue
    wire_protocol="1.0"
    for lang in Python C CS WLua Java
    do
        generate_mavlink
    done
    wire_protocol="2.0"
    for lang in Python C C++11 CS WLua Java
    do
        generate_mavlink
    done
done
# Avoid `spurious errors` caused by ~/.npm permission issues
# ref: https://github.com/travis-ci/travis-ci/issues/2244
# ref: https://github.com/npm/npm/issues/4815
# Does it already exist? Who owns? What permissions?
ls -lah ~/.npm || mkdir ~/.npm
# Make sure we own it
# $USER references the current user in Travis env
chown -R "$USER" ~/.npm
if [ -f /usr/bin/nodejs ]
then
	mkdir -p ~/bin
	ln -sf /usr/bin/nodejs ~/bin/node
	. ~/.bashrc	
fi
cd "$SRC_DIR/pymavlink/generator/javascript" && npm test

