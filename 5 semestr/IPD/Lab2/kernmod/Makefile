obj-m += tcfxm.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	make
	sudo insmod tcfxm.ko

uninstall:
	make unmount
	sudo rmmod tcfxm

reinstall:
	make uninstall
	make install

major=0
mount:
	make unmount
	sudo mknod /dev/tcfx c $(major) 0
	make fix

fix:
	sudo chmod 666 /dev/tcfx

unmount:
	sudo rm -f /dev/tcfx

watch:
	journalctl -f
