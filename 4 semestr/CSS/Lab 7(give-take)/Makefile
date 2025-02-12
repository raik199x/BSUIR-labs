all:
	gcc main.c old_functions/functions.c circle.c -o run --std=gnu11 -pthread -lrt
	gcc producer.c -o producer -pthread
	gcc taker.c -o taker -pthread
	./run
compile:
	gcc main.c old_functions/functions.c circle.c -o run --std=gnu11 -pthread -lrt
	gcc producer.c -o producer -pthread
	gcc taker.c -o taker -pthread
debug:
	gcc main.c old_functions/functions.c circle.c -o run -g --std=gnu11 -pthread -lrt
	gcc producer.c -o producer -g -pthread
	gcc taker.c -o taker -g -pthread
clear:
	rm -rf run
	rm -rf taker
	rm -rf producer