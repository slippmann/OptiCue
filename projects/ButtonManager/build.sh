if [ ! -d build/ ]; then
	mkdir build/
fi

cd build/
cmake .. -DCMAKE_TOOLCHAIN_FILE="../../tools/toolchain-rpi-cpp.cmake"
make
