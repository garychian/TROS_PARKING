#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
chmod +x publisher
./publisher "$@"