#!/bin/bash
rm -rf CMakeFiles && rm CMakeCache.txt && rm Makefile \
&& cmake . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
&& cmake --build . --clean-first --verbose 
