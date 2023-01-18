#!/bin/bash
if [ $# = 0 ]; then 
    echo "please use either the build or test argument"
    exit
fi

absolute_path="$( cd -- "$(dirname -- "$0")"; pwd )"

if [ $1 = 'build' ]; then
    BOARD=""
    if [ $# !=  2 ]; then
        BOARD=$2
    else
        BOARD="nucleol552zeq"
    fi
    docker run -v $absolute_path/../:/project snaccoverflow/build-zp:latest -p $BOARD
else
        docker run -v $absolute_path/../:/project snaccoverflow/test-zp:latest
fi