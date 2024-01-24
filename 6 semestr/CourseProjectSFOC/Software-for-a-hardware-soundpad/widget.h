
#ifndef WIDGET_H
#define WIDGET_H

#include "deviceinfo.h"
#include "samplewidget.h"
#include <QDebug>
#include <QGridLayout>
#include <QMenu>
#include <QPushButton>
#include <QWidget>
#include <QSettings>



class Widget : public QWidget

{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    DeviceInfo * deviceInfo;
    QStorageInfo downloadDisk;
    QStorageInfo searchDisk;
    QPushButton *downloadMenuButton;
    QPushButton *searchMenuButton;
    QPushButton *buttons[4][4];


private:
    void SetBaseConfiguration(DeviceInfo * deviceInfo);

private slots:
    void RefreshDeviceButtonClicked();
    void SampleButtonClicked();
    void GlobalCheckClicked();
    void DownloadDiskActionTriggered();
    void SearchDiskActionTriggered();
};

#endif // WIDGET_H
