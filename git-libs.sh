#!/bin/sh

DIR=$(pwd)

CMAKE_FILE="CMakeLists.txt"
C_TOOLCHAIN="toolchain-rpi.cmake"
CPP_TOOLCHAIN="toolchain-rpi-cpp.cmake"

git clone https://github.com/raspberrypi/tools

cd tools/

cat << EOM > $C_TOOLCHAIN
# Define our host system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)
# Define the cross compiler locations
SET(CMAKE_C_COMPILER   "$DIR/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc")
SET(CMAKE_CXX_COMPILER "$DIR/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc")
# Define the sysroot path for the RaspberryPi distribution in our tools folder 
SET(CMAKE_FIND_ROOT_PATH "$DIR/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot/")
# Use our definitions for compiler tools
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories only
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
add_definitions(-Wall)

EOM

cat << EOM > $CPP_TOOLCHAIN
# Define our host system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)
# Define the cross compiler locations
SET(CMAKE_C_COMPILER   "$DIR/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc")
SET(CMAKE_CXX_COMPILER "$DIR/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc")
# Define the sysroot path for the RaspberryPi distribution in our tools folder 
SET(CMAKE_FIND_ROOT_PATH "$DIR/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot/")
# Use our definitions for compiler tools
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories only
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
add_definitions(-Wall -std=c11)

EOM

cd ..

git clone git://git.drogon.net/wiringPi

cd wiringPi/wiringPi

cat << EOM > $CMAKE_FILE
cmake_minimum_required(VERSION 3.0)
# Have CMake find our pthreads library within our toolchain (required for this library)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
# add all the *.c files as sources
FILE(GLOB SRC_FILES *.c)
# make this output a shared library (with .so output)
add_library (wiringPi SHARED \${SRC_FILES})
# be sure to include the current source directory for header files
target_include_directories (wiringPi PUBLIC \${CMAKE_CURRENT_SOURCE_DIR})
# add the following required libraries:
# Threads, Math, Crypt, and RealTime
target_link_libraries(wiringPi \${CMAKE_THREAD_LIBS_INIT} crypt m rt)

EOM

if [ ! -d lib/ ]; then
	mkdir lib/
fi

cd lib/
cmake .. -DCMAKE_TOOLCHAIN_FILE="~/OptiCue/tools/toolchain-rpi.cmake"
make
