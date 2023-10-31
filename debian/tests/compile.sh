#!/bin/sh

set -e
set -u

CC=$1

if [ "$(dpkg --print-architecture)" = "armhf" ]; then
    CXXFLAGS="-mfloat-abi=hard"
else
    CXXFLAGS=""
fi

cp debian/tests/test.cpp "$AUTOPKGTEST_TMP"
cd "$AUTOPKGTEST_TMP"
${CC} $CXXFLAGS test.cpp -o test
./test

