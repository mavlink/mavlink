#!/usr/bin/env bash
# test_c_parser.sh
#
# Generate the C headers for the common dialect with both wire protocols
# (exactly like test.sh does via mavgen), build the parser behaviour tests
# against each of them, and run the binaries.
#
# The test sources live in pymavlink/generator/C/test/mutation_parser/.
#   parse_runtime_test.c        receiver side: mavlink_parse_char()
#   convenience_sender_test.c   sender side: convenience send functions
#
# run:  ./scripts/test_c_parser.sh
set -e

SEP="##############################################"

generate_mavlink() {
    outdir="/tmp/mavlink_cparser_${wire_protocol}_C"
    rm -rf "$outdir"
    # shellcheck disable=SC2086
    pymavlink/tools/mavgen.py --lang=C \
        --wire-protocol "${wire_protocol}" \
        --strict-units \
        --output="${outdir}" \
        message_definitions/v1.0/common.xml
}

run_receiver() {
    outdir="$1"
    extra="$2"
    label="$3"
    # shellcheck disable=SC2086
    $CC $CFLAGS $extra -I"${outdir}" \
        pymavlink/generator/C/test/mutation_parser/parse_runtime_test.c \
        -o "/tmp/mavlink_cparser_${label}"
    "/tmp/mavlink_cparser_${label}"
    echo "PASS receiver ${label}"
}

CC="${CC:-gcc}"
CFLAGS="-Wall -Werror -O0 -Wno-address-of-packed-member -Wno-unused-function"

echo "${SEP}"
echo "PARSER RUNTIME TEST"
echo "${SEP}"

for wire_protocol in 1.0 2.0; do
    generate_mavlink
done

run_receiver "/tmp/mavlink_cparser_1.0_C" "" "1.0"
run_receiver "/tmp/mavlink_cparser_1.0_C" "-DMAVLINK_CHECK_MESSAGE_LENGTH" "1.0_length"
run_receiver "/tmp/mavlink_cparser_2.0_C" "" "2.0"
run_receiver "/tmp/mavlink_cparser_2.0_C" "-DMAVLINK_CHECK_MESSAGE_LENGTH" "2.0_length"

# sender side; the MAVLink 2 only assertions compile out on the 1.0 headers
$CC $CFLAGS -I"/tmp/mavlink_cparser_2.0_C" \
    pymavlink/generator/C/test/mutation_parser/convenience_sender_test.c \
    -o "/tmp/mavlink_cparser_sender"
/tmp/mavlink_cparser_sender
echo "PASS receiver sender"

echo "PASS"