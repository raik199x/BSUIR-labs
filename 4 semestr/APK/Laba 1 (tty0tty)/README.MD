This lab was written on linux mashine and it can be a bit difficult to make all things work, so here is a little guide

First, we need to download a com emulator on linux.
Since i use debian (also should work on ubuntu) [this](https://github.com/freemed/tty0tty) will perfectly fits me

> Great thanks to lcgamboa@yahoo.com

---

When all is done (and you can see all tnt) you can test them, for that:
  * open 2 terminals
  * in first write `cat /dev/tnt1`
  * in second write `echo "hello world" >> /dev/tnt0`

If you can see message in first terminal, you've done all good

---

Now we must configure virtual box.
Here is a little problem with dosbox, it dont want to communicate with bios 14h, only way (that i find) is to ~~delete dosbox~~ install virtual box.
Full guide how to install (and even mount disk, we will use it in future you can find [here](https://www.makeuseof.com/tag/run-ms-dos-games-apps-linux/))

Then, after you install virtual box (and dos) go to:
Settings->serial port->enable two ports->in each set port mode to "host device" and write /dev/tnt0(1)
Now ports are connected and ready to use

> Note: there is a bug, that after second start mashine the whole system can stuck, problem wasnt solve by me but i think it caused by tnt ports

---

Now we need to transfer file to mashine, use guide that told before.
All should work now.


Information about [bios 14h](http://www.codenet.ru/progr/dos/int_0013.php) THX to [Semyon Matusevich](https://github.com/Xotab413)

Information about [in\out](https://sysprog.ru/post/komandy-in-i-out) Also thx to [Semyon Matusevich](https://github.com/Xotab413)
