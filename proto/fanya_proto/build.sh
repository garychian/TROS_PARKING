#!/bin/bash
pwd=$(pwd)
current_path=$(dirname "$(readlink -f "$0")")
cd ${current_path}
rm -rf build output
python tools/build.py  s32g
cd ${pwd}