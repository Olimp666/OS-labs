@echo off
mkdir build
cmake -B build -G "MinGW Makefiles"
cd build
mingw32-make