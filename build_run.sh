#!/bin/bash
# More info on gdb: 
# http://marvin.cs.uidaho.edu/Teaching/CS445/debuggingSegFaults.html
export LDFLAGS="-Wl,--copy-dt-needed-entries"

params=$1

if [ ! -d "build" ] || 
  [ "$params" == "-clean" ] ||
  [ "$params" == "-debug -clean" ] ||
  [ "$params" == "-clean -debug" ] ; then
  rm -rf "build";
  mkdir "build";
  cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
  cd ..
else
  cd build 
  if [ -f "Voxel-Physics" ] ; then
    mv Voxel-Physics VOXEL-PREVIOUS-BUILD
  fi
  cd ..
fi

cp -a -r resources/ build/
cd build && make

if [ "$params" == "-debug" ] ||
  [ "$params" == "-debug -clean" ] ||
  [ "$params" == "-clean -debug" ] ; then
  if [ -f "Voxel-Physics" ] ; then
    gdb Voxel-Physics -q
  fi
else
  if [ -f "Voxel-Physics" ] ; then
    ./Voxel-Physics
  fi
fi

