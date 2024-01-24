#pragma once

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QWidget>

#include "sharedVariables.h"

#define TextSuccess "Passed"
#define TextFailure "Failed"

class TableWidget : public QWidget {
  Q_OBJECT

public:
  TableWidget(QWidget *parent = nullptr);
  ~TableWidget();

  QVBoxLayout *MainLayout     = nullptr;
  QGridLayout *TableLayout    = nullptr;
  QSpinBox    *AmountOfBlocks = nullptr;
  QPushButton *ChooseFile     = nullptr;
  QCheckBox  **needRunTest      = nullptr;
  QLabel     **TestStatus     = nullptr;

private:
  QWidget      *Parent;
  const QString NistNames[AmountOfNists] = {"Frequency (monobit) test",
                                            "Frequency block test",
                                            "Test on selection of similar bits",
                                            "Test for the longest run of ones",
                                            "Test of matrix ranks",
                                            "Spectrum test",
                                            "Non-overlapping matching test templates",
                                            "Overlapping pattern matching test"};
};