#!/bin/bash

rm -rf build output

ARCH="j5"

usage() {
  echo "usage: bash package.sh [j3|j5|linux] [release|debug|clean]"
  exit 1
}

while [ $# -ne 0 ]
do
  if [ x$1 == x"j3" ]; then
    ARCH=$1
  elif [ x$1 == x"j5" ]; then
    ARCH=$1
  elif [ x$1 == x"linux" ]; then
    ARCH=$1
  elif [ x$1 == x"s32g" ]; then
    ARCH=$1
  elif [ x$1 == x"clean" ]; then
    build_clean
    exit
  fi
  shift
done

python3 scripts/build.py --config ${ARCH}_conan --build_type Release