#!/bin/bash
OUTDIR=out
FILE=$1
shift;
mkdir -p $OUTDIR
#g++ --std=c++17 $FILE -o $OUTDIR/runner && ./$OUTDIR/runner $@
clang++ --std=c++17 -Wunused-variable $FILE -o $OUTDIR/runner && ./$OUTDIR/runner $@
