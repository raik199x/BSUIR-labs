# ComPortTalk

Checked by legendary man - Glezevich

QT + linux

---

For com-port emulation was used tool name tty0tty that [can be found in pca lab](https://github.com/raik199x/BSUIR-labs/tree/main/4%20semestr/PCA/Lab%201%20(tty0tty))

But bad thing about this emulator is that after changing number of bits in byte, text will not change, but Glez said that is okay (because it's not the students fault)

---

## How to configure

I woun't explain how to install tty0tty, but main problem that you face when you want to show your lab to Glez is a port numbering,

You must use ports x and x+1 for one client, y and y+1 for second client (ex. tty0 + tty1 --- tty2 + tty3) and you may ask: how they will talk?

(who did not understand, if we write in tnt0 port we will receive message in tnt1 port, but in the condition it must be the same client)

So one solution that i found is to create soft links like tnt0 + tnt3 for client 1 and tnt 2 + tnt 1 for client to:

    ln -s /dev/tnt0 tnt0
    ln -s /dev/tnt3 tnt1
    ln -s /dev/tnt2 tnt2
    ln -s /dev/tnt1 tnt3