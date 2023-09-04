CXX_Version=17
BuildFolder=build
# Architecture for MX130 (!)
ARCHITECTURE=--gpu-architecture=sm_50
opencvLinker=-I/usr/local/include/opencv4 -L/usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
Base_Build=nvcc --std=c++${CXX_Version} --compiler-bindir=/usr/bin/g++-10 src/* ${opencvLinker} --disable-warnings -O2


all:
	mkdir -p ${BuildFolder}
	${Base_Build} -o build/run

debug:
	mkdir -p ${BuildFolder}
	${Base_Build} -g -o build/debug

clear:
	rm -rf ${BuildFolder}

run:
	make
	./build/run image.jpeg

build_sm_50:
	mkdir -p ${BuildFolder}
	${Base_Build} ${ARCHITECTURE} -o build/run_sm_50

run_sm_50:
	make build_sm_50
	./build/run_sm_50 image.jpeg
