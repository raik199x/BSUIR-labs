all:
	gcc process.c functions.c -o start -W -Wall -Wno-unused-variable -std=gnu11 -pedantic
	gcc child.c functions.c -o child
clear:
	rm -rf start
	rm -rf proc_debug
	rm -rf child
debug:
	gcc process.c functions.c -o proc_debug -g
	gcc child.c functions.c -o child