#!/usr/bin/env bash

echo "*** SYNCING TEST PICTURE DATA TO ~/Pictures/libimread-test-data"

: ${THISDIR:=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)}
: ${PRAXONS:="${THISDIR}/praxons"}
# source "${PRAXONS}/anybar.sh"

PROJECT_PATH="/Users/fish/Dropbox/subjective-c"
EXTERNAL_PATH="/Users/fish/Pictures/libimread-test-data"

: ${COLOR_TRACE:="ON"}
: ${VERBOSE:="ON"}

pushd ${EXTERNAL_PATH}/..
    rm -rf subjective-c-test-data && \
    mkdir subjective-c-test-data
popd

pushd $PROJECT_PATH
    mcp "tests/data/*.*" "~/Pictures/subjective-c-test-data/#1.#2"
popd

echo "*** EXTERNAL SYNC COMPLETE"