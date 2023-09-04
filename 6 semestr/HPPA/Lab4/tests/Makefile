help:
	cat Makefile

build:
	mkdir -p bin
	g++ -std=c++20 *.cpp -O2 -o bin/run -lcriterion

run:
	make build
	./bin/run --verbose -j1

clear:
	rm -rf bin
