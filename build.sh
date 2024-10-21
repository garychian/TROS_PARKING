#!/bin/bash
time=$(date "+%Y%m%d%H%M%S")
work_dir=$(pwd)

usage() {
  echo "usage: bash package.sh [all|proto|apa|clean] [release|debug] [deploy]"
  exit 1
}


# if [ $# -eq 0 ];then
#   usage
# fi

ALL="parking"
ARCH="j5"
COMPILE_MODE="Release"
DEPLOY_PATH=$(cat "deploy_path")
version_num=""

OutputDir=${work_dir}/output
LibDir=${work_dir}/output/parking/tros_common_lib

function build_clean() {
  cd ${work_dir}
  rm -rf build output
  rm -rf *.zip
}

function build_proto(){
  echo "fanya proto build begin"
  bash ${work_dir}/proto/fanya_proto/build.sh ${ARCH}
  echo "fanya proto build end"
}

function build_project(){
    echo ${ARCH}_conan
    echo  ${COMPILE_MODE}
    echo ${ALL}

    python3 scripts/build.py --config ${ARCH}_conan --build_type ${COMPILE_MODE}

    if [ $? -ne 0 ]; then
        echo "build failed"
        exit 1
    fi
    mv ${LibDir} ${work_dir}/output/
    package
}

generate_git_log() {
  echo 1 > ${OutputDir}/parking/commit_info
  echo ${version_num} > ${OutputDir}/parking/commit_info
  echo "" >> ${OutputDir}/parking/commit_info
  echo "Buid Repository" >> ${OutputDir}/parking/commit_info
  echo "branch:" >> ${OutputDir}/parking/commit_info

  if git rev-parse --is-inside-work-tree >/dev/null 2>&1 && git rev-parse --abbrev-ref HEAD >/dev/null 2>&1; then
    echo $(git rev-parse --abbrev-ref HEAD) >> ${OutputDir}/parking/commit_info
  fi
  echo "commit id:" >> ${OutputDir}/parking/commit_info
  if git rev-parse --is-inside-work-tree >/dev/null 2>&1 && git rev-parse HEAD >/dev/null 2>&1; then
    echo $(git rev-parse HEAD) >> ${OutputDir}/parking/commit_info
  fi
  echo "" >> ${OutputDir}/parking/commit_info
}


generate_version() {
  version=$(cat ${work_dir}/version.txt)
  IFS="=" read -ra arr <<< "$version"
  version=V${arr[1]}
  version_num=APP_${version}_${time}_$(echo ${ARCH} | tr '[:lower:]' '[:upper:]')
  echo ${version_num}_${COMPILE_MODE} > ${OutputDir}/parking/version
}

compress_package() {
  pack=${version_num}.zip
  cd ${work_dir}
  mkdir release
  cd release
  cp -rf ${OutputDir} ./

  mv output app

  ln -s ../tros_common_lib app/parking/tros_common_lib

  echo "package name: $pack"

  zip -y -r -q $pack *

  echo "package $pack completed"

  if [ ! -z "$DEPLOY_PATH" ]; then
      scp -o ConnectTimeout=20 $pack "$DEPLOY_PATH" && echo "transfer $pack to $DEPLOY_PATH successfully"
  fi

  mv $pack ../
  if [ $? = '0' ];then 
    rm -rf ${work_dir}/release
  fi
}

package() {
  generate_version
  generate_git_log
  compress_package
}


while [ $# -ne 0 ]
do
  if [ x$1 == x"all" ]; then
    ALL="all"
    echo "build all"
  elif [ x$1 == x"proto" ]; then
    echo "build proto file only"
    echo "begin build proto"
    ALL="proto"
  elif [ x$1 == x"release" ]; then
    COMPILE_MODE="Release"
  elif [ x$1 == x"debug" ]; then
    COMPILE_MODE="Debug"
  elif [ x$1 == x"clean" ]; then
    build_clean
    exit
  fi
  shift
done

if [[ ${ALL} == "all" ]]; then
    build_proto
fi

if [[ ${ALL} == "proto" ]]; then
    build_proto
    exit 0
fi
build_project

