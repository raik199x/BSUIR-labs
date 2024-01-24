#include <QMessageBox>
#include <QVBoxLayout>

#include "OperationalFunctions/Operations.h"
#include "RandomGenerators/Generators.h"
#include "mainwindow.h"
#include "sharedVariables.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // Basic setup
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  this->setWindowTitle("Modeling Lab 2: Distributions");
  this->setMinimumSize(WindowWidth, WindowHeight);

  this->SelectionSizeWidget           = new SelectionSize(WindowWidth / 3, WindowHeight / 4);
  this->LehmerRandomizerWidget        = new LehmerWidget(this, WindowWidth / 3, WindowHeight / 4);
  this->MiddleSquaresRandomizerWidget = new MiddleSquaresWidget(this, WindowWidth / 3, WindowHeight / 4);

  // Combo box
  this->RandomGenerator = new QComboBox(this);
  this->RandomGenerator->addItem("Lehmer generator");
  this->RandomGenerator->addItem("Middle squares generator");
  connect(this->RandomGenerator, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ComboBox_currentIndexChanged(int)));

  QVBoxLayout *LeftSide = new QVBoxLayout();
  this->RandomButton    = new QPushButton("Generate selection");
  LeftSide->addWidget(this->RandomButton);
  connect(this->RandomButton, &QPushButton::clicked, this, &MainWindow::on_GenerateSelections_clicked);
  LeftSide->addWidget(this->SelectionSizeWidget);
  LeftSide->addWidget(this->RandomGenerator);
  LeftSide->addWidget(this->LehmerRandomizerWidget);
  LeftSide->addWidget(this->MiddleSquaresRandomizerWidget);
  this->BuildPlot = new QPushButton("Build histogram");
  connect(this->BuildPlot, &QPushButton::clicked, this, &MainWindow::on_buildPlot_clicked);
  LeftSide->addWidget(this->BuildPlot);
  this->DistributionMenuWidget = new DistributionMenu(WindowWidth / 3, WindowHeight / 5);
  LeftSide->addWidget(this->DistributionMenuWidget);
  this->MathematicalStats = new MathStats(this, WindowWidth / 3, WindowHeight / 4);
  LeftSide->addWidget(this->MathematicalStats);

  this->MiddleSquaresRandomizerWidget->hide();

  this->RightSide     = new QVBoxLayout();
  this->HistogramPlot = GetEmptyPlot("Histogram", "Value", "Amount");
  this->RightSide->addWidget(this->HistogramPlot);

  QHBoxLayout *MainLayout = new QHBoxLayout();
  MainLayout->addLayout(LeftSide);
  MainLayout->addLayout(RightSide);
  centralWidget->setLayout(MainLayout);
}

MainWindow::~MainWindow() {
}

void MainWindow::on_ComboBox_currentIndexChanged(int index) {
  this->LehmerRandomizerWidget->hide();
  this->MiddleSquaresRandomizerWidget->hide();

  switch (index) {
  case 0: {
    this->LehmerRandomizerWidget->show();
    break;
  }
  case 1: {
    this->MiddleSquaresRandomizerWidget->show();
    break;
  }
  default: {
    QMessageBox::critical(this, "Error", "Something went wrong");
    return;
  }
  }
}

