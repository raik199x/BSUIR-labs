BuildFolder=build
# Architecture for MX130 (!)
ARCHITECTURE=--gpu-architecture=sm_50
opencvLinker=-I/usr/local/include/opencv4 -L/usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
Base_Build=mpic++ src/*.cpp ${opencvLinker}


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
	mpirun -np 2 build/run
