
#include "deviceinfo.h"

DeviceInfo::DeviceInfo(){}

void DeviceInfo::GetVolumes()
{
    this->volumes = QStorageInfo::mountedVolumes();
}

