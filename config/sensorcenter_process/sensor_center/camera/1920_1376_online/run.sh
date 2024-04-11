#!/system/bin/sh

export LOGLEVEL=4
cp ./libisx031.so.1.0.1 /userdata/mount/sensorlib/
./rwdir /system/lib/sensorlib/

grep "hobot.socver=5.2.0" /proc/cmdline >/dev/null 2>&1
if [[ $? -eq 0 ]]
then
echo "J5 2.0 Board Test"
export LD_LIBRARY_PATH=/map/longshui/tools/:$LD_LIBRARY_PATH
else
echo "J5 1.0 Board Test"
export LD_LIBRARY_PATH=/map/longshui/tools/:$LD_LIBRARY_PATH
fi


/map/longshui/tools/vpm_gtest            \
    -v "./vpm_config.json"    \
    -S 1                                     \
    -e 0                                     \
    -r 9999999                                  \
    -k 0                                     \
    -c "./hb_j5dev.json"      \
    -M 15                                    \
    --gtest_filter=VpmScenarioTest.multi_streams_pym	





