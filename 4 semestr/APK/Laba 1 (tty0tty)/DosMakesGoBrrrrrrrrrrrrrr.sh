#!/bin/bash

echo "(m)ount or (u)nmount?"
read what


if [[ "$what" == "m" ]]; then
  sudo modprobe nbd
  #path to your vhd
  sudo qemu-nbd -c /dev/nbd0 /home/alexander/VirtualBox\ VMs/Dos/Dos.vhd
  #path to folder that will pretend to be a dos (to_send_dos for me)
  sudo mount /dev/nbd0p1 /home/alexander/to_send_dos
elif [[ "$what" == "u" ]]; then
  #path as in line 12
  sudo umount /home/alexander/to_send_dos
  sudo qemu-nbd -d /dev/nbd0
else
  echo "Error input"
fi
