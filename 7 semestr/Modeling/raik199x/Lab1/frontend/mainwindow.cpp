#include <complex>
#include <ctime>

#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QTabWidget>

#include "NistOperationalFunctions/Operations.h"
#include "RandomGenerators/Generators.h"
#include "Threading.h"
#include "mainwindow.h"
#include "sharedVariables.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // Basic setup
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  this->setWindowTitle("Modeling Lab 1: Random numbers");
  this->setMinimumSize(WindowWidth, WindowHeight);

  // Combo box
  this->RandomGenerator = new QComboBox(this);
  this->RandomGenerator->addItem("Lehmer generator");
  this->RandomGenerator->addItem("Middle squares generator");
  this->RandomGenerator->addItem("Inverse control generator");
  connect(this->RandomGenerator, SIGNAL(currentIndexChanged(int)), this,
          SLOT(on_ComboBox_currentIndexChanged(int)));

  // Selection size
  this->SelectionSize = new QRadioButton *[this->AmountRadioButton];
  for (uint8_t i = 0; i < this->AmountRadioButton; i++) {
    this->SelectionSize[i] = new QRadioButton(this);
    connect(this->SelectionSize[i], &QRadioButton::clicked, this, &MainWindow::on_RadioButton_clicked);
    if (i == this->AmountRadioButton - 1) {
      this->SelectionSize[i]->setText("Custom");
      continue;
    }
    this->SelectionSize[i]->setText(QString::number(this->SValues[i]));
  }
  this->SelectionSize[0]->setChecked(true);
  this->SelectionSizeCustom = new QLineEdit(this);
  this->SelectionSizeCustom->setValidator(
      new QRegularExpressionValidator(QRegularExpression("[0-9]{1,9}"), this));
  this->SelectionSizeCustom->setEnabled(false);

  // Generate selections
  this->GenerateSelections = new QPushButton("Generate selection", this);
  this->GenerateSelections->setStyleSheet("QPushButton { font-size: 15px; }");
  connect(this->GenerateSelections, &QPushButton::clicked, this, &MainWindow::on_GenerateSelections_clicked);

  // Mathematical mean (not modifiable by user)
  this->MathematicalMean.MathematicalMean = new QLineEdit(this);
  this->MathematicalMean.MathematicalMean->setReadOnly(true);
  this->MathematicalMean.TheoreticalMean = new QLineEdit(this);
  this->MathematicalMean.TheoreticalMean->setReadOnly(true);

  // Dispersion (not modifiable by user)
  this->Dispersion.MathematicalDispersion = new QLineEdit(this);
  this->Dispersion.MathematicalDispersion->setReadOnly(true);
  this->Dispersion.TheoreticalDispersion = new QLineEdit(this);
  this->Dispersion.TheoreticalDispersion->setReadOnly(true);

  // Table
  this->Table = new TableWidget(this);

  // Random generators
  this->Lehmer = new LehmerWidget(this, WindowWidth / 2, WindowHeight / 3);
  this->Lehmer->setMinimumSize(WindowWidth / 2, WindowHeight / 3);

  this->MiddleSquares = new MiddleSquaresWidget(this, WindowWidth / 2, WindowHeight / 3 - 200);
  this->MiddleSquares->setMinimumSize(WindowWidth / 2, WindowHeight / 3);

  this->Mic = new MicWidget(this, WindowWidth / 2, WindowHeight / 3);
  this->Mic->setMinimumSize(WindowWidth / 2, WindowHeight / 3);

  // hind all random generators
  this->MiddleSquares->hide();
  this->Mic->hide();

  // Layouts
  QVBoxLayout *MainLayout  = new QVBoxLayout;
  QHBoxLayout *UpperLayout = new QHBoxLayout;
  QVBoxLayout *LeftLayout  = new QVBoxLayout;
  QVBoxLayout *RightLayout = new QVBoxLayout;
  BottomLayout             = new QHBoxLayout;

  UpperLayout->addLayout(LeftLayout);
  UpperLayout->addLayout(RightLayout);

  QHBoxLayout *RandomGeneratorLayout = new QHBoxLayout;
  RandomGeneratorLayout->addWidget(this->RandomGenerator);
  RandomGeneratorLayout->addWidget(this->GenerateSelections);
  LeftLayout->addLayout(RandomGeneratorLayout);
  QHBoxLayout *RadioButtonLayout = new QHBoxLayout;
  for (uint8_t i = 0; i < this->AmountRadioButton; i++) {
    RadioButtonLayout->addWidget(this->SelectionSize[i]);
  }
  QVBoxLayout *SelectionSizeLayout = new QVBoxLayout;
  SelectionSizeLayout->addLayout(RadioButtonLayout);
  SelectionSizeLayout->addWidget(this->SelectionSizeCustom);
  LeftLayout->addLayout(SelectionSizeLayout);

  // Random generators
  LeftLayout->addSpacerItem(new QSpacerItem(10, 50));
  LeftLayout->addWidget(this->Lehmer);
  LeftLayout->addWidget(this->MiddleSquares);
  LeftLayout->addWidget(this->Mic);

  // Mathematical mean
  QVBoxLayout *MathematicalMeanLayout = new QVBoxLayout;
  QHBoxLayout *MeanLabelsLayout       = new QHBoxLayout;
  MeanLabelsLayout->addWidget(new QLabel("Mathematical mean", this));
  MeanLabelsLayout->addWidget(new QLabel("Theoretical mean", this));
  MathematicalMeanLayout->addLayout(MeanLabelsLayout);
  QHBoxLayout *MeanValuesLayout = new QHBoxLayout;
  MeanValuesLayout->addWidget(this->MathematicalMean.MathematicalMean);
  MeanValuesLayout->addWidget(this->MathematicalMean.TheoreticalMean);
  MathematicalMeanLayout->addLayout(MeanValuesLayout);
  LeftLayout->addLayout(MathematicalMeanLayout);

  // Dispersion
  QVBoxLayout *DispersionLayout       = new QVBoxLayout;
  QHBoxLayout *DispersionLabelsLayout = new QHBoxLayout;
  DispersionLabelsLayout->addWidget(new QLabel("Mathematical dispersion", this));
  DispersionLabelsLayout->addWidget(new QLabel("Theoretical dispersion", this));
  DispersionLayout->addLayout(DispersionLabelsLayout);
  QHBoxLayout *DispersionValuesLayout = new QHBoxLayout;
  DispersionValuesLayout->addWidget(this->Dispersion.MathematicalDispersion);
  DispersionValuesLayout->addWidget(this->Dispersion.TheoreticalDispersion);
  DispersionLayout->addLayout(DispersionValuesLayout);
  LeftLayout->addLayout(DispersionLayout);

  // Right side
  QHBoxLayout *AmountOfBlocksLayout = new QHBoxLayout;
  AmountOfBlocksLayout->addWidget(new QLabel("Amount of blocks", this));
  this->AmountOfBlocks = new QSpinBox(this);
  this->AmountOfBlocks->setMinimum(1);
  this->AmountOfBlocks->setMaximum(500);
  this->AmountOfBlocks->setValue(1);
  this->AmountOfBlocks->setStyleSheet("QSpinBox { font-size: 15px; }");
  AmountOfBlocksLayout->addWidget(this->AmountOfBlocks);
  RightLayout->addLayout(AmountOfBlocksLayout);

  QHBoxLayout *ChooseFileLayout = new QHBoxLayout;
  ChooseFileLayout->addWidget(new QLabel("Choose template file", this));
  this->ChooseFile = new QPushButton("Choose file", this);
  connect(this->ChooseFile, &QPushButton::clicked, this, &MainWindow::on_ChooseFile_clicked);
  this->ChooseFile->setStyleSheet("QPushButton { font-size: 14px; }");
  ChooseFileLayout->addWidget(this->ChooseFile);
  RightLayout->addLayout(ChooseFileLayout);

  RightLayout->addWidget(this->Table);
  this->Table->setMinimumSize(WindowWidth / 2, WindowHeight / 2);

  this->RunAll = new QPushButton("Run all", this);
  connect(this->RunAll, &QPushButton::clicked, this, &MainWindow::on_RunAll_clicked);
  this->RunAll->setStyleSheet("QPushButton { font-size: 15px; }");
  RightLayout->addWidget(this->RunAll);

  // Bottom side
  this->HistogramPlot = GetEmptyPlot("Histogram", "Value", "Frequency");
  BottomLayout->addWidget(this->HistogramPlot);

  MainLayout->addLayout(UpperLayout);
  MainLayout->addLayout(BottomLayout);

  centralWidget->setLayout(MainLayout);
}

