#!/bin/bash

cd "$(dirname "$0")" || exit

#rm -rf build
mkdir -p build
cd build
cmake .. -DBUILD_TEST=TRUE
make
ctest
