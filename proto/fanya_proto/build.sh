#!/bin/bash
pwd=$(pwd)
current_path=$(dirname "$(readlink -f "$0")")
cd ${current_path}
rm -rf build ouput
python tools/build.py  aarch64_gcc9.3
cd ${pwd}