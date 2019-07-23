#!/bin/bash
set -e

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Initialize variables
mode="format"

while getopts "h?c" opt; do
    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    c)  mode="check"
        ;;
    esac
done

xml_files=$(find . -name "*.xml")
ret=0
for  f in $xml_files
do
	xmllint -format "${f}" > "${f}".new
	case "$mode" in
	format)
		if ! cmp "${f}" "${f}".new >/dev/null 2>&1
		then
			echo "formatting $f"
			cp "${f}".new "${f}"
		fi
		;;
	check)
		if ! cmp "${f}" "${f}".new >/dev/null 2>&1
		then
			echo "$f needs formatting - run ./scripts/format_xml.sh $f"
			ret=1
		fi
		;;
	esac
	rm "${f}".new
done

exit $ret