MainWindow::~MainWindow() {
}

void MainWindow::on_GenerateSelections_clicked() {
  // Getting amount of numbers
  size_t AmountOfNumbers = 0;
  for (uint8_t i = 0; i < this->AmountRadioButton; i++) {
    if (this->SelectionSize[i]->isChecked()) {
      if (i == this->AmountRadioButton - 1) {
        AmountOfNumbers = this->SelectionSizeCustom->text().toULongLong();
        break;
      }
      AmountOfNumbers = this->SValues[i];
      break;
    }
  }
  if (AmountOfNumbers == 0) {
    QMessageBox::critical(this, "Error", "Amount of numbers is 0");
    return;
  }
  if (this->GeneratedSelection != nullptr) {
    delete[] this->GeneratedSelection;
    this->GeneratedSelection = nullptr;
  }

  this->GeneratedSelectionSize = AmountOfNumbers;
  uint64_t seed                = 0;
  switch (this->RandomGenerator->currentIndex()) {
  case 0: {
    // getting needed values
    uint64_t m = this->Lehmer->ConstMLine->text().toULongLong();
    uint64_t a = this->Lehmer->ConstALine->text().toULongLong();
    seed       = this->Lehmer->LehmerSeedLine->text().toULongLong();

    // checking values
    if (m == 0 || a == 0 || seed == 0) {
      QMessageBox::critical(this, "Error", "m, a, seed - one of the values is 0");
      return;
    }

    this->GeneratedSelection = RandomLehmerNumbers(AmountOfNumbers, seed, m, a);
    break;
  }
  case 1: {
    // getting needed values
    seed = this->MiddleSquares->SeedLine->text().toULongLong();

    // checking values
    if (seed == 0) {
      QMessageBox::critical(this, "Error", "seed - one of the values is 0");
      return;
    }

    this->GeneratedSelection = RandomMiddleSquaresNumbers(AmountOfNumbers, seed);
    break;
  }
  case 2: {
    // getting needed values
    seed                  = this->Mic->MicSeedLine->text().toULongLong();
    uint64_t feedbackMask = this->Mic->MicMaskLine->text().toULongLong();

    // checking values
    if (seed == 0 || feedbackMask == 0) {
      QMessageBox::critical(this, "Error", "seed, feedback mask - one of the values is 0");
      return;
    }

    this->GeneratedSelection = RandomMicNumbers(AmountOfNumbers, seed, feedbackMask);
    break;
  }
  default: {
    QMessageBox::critical(this, "Error", "Something went wrong");
    this->GeneratedSelectionSize = 0;
    return;
  }
  }

  // Calculate mean
  this->MathematicalMean.MathematicalMean->setText(
      QString::number(calculateMean(this->GeneratedSelection, this->GeneratedSelectionSize)));
  // Calculate dispersion
  this->Dispersion.MathematicalDispersion->setText(
      QString::number(calculateDispersion(this->GeneratedSelection, this->GeneratedSelectionSize,
                                          this->MathematicalMean.MathematicalMean->text().toDouble())));

  // Calculate theoretical values
  uint64_t *randomNumbers         = CplusplusNumbers(AmountOfNumbers, seed);
  double    theoreticalMean       = calculateMean(randomNumbers, AmountOfNumbers);
  double    theoreticalDispersion = calculateDispersion(randomNumbers, AmountOfNumbers, theoreticalMean);
  delete[] randomNumbers;
  this->Dispersion.TheoreticalDispersion->setText(QString::number(theoreticalDispersion));
  this->MathematicalMean.TheoreticalMean->setText(QString::number(theoreticalMean));

  // Output histogram
  delete this->HistogramPlot;
  this->HistogramPlot =
      BuildHistogramPlot(this->GeneratedSelection, this->GeneratedSelectionSize,
                         GetMinimalValue(this->GeneratedSelection, this->GeneratedSelectionSize),
                         GetMaximalValue(this->GeneratedSelection, this->GeneratedSelectionSize));

  UpdatePlot(this->HistogramPlot);
  this->UpdateNistStatuses();
}

