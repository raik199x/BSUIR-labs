help:
	cat Makefile

BIN = ./bin
FFMPEG = -lavformat -lavcodec -lavutil -lswscale -lswresample -lavdevice -lavfilter -lpostproc

build:
	mkdir -p ${BIN}
	nvcc src/*.cu -o ${BIN}/run ${FFMPEG}

run:
	make build
	${BIN}/run ./res/sample.mkv ./res/output0.mkv

clean:
	rm -rf ${BIN}
