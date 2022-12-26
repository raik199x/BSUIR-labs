# Info about serivce

how to manually install (sudo needed):

    mkdir /opt/IPD_battery

    cp batteryUpTime.sh /opt/IPD_battery/

    cp IPD_batteryDaemon.service /etc/systemd/system/

    systemctl daemon-reload

    systemctl enable IPD_batteryDaemon.service

    systemctl start IPD_batteryDaemon.service

how to manually uninstall (sudo needed):

    systemctl stop IPD_batteryDaemon.service

    systemctl disable IPD_batteryDaemon.service

    rm -rf /opt/IPD_battery /etc/systemd/system/IPD_batteryDaemon.service

    systemctl daemon-reload