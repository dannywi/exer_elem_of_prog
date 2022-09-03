#!/bin/bash
FILE=$1
shift;
g++ --std=c++17 $FILE -o build/runner && ./build/runner $@