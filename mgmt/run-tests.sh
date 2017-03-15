#!/usr/bin/env bash

echo "*** TEST RUN COMMENCING"

: ${THISDIR:=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)}
: ${PRAXONS:="${THISDIR}/praxons"}
# source "${PRAXONS}/anybar.sh"
source "${PRAXONS}/gmalloc.sh"

PROJECT_PATH="/Users/fish/Dropbox/subjective-c"

pushd $PROJECT_PATH && \
    rm -rf ./build ./dist && \
    mkdir -p ./build ./dist && \
    pushd ./build && \
        cmake .. -Wno-dev -DCMAKE_INSTALL_PREFIX="${PROJECT_PATH}/dist" && \
        make install && \
            ./imread_tests --success --durations yes --abortx 10

# cd $PROJECT_PATH
popd
popd
echo "*** TEST RUN COMPLETE"