#pragma once

#include <QThread>

#include "mainwindow.h"
#include "sharedVariables.h"

class Threading : public QThread {
  Q_OBJECT
public:
  Threading(MainWindow *mainWindow, const bool TestsToRun[AmountOfNists]);

protected:
  void run() override;

private:
  bool        TestToRun[AmountOfNists];
  MainWindow *mainWindow;
};
