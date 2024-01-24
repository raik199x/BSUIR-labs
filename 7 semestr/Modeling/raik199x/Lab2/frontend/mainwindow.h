#pragma once

#include <QComboBox>
#include <QMainWindow>
#include <QPushButton>

#include "DistributionMenu.h"
#include "LehmerWidget.h"
#include "MathStats.h"
#include "MiddleSquaresWidget.h"
#include "SelectionSizeWidget.h"
#include "thirdParty/qcustomplot/qcustomplot.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  QComboBox *RandomGenerator = nullptr;

private:
  DistributionMenu    *DistributionMenuWidget        = nullptr;
  SelectionSize       *SelectionSizeWidget           = nullptr;
  LehmerWidget        *LehmerRandomizerWidget        = nullptr;
  MiddleSquaresWidget *MiddleSquaresRandomizerWidget = nullptr;
  MathStats           *MathematicalStats             = nullptr;

  QCustomPlot *HistogramPlot = nullptr;
  QPushButton *RandomButton  = nullptr;
  QPushButton *BuildPlot     = nullptr;

  QVBoxLayout *RightSide = nullptr;

  uint64_t *Selection          = nullptr;
  size_t    SelectionSizeValue = 0;

public slots:
  void on_ComboBox_currentIndexChanged(int index);
  void on_GenerateSelections_clicked();
  void on_buildPlot_clicked();
};