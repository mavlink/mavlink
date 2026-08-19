#!/usr/bin/env bash
set -e
set -o pipefail

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Initialize variables
mode="format"
xml_dir="message_definitions"
keep_old=0

show_help() {
    cat <<EOF
Usage: $(basename "$0") [OPTIONS] [xml_file]

Format or check formatting of MAVLink XML definition files using xmllint.

Options:
  -h        Show this help message and exit.
  -c        Check mode: report files that need formatting instead of
            formatting them in place.
  -d DIR    XML directory to process (default: message_definitions).
  -o        Keep a .old backup of each file that is reformatted.

If xml_file is given, only that file (relative to DIR) is processed;
otherwise all *.xml files under DIR are processed.
EOF
}

while getopts ":hcd:o" opt; do
    case "$opt" in
    h)
        show_help
        exit 0
        ;;
    \?)
        echo "Error: Invalid option -$OPTARG" >&2
        show_help
        exit 1
        ;;
    :)
        echo "Error: Option -$OPTARG requires an argument." >&2
        show_help
        exit 1
        ;;
    c)  mode="check"
        ;;
    d)  xml_dir=${OPTARG}
        ;;
    o)  keep_old=1
        ;;
    esac
done

shift $(($OPTIND - 1))

xml_file="$1"

if [ "$xml_file" == "" ]
then
    xml_files=$(find $xml_dir -name "*.xml")
else
    xml_files="$xml_dir/$xml_file"
fi
echo "processing file(s):"
echo "$xml_files"

ret=0
for f in $xml_files
do
    xmllint -format "${f}" | sed 's/[[:blank:]]*$//' > "${f}".new
    case "$mode" in
    format)
        if ! cmp "${f}" "${f}".new >/dev/null 2>&1
        then
            echo "formatting $f"
            if [ $keep_old -eq 1 ]
            then
                cp "${f}" "${f}".old
            fi
            cp "${f}".new "${f}"
        fi
        ;;
    check)
        if ! cmp "${f}" "${f}".new >/dev/null 2>&1
        then
            echo "file $f needs formatting - run ./scripts/format_xml.sh $f"
            ret=1
        fi
        ;;
    esac
    rm "${f}".new
done

exit $ret