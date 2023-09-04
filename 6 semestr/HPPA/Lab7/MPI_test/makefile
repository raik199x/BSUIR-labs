BuildFolder=build
Base_Build=mpic++ src/*.cpp


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
