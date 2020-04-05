#!/bin/sh

set -e

CONF="${1:-build}"
echo "Running dosbox -conf /build/tools/dosbox-$CONF.conf"
dosbox -conf "/build/tools/dosbox-$CONF.conf"
cat /build/build/BUILD.LOG
exit $(head -c 1 /build/build/RESULT)
