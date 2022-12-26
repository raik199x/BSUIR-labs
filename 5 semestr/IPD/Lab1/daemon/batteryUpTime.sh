#!/bin/bash

while true; do
  batStatus=$(cat /sys/class/power_supply/BAT0/status)
  if [[ $batStatus == "Discharging" ]]; then
	if [[ ! -f "/opt/IPD_battery/lastDisBattery.txt" ]]; then
	  echo $(date +%s) > /opt/IPD_battery/lastDisBattery.txt
	fi
  elif [[ -f "/opt/IPD_battery/lastDisBattery.txt" ]]; then
	if [ $batStatus == "Charging" ] || [ $batStatus == "Full" ] || [ $batStatus == "Not charging" ]; then
	  rm /opt/IPD_battery/lastDisBattery.txt
	fi
  fi

  sleep 2
done
