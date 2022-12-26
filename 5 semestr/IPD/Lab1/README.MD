# Battery info

Program made by raik199x and BadWolf

---

### Info

All info about battery we take from:

     /sys/class/power_supply/{BAT0}
{} - means there can be multiple battery's


info about files (self-tested)

 * status - show inf
 * technology - shows info about battery type
 * energy_now + energy_full - shows info about current energy and maximum amount
 * status - show us info about current state of battery (Discharging, Full (also Not Charging), Charging)
 * uevent - collect all data mentioned before (and a bit more) and stores it into environment variables

We don't cover all files because we don't really needed them all, but you can explore it by yourself.

/sys/power/state -- shows all possible modes on out system (also used to set mode) where:

 * freeze - Suspend to idle
 * mem - Suspend to ram
 * disk - suspend to disk


Note also that it is thus possible to use two different ways to Suspend-To-Idle:

either by writing freeze to /sys/power/state, or by writing s2idle to /sys/power/mem_sleep and subsequently writing mem to /sys/power/state.

---

### Requirements

This program was tested on Debian (Parrot os) and Arch (Monjaro), but best compatibility was achieved by using debian.

**systemctl** - is needed to run self-made daemon (you can use **sevice** command, but you have to rewrite code a bit)  

**grep and cut** - simple parsing tools that should be on every distro.

**upower** - to get time before battery dies. (we tried to use **acpi** but on some laptops it just does not want to show time)

Also you should run program from folder where *daemon* folder is located, otherwise it could not be installed. To compile using make:

    g++ main.cpp lpalette/lpalette.cpp battery.cpp -o battery --std=c++17 -pthread

---

### Useful links

https://www.howtogeek.com/devops/how-to-hibernate-or-sleep-linux-from-the-command-line/

