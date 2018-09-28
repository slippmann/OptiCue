if [ ! -d build/ ]; then
	mkdir build/
fi

cd build/
cmake .. -DCMAKE_TOOLCHAIN_FILE="~/OptiCue/tools/toolchain-rpi-cpp.cmake"
make
