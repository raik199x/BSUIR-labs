# List all PCI devices

| Information | |
| - | - |
| Year | 3 |
| Term | 5 |
| Lab | 2 |
| OS | Linux* |

 *- Tested for Manjaro Linux only, should work for other distributions as well.

# Short overview

This utility uses I/O-port to scan PCI and list all connected devices (almost, see the [Final Notice](#final-notice)).

To be able to run this app you will need to:

1. Build and load tCFX kernel module;
2. Create a filesystem node (`/dev/tcfx`);
3. Build and run app itself.

Optionally, you can update the database: pci.ids, which can be obtained [here](https://pci-ids.ucw.cz).

# tCFX Kernel Module

## Building and Loading

The trickiest part is to find out how to **display kernel logs**.

And the answer to this question depends on the distribution you use, so I'll leave it to those unfortunate, poor and miserable ones who will attempt to run this code on their machine.

If you use Manjaro Linux (*as I personally do*) this is your day: just type:

```
journalctl -f
```

You should see something like this:

```
Sep 26 20:39:58 <your_name> kernel: audit: type=1131 audit(1664213998.917:180): pid=1 uid=0 auid=4294967295 ses=4294967295 msg='unit=systemd-localed comm="systemd" exe="/usr/lib/systemd/systemd" hostname=? addr=? terminal=? res=success'
Sep 26 20:39:58 <your_name> audit[1]: SERVICE_STOP pid=1 uid=0 auid=4294967295 ses=4294967295 msg='unit=systemd-localed comm="systemd" exe="/usr/lib/systemd/systemd" hostname=? addr=? terminal=? res=success'
Sep 26 20:39:59 <your_name> audit: BPF prog-id=0 op=UNLOAD
Sep 26 20:39:59 <your_name> audit: BPF prog-id=0 op=UNLOAD
Sep 26 20:39:59 <your_name> audit: BPF prog-id=0 op=UNLOAD
Sep 26 20:39:59 <your_name> kernel: audit: type=1334 audit(1664213999.021:181): prog-id=0 op=UNLOAD
Sep 26 20:39:59 <your_name> kernel: audit: type=1334 audit(1664213999.021:182): prog-id=0 op=UNLOAD
Sep 26 20:39:59 <your_name> kernel: audit: type=1334 audit(1664213999.021:183): prog-id=0 op=UNLOAD
```

Install CMake, gcc and g++ if not installed yet.

Now you are ready to build the kernel module and load it. Go to `kernmod` directory and run:

```
make install
```

In the kernel logs you should see:

```
Sep 26 20:44:57 timurialvarez-haff kernel: tcfx module has been loaded.
Sep 26 20:44:57 timurialvarez-haff kernel: tcfx major value: <your_value>.
```

*Note: if you see no such messages, probably you are watching the wrong log.*

Now the module is loaded in the kernel. The value of `<your_value>` is **very important**: it will be used to create [a filesystem node](#configuring) to access the kernel module.

## Configuring

Still in the `kernmod` directory run:

```
make mount major=<your_value>
```

tCFX kernel module configuration is completed. If you have reached this point you have my honour!

*Note: all of these steps should be repeated each time you reboot you pc or reload the kernel module, keep that in mind.*

# Lab

## Building

```
g++ --std=c++20 main.cpp kernmod.cpp pcitable.cpp lpalette/lpalette.cpp -o lab2
```

## Running

```
./lab2
```

# Final Notice

## Hidden Devices

### aka Some Devices are Missing

Well the problem is using ports to reach the PCI. When the Linux starts it initializes all the devices, saves all the information about them to be used by the Linux API and then **powers some of them off** if there is actually no need for them to be active (e.g. discrete GPU), so the solution is very simple: don't use I/O-ports, be kind and always use the best technologies.

## Disclaimer

The author is not responsible for any of issues you can possibly face during the following this guide. Do everything at your own risk.

Good luck with reading my code.

Pull requests are welcome.

# Licencing

No license provided, except for those files that explicitly provide it.
