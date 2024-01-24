#include <QtCore/QStorageInfo>
#include <QStorageInfo>
#include <QtCore/QDebug>

#pragma once




class DeviceInfo
{

public:

    DeviceInfo();
    void GetVolumes();
    QList<QStorageInfo> volumes;
};