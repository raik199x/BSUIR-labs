all:
	gcc main.c ../../../MyLibs/C_Lib/functions.c circle.c -o run --std=gnu11 -pthread
	./run
compile:
	gcc main.c ../../../MyLibs/C_Lib/functions.c circle.c -o run --std=gnu11 -pthread
debug:
	gcc main.c ../../../MyLibs/C_Lib/functions.c circle.c -o run --std=gnu11 -pthread -g
clear:
	rm -rf run