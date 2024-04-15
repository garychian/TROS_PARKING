#!/bin/bash
pwd=$(pwd)
current_path=$(dirname "$(readlink -f "$0")")
cd ${current_path}
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

# platform [linux | win64 | mac | aarch64_gcc9.3 | aarch64_gcc6.5]

platform="aarch64_gcc9.3"


if [[ ${ARCH} == "j5" ]]; then
    platform="aarch64_gcc9.3"
elif [[ ${ARCH} == "linux" ]]; then
    platform="linux"
else
    echo "unknow platform, compile proto failed"
    exit 1
fi

python tools/build.py  ${platform}

cd ${pwd}