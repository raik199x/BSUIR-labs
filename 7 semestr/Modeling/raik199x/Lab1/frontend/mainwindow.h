#pragma once

#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>

#include "LehmerWidget.h"
#include "MicWidget.h"
#include "MiddleSquaresWidget.h"
#include "TableWidget.h"
#include "thirdParty/qcustomplot/qcustomplot.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  uint64_t *GeneratedSelection     = nullptr;
  unsigned  GeneratedSelectionSize = 0;

  // Left side
  QComboBox     *RandomGenerator     = nullptr;
  QRadioButton **SelectionSize       = nullptr;
  QLineEdit     *SelectionSizeCustom = nullptr;
  QPushButton   *GenerateSelections  = nullptr;
  struct Means {
    QLineEdit *MathematicalMean = nullptr;
    QLineEdit *TheoreticalMean  = nullptr;
  } MathematicalMean;
  struct Dispersion {
    QLineEdit *MathematicalDispersion = nullptr;
    QLineEdit *TheoreticalDispersion  = nullptr;
  } Dispersion;

  LehmerWidget        *Lehmer        = nullptr;
  MiddleSquaresWidget *MiddleSquares = nullptr;
  MicWidget           *Mic           = nullptr;

  // Right side (table)
  QPushButton *RunAll         = nullptr;
  QSpinBox    *AmountOfBlocks = nullptr;
  QPushButton *ChooseFile     = nullptr;
  QString      FilePath;
  TableWidget *Table = nullptr;

  // Bottom side
  QHBoxLayout *BottomLayout  = nullptr;
  QCustomPlot *HistogramPlot = nullptr;

  uint8_t ThreadBuffer[AmountOfNists];

public slots:
  void on_GenerateSelections_clicked();
  void on_ComboBox_currentIndexChanged(int index);
  void on_RadioButton_clicked();
  void on_ChooseFile_clicked();
  void on_RunAll_clicked();
  void on_Thread_finished();

private:
  size_t SValue1           = 10'000;
  size_t SValue2           = 500'000;
  size_t SValue3           = 100'000'000;
  size_t AmountRadioButton = 4;
  size_t SValues[3]        = {SValue1, SValue2, SValue3};
  bool   threadBusy        = false;

#define UpdatePlot(Plot)                  \
  this->BottomLayout->removeWidget(Plot); \
  this->BottomLayout->addWidget(Plot)

  void UpdateNistStatuses();
};
