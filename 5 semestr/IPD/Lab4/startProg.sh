#!/bin/bash
cd NonSilentMode
cmake .
make
export OPENCV_LOG_LEVEL=OFF
./main
echo "Clean files folder? (y\n)"
read -n 1 what
echo ""
if [[ $what == 'y' ]]; then
    rm -rf files
fi
rm main
cd ..