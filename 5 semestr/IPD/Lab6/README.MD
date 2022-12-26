## Brief

Our implementation is BAD and "lazy" because we didnt actually coded something with bluetooth but used linux api for doing tasks. All because of pervious task that was actually impossible to do, so we were just lazy to do next task in normal way.

---

## Dependecies

Tested on Debian distros (Pop_os), does not work with VM's

bluetoothctl - main tool for all operations, should be installed by default by all distros

bluetooth-sendto - GNOME tool, if you use another graphical environment find solution for yourselves

---

## Known bug

When you first pair your devices, you need to open bluetooth settings so confirmation code would appear, if you dont do this bluetooth just shows you an error of confirmation code.