void MainWindow::on_ComboBox_currentIndexChanged(int index) {
  this->Lehmer->hide();
  this->MiddleSquares->hide();
  this->Mic->hide();

  switch (index) {
  case 0: {
    this->Lehmer->show();
    break;
  }
  case 1: {
    this->MiddleSquares->show();
    break;
  }
  case 2: {
    this->Mic->show();
    break;
  }
  default: {
    QMessageBox::critical(this, "Error", "Something went wrong");
    return;
  }
  }
}

void MainWindow::on_RadioButton_clicked() {
  if (this->SelectionSize[this->AmountRadioButton - 1]->isChecked()) {
    this->SelectionSizeCustom->setEnabled(true);
    return;
  }
  this->SelectionSizeCustom->setEnabled(false);
}

void MainWindow::on_ChooseFile_clicked() {
  this->FilePath = QFileDialog::getOpenFileName(this, "Choose template file", "", "*.txt");
  if (this->FilePath.isEmpty()) {
    QMessageBox::critical(this, "Error", "File path is empty");
    return;
  }
  this->ChooseFile->setText(this->FilePath);
  this->UpdateNistStatuses();
}

void MainWindow::UpdateNistStatuses() {
  for (uint8_t i = 0; i < AmountOfNists; i++) {
    // if label text is Selection not generate, than we check if selection generated and then we update label
    if (this->Table->TestStatus[i]->text() == "Selection not generated") {
      if (this->GeneratedSelection == nullptr) {
        continue;
      }
      this->Table->TestStatus[i]->setText("Ready to launch");
      this->Table->TestStatus[i]->setStyleSheet("color: yellow");
    }

    if (this->Table->TestStatus[i]->text() == TextSuccess) {
      this->Table->TestStatus[i]->setStyleSheet("color: green");
    }
    if (this->Table->TestStatus[i]->text() == TextFailure) {
      this->Table->TestStatus[i]->setStyleSheet("color: red");
    }
  }

  // Last two nists must have template file
  for (uint8_t i = AmountOfNists - 2; i < AmountOfNists; i++) {
    if (this->Table->TestStatus[i]->text() == "Template file not setted" && !this->FilePath.isEmpty()) {
      this->Table->TestStatus[i]->setText("Ready to launch");
      this->Table->TestStatus[i]->setStyleSheet("color: yellow");
    } else if (this->Table->TestStatus[i]->text() != "Template file not setted" && this->FilePath.isEmpty()) {
      this->Table->TestStatus[i]->setText("Template file not setted");
      this->Table->TestStatus[i]->setStyleSheet("color: red");
    }
  }
}

