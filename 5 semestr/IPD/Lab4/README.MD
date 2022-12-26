# Camera

Allows you to make photo + video in both silent and non silent modes.

Tested on linux Debian (Parrots Os, Pop os!, ubuntu) and arch (Monjaro).

---

# Dependencies

For getting info about webcam was used v4l (video4linux) api, you can find it in /sys/class/video4linux

install opencv using [this guide](https://www.geeksforgeeks.org/how-to-install-opencv-in-c-on-linux/)

you need to install ncruses libs

	sudo apt-get install libncurses5 libncurses5-dev

---

# Building

For NonSilentMode simply use *startProg.sh* script

For silent mode:

    cd SilentMode && cmake . && make