void MainWindow::on_GenerateSelections_clicked() {
  // Getting amount of numbers
  size_t AmountOfNumbers = 0;
  for (uint8_t i = 0; i < this->SelectionSizeWidget->AmountRadioButton; i++) {
    if (this->SelectionSizeWidget->Values[i]->isChecked()) {
      if (i == this->SelectionSizeWidget->AmountRadioButton - 1) {
        AmountOfNumbers = this->SelectionSizeWidget->CustomValue->text().toULongLong();
        break;
      }
      AmountOfNumbers = this->SelectionSizeWidget->SValues[i];
      break;
    }
  }
  if (AmountOfNumbers == 0) {
    QMessageBox::critical(this, "Error", "Amount of numbers is 0");
    return;
  }
  if (this->Selection != nullptr) {
    delete[] this->Selection;
    this->Selection = nullptr;
  }

  this->SelectionSizeValue = AmountOfNumbers;
  uint64_t seed            = 0;
  switch (this->RandomGenerator->currentIndex()) {
  case 0: {
    // getting needed values
    uint64_t m = this->LehmerRandomizerWidget->ConstMLine->text().toULongLong();
    uint64_t a = this->LehmerRandomizerWidget->ConstALine->text().toULongLong();
    seed       = this->LehmerRandomizerWidget->LehmerSeedLine->text().toULongLong();

    // checking values
    if (m == 0 || a == 0 || seed == 0) {
      QMessageBox::critical(this, "Error", "m, a, seed - one of the values is 0");
      return;
    }

    this->Selection = LehmerGenerator(AmountOfNumbers, seed, m, a);
    break;
  }
  case 1: {
    // getting needed values
    seed = this->MiddleSquaresRandomizerWidget->SeedLine->text().toULongLong();

    // checking values
    if (seed == 0) {
      QMessageBox::critical(this, "Error", "seed - one of the values is 0");
      return;
    }

    this->Selection = RandomMiddleSquaresNumbers(AmountOfNumbers, seed);
    break;
  }
  default: {
    QMessageBox::critical(this, "Error", "Something went wrong");
    this->Selection = 0;
    return;
  }
  }
}

void MainWindow::on_buildPlot_clicked() {

  if (this->Selection == nullptr) {
    QMessageBox::warning(this, "Error", "generate selection first");
    return;
  }

  // Getting which radio button in distribution menu is checked
  uint8_t indexDistribution = 255;
  for (uint8_t i = 0; i < this->DistributionMenuWidget->numOfDistributions; i++) {
    if (this->DistributionMenuWidget->distributionName[i]->isChecked()) {
      indexDistribution = i;
      break;
    }
  }

  if (indexDistribution == 255) {
    QMessageBox::warning(this, "Error", "Unexpected value");
    return;
  }

  double       *BinarySelection = nullptr;
  Distributions generateDistribution;
  switch (indexDistribution) {
  case 0: {
    BinarySelection = generateDistribution.uniform_distribution(this->SelectionSizeValue, this->Selection);
    break;
  }
  case 1: {
    BinarySelection = generateDistribution.normal_aka_gaussian_distribution(this->SelectionSizeValue, this->Selection);
    break;
  }
  case 2: {
    double lambda   = 0.3;
    BinarySelection = generateDistribution.exponential_distribution(this->SelectionSizeValue, this->Selection, lambda);
    break;
  }
  case 3: {
    BinarySelection = generateDistribution.gamma_distribution(this->SelectionSizeValue, this->Selection);
    break;
  }
  case 4: {
    BinarySelection = generateDistribution.triangular_distribution(this->SelectionSizeValue, this->Selection);
    break;
  }
  case 5: {
    BinarySelection = generateDistribution.simpson_distribution(this->SelectionSizeValue, this->Selection);
    break;
  }
  default: {
    QMessageBox::warning(this, "Error", "Unexpected case");
    return;
  }
  }

  double MathMean = this->MathematicalStats->calculateMean<double>(BinarySelection, this->SelectionSizeValue);
  double disp =
      this->MathematicalStats->calculateDispersion<double>(BinarySelection, this->SelectionSizeValue, MathMean);

  this->MathematicalStats->MathematicalMean.MathematicalMean->setText(QString::number(MathMean));
  this->MathematicalStats->Dispersion.MathematicalDispersion->setText(QString::number(disp));

  double theoretical = std::round(MathMean * 100) / 100;
  if (theoretical == 0)
    theoretical++;

  this->MathematicalStats->MathematicalMean.TheoreticalMean->setText(QString::number(theoretical));

  theoretical = std::round(disp * 1000) / 1000;
  this->MathematicalStats->Dispersion.TheoreticalDispersion->setText(QString::number(theoretical));
#pragma omp parallel for
  for (size_t i = 0; i < this->SelectionSizeValue; i++)
    BinarySelection[i] *= 1000;

  this->RightSide->removeWidget(this->HistogramPlot);
  delete this->HistogramPlot;
  this->HistogramPlot = BuildHistogramPlot<double>(BinarySelection, this->SelectionSizeValue,
                                                   GetMinimalValue<double>(BinarySelection, this->SelectionSizeValue),
                                                   GetMaximalValue<double>(BinarySelection, this->SelectionSizeValue));
  this->RightSide->addWidget(this->HistogramPlot);
  delete[] BinarySelection;
}