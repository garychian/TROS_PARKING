#!/bin/sh
# export  SPI_HAL_DEBUG_LEVEL=4
chmod +x bin/vehicle_service
export LD_LIBRARY_PATH=../tros_common_lib:$LD_LIBRARY_PATH
./bin/vehicle_service config/user_config.json 3 