void MainWindow::on_Thread_finished() {
  for (uint8_t i = 0; i < AmountOfNists; i++) {
    QString status;
    switch (this->ThreadBuffer[i]) {
    case 0:
      status = TextFailure;
      break;
    case 1:
      status = TextSuccess;
      break;
    case 2:
      status = this->Table->TestStatus[i]->text();
      break;
    default:
      QMessageBox::critical(this, "Error", "Internal error: unkown argument passed to a results");
      break;
    }
    this->Table->TestStatus[i]->setText(status);
  }
  this->threadBusy = false;
  this->UpdateNistStatuses();
}

void MainWindow::on_RunAll_clicked() {
  if (this->GeneratedSelection == nullptr) {
    QMessageBox::critical(this, "Error", "Selection not generated");
    return;
  }
  if (this->threadBusy) {
    QMessageBox::critical(this, "Error", "Wait until previous calculations are finished");
    return;
  }

  this->threadBusy = true;
  bool nistsToRun[AmountOfNists];

  for (uint8_t i = 0; i < AmountOfNists; i++) {
    if (this->Table->needRunTest[i]->isChecked()) {
      this->Table->TestStatus[i]->setText("Processing...");
      this->Table->TestStatus[i]->setStyleSheet("");
      nistsToRun[i] = true;
    } else {
      nistsToRun[i] = false;
    }
  }

  Threading *WorkerThread = new Threading(this, nistsToRun);
  connect(WorkerThread, &QThread::finished, this, &MainWindow::on_Thread_finished);
  WorkerThread->start();
}
