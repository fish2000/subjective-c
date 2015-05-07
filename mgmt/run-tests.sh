#!/usr/bin/env bash

echo "*** TEST RUN COMMENCING"

PROJECT_PATH="/Users/fish/Dropbox/libimread"

cd $PROJECT_PATH && \
    rm -rf ./build ./dist && \
    mkdir -p ./build ./dist && \
    cd ./build && \
    cmake .. -Wno-dev -DCMAKE_INSTALL_PREFIX=./dist && \
    make install && \
        ./test_libimread --success --durations yes --abortx 10

cd $PROJECT_PATH
echo "*** TEST RUN COMPLETE"