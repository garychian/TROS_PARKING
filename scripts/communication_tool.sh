#!/bin/sh
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./tool/communication_tool "$@"