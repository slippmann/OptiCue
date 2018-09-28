if [ ! -d build/ ]; then
	mkdir build/
fi

cd build/
cmake .. -DCMAKE_TOOLCHAIN_FILE="~/PiDev/tools/toolchain-rpi-cpp.cmake"
make
