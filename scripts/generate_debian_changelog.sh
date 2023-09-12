#!/usr/bin/env bash

set -e

# Date according to RFC 5322
DATE=$(date -R)
AUTHOR="MAVLink Team"
EMAIL=""

# Fetch tags from upstream in case they're not synced
git fetch --tags

# Get the tag which points to the current commit hash.
CURRENT_REF="$(git rev-parse --short HEAD)"

# dpkg-buildpacakge expects the version to start with a digit, hence the 0
VERSION="0v$CURRENT_REF"

function usage() {
cat << EOF
    Usage:
    $0 <options ...>
        --version <version core; default=0v[CURRENT_REF] or [CURRENT_TAG]>
        --pre <pre-release number; default=1>
        --author <author name; default=MAVSDK Team>
        --email <author email; default=[empty]>
    Examples:
        ./generate_debian_changelog.sh
            --version=1.0.0
            --pre=1
            --author="Example Name"
            --email="name@example.com"
EOF
}

function usage_and_exit() {
    usage
    exit $1
}

for i in "$@"
do
    case $i in
        --version=*)
        VERSION="${i#*=}"
        ;;
        --author=*)
        AUTHOR="${i#*=}"
        ;;
        --email=*)
        EMAIL="${i#*=}"
        ;;
        *) # unknown option
        usage_and_exit 1
        ;;
    esac
done

echo "mavlink ($VERSION) unstable; urgency=medium"
echo ""
echo "  * MAVLink release"
echo ""
echo " -- $AUTHOR <$EMAIL>  $DATE"
