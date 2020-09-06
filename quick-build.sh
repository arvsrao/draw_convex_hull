#!/usr/bin/env bash

DIR="build"

if [[ -d $DIR ]]; then
    echo 'deleting previous build directory' $DIR;
    rm -rf $DIR
fi

[[ ! -d $DIR ]] && echo $DIR "was deleted" 


mkdir build && cd build && cmake ../ && make
exit 0;


