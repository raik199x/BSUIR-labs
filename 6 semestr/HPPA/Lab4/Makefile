CXX_Version=17
BuildFolder=build
# Architecture for MX130 (!)
ARCHITECTURE=--gpu-architecture=sm_50
Base_Build=nvcc --std=c++${CXX_Version} --compiler-bindir=/usr/bin/g++-10 src/* -O2

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
	./build/run default > temp
