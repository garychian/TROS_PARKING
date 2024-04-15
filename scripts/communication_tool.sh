#!/bin/sh
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./tools/communication_tool "$@"