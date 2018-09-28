#!/bin/sh

CMAKE_FILE="CMakeLists.txt"

git clone https://github.com/raspberrypi/tools
git clone git://git.drogon.net/wiringPi

cd wiringPi/wiringPi

cat <<EOM >$CMAKE_FILE
cmake_minimum_required(VERSION 3.0)
# Have CMake find our pthreads library within our toolchain (required for this library)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
# add all the *.c files as sources
FILE(GLOB SRC_FILES *.c)
# make this output a shared library (with .so output)
add_library (wiringPi SHARED ${SRC_FILES})
# be sure to include the current source directory for header files
target_include_directories (wiringPi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# add the following required libraries:
# Threads, Math, Crypt, and RealTime
target_link_libraries(wiringPi ${CMAKE_THREAD_LIBS_INIT} crypt m rt)
EOM

if [ ! -d lib/ ]; then
	mkdir lib/
fi

cd lib/
cmake .. -DCMAKE_TOOLCHAIN_FILE="~/OptiCue/tools/toolchain-rpi.cmake"
make
