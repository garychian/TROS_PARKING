#!/bin/bash

echo ""
echo "parking app."

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

PROCESS_SENSOR_CENTER="parking_sc"
PROCESS_DETECTION="parking_psd"
PROCESS_LOCATION="parking_loc"
PROCESS_APAHANDLE="parking_aph"
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
elif [[ "$TARGET" == "2" ]];then
  kill -9 $PID_APAHANDLE
elif [[ "$TARGET" == "3" ]];then
  kill -9 $PID_DETECTION
elif [[ "$TARGET" == "4" ]];then
  kill -9 $PID_LOCATION
else
  echo "invalid mode = $MODE, exit!"
  exit 1
fi

echo MODE=$MODE
echo TARGET=$TARGET

# dataflow require mainboard2 in PATH, when run program by launch2.
export PATH=.:$PATH
export LD_LIBRARY_PATH=tros_common_lib:$LD_LIBRARY_PATH
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
  ./$MAINBOARD -c process.json -w config/sensorcenter_process
elif [[ "$TARGET" == "2" ]];then
  ./$MAINBOARD -c process.json -w config/apahandle_process
elif [[ "$TARGET" == "3" ]];then
  ./$MAINBOARD -c process.json -w config/parkingslot_detection_process
elif [[ "$TARGET" == "4" ]];then
  ./$MAINBOARD -c process.json -w config/location_map_process
else
  echo "invalid mode = $MODE, exit!"
  exit 1
fi