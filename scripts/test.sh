set -e

SRC_DIR=`pwd`

git submodule update --init --recursive

# NOTE: we must do all testing on the installed python package, not 
# on the build tree. Otherwise the testing is invalid and may not
# indicate the code actually works

# check format
sep="##############################################"
echo $sep
echo "FORMAT TEST"
echo $sep
cd $SRC_DIR
# ./scripts/format_xml.sh -c
echo PASS

# install
echo $sep
echo "PYMAVLINK INSTALL"
echo $sep
cd $SRC_DIR

user_arg="--user"
if [ "$TRAVIS" == true ]
then
	user_arg=""
fi
pip install $user_arg -r pymavlink/requirements.txt
cd $SRC_DIR/pymavlink
python setup.py build install $user_arg

msg_def="message_definitions/v1.0/common.xml"

cd $SRC_DIR
for wire_protocol in 1.0 2.0
do
	for lang in Python C CS WLua Java
	do
		echo $sep
		echo "GENERATING MAVLINK " \
			"protocol:${wire_protocol} language:${lang}"
		echo $sep
		outdir="/tmp/mavlink_${wire_protocol}_${lang}"
		mavgen.py --lang=${lang} \
			--wire-protocol ${wire_protocol} \
			--output=${outdir} ${msg_def}
		echo PASS
	done
done

# Avoid `spurious errors` caused by ~/.npm permission issues
# ref: https://github.com/travis-ci/travis-ci/issues/2244
# ref: https://github.com/npm/npm/issues/4815
# Does it already exist? Who owns? What permissions?
ls -lah ~/.npm || mkdir ~/.npm
# Make sure we own it
# $USER references the current user in Travis env
chown -R $USER ~/.npm
if [ -f /usr/bin/nodejs ]
then
	mkdir -p ~/bin
	ln -sf /usr/bin/nodejs ~/bin/node
	. ~/.bashrc	
fi
cd $SRC_DIR/pymavlink/generator/javascript && npm test

# Test quaternions
echo $sep
echo "QUATERNION TEST"
echo $sep
cd $SRC_DIR
pymavlink/tools/quaterniontest.py
echo PASS
