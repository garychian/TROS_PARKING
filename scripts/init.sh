#!/bin/bash

echo ""
echo "parking."

show_usage() {
  echo "Usage: sh $0 [opt] [arg]"
  echo "  -m [mode] : 0: remote fillback, 1: online. 0 by default."
  echo "  -t [target to run]: 0: all processes, 1: apa_sc only, 0 by default."
  echo "  -h [show usuage]: display usage guide"
  echo "  no arg : run with default parameters: [mode=0][target=0]"
}

MODE=0
TARGET=0

while getopts "m:t:h" opt; do
  case $opt in
  m)
    MODE="$OPTARG"
    ;;
  t)
    TARGET="$OPTARG"
    ;;
  h)
    show_usage
    exit 0
    ;;
  ?)
    echo "Ignore unrecognized parameter: $OPTARG"
    ;;
  esac
done

PROCESS_SENSOR_CENTER="apa_sc"
PROCESS_DETECTION="parkingslot_detection_process"
PROCESS_LOCATION="location_map_process"
PROCESS_APAHANDLE="apahandle_process"
# kill before run
PID_SENSOR_CENTER=$(ps | grep $PROCESS_SENSOR_CENTER | grep -v grep | awk '{print $1}')
PID_DETECTION=$(ps | grep $PROCESS_DETECTION | grep -v grep | awk '{print $1}')
PID_LOCATION=$(ps | grep $PROCESS_LOCATION | grep -v grep | awk '{print $1}')
PID_APAHANDLE=$(ps | grep $PROCESS_APAHANDLE | grep -v grep | awk '{print $1}')

if [[ "$TARGET" == "0" ]];then
  kill -9 $PID_SENSOR_CENTER
  kill -9 $PID_DETECTION
  kill -9 $PID_LOCATION
  kill -9 $PID_APAHANDLE
elif [[ "$TARGET" == "1" ]];then
  kill -9 $PID_SENSOR_CENTER
else
  echo "invalid mode = $MODE, exit!"
  exit 1
fi

echo MODE=$MODE
echo TARGET=$TARGET

# dataflow require mainboard2 in PATH, when run program by launch2.
export PATH=.:$PATH
export LD_LIBRARY_PATH=libs:$LD_LIBRARY_PATH
export HB_DNN_LOG_LEVEL=3
export HB_EASY_DNN_LOG_LEVEL=3

# mainboard
MAINBOARD=mainboard2
# launch2
LAUNCH=launch2

chmod +x $MAINBOARD
chmod +x $LAUNCH

# do nothing for MODE field yet.
# only support remote fillback yet.


if [[ "$TARGET" == "0" ]];then
  ./$LAUNCH -c processes.json -w config
elif [[ "$TARGET" == "1" ]];then
  ./$MAINBOARD -c process.json -w config/$PROCESS_SENSOR_CENTER
else
  echo "invalid mode = $MODE, exit!"
  exit 1
fi




