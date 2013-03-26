#!/bin/sh

for protocol in 0.9 1.0; do
 for xml in ../../message_definitions/v$protocol/*.xml; do
     base=$(basename $xml .xml)
     mkdir -p javascript
     mkdir -p javascript/implementations
     ./mavgen.py --lang=javascript --wire-protocol=$protocol --output=javascript/implementations/mavlink_${base}_v$protocol.js $xml || exit 1
 done
done