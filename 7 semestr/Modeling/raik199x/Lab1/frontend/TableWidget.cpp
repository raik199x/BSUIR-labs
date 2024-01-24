#include "TableWidget.h"
#include "sharedVariables.h"

TableWidget::TableWidget(QWidget *parent) {
  this->TableLayout = new QGridLayout();
  this->MainLayout  = new QVBoxLayout(this);
  // this->setLayout(MainLayout);
  this->setMinimumSize(WindowWidth / 2, WindowHeight);

  // Setting table names
  QLabel *testNameLabel   = new QLabel("Test name");
  QLabel *testStatusLabel = new QLabel("Test status");
  QLabel *nrTestLabel     = new QLabel("Need to run");

  const QString columnQLabelStyleSheet = "QLabel { font-size: 18px; font: Bold }";
  testNameLabel->setStyleSheet(columnQLabelStyleSheet);
  testStatusLabel->setStyleSheet(columnQLabelStyleSheet);
  nrTestLabel->setStyleSheet(columnQLabelStyleSheet);

  TableLayout->addWidget(testNameLabel, 0, 0);
  TableLayout->addWidget(testStatusLabel, 0, 1, Qt::AlignCenter);
  TableLayout->addWidget(nrTestLabel, 0, 2, Qt::AlignCenter);

  this->TestStatus  = new QLabel *[AmountOfNists];
  this->needRunTest = new QCheckBox *[AmountOfNists];
  for (uint8_t i = 0; i < AmountOfNists; i++) {
    this->TableLayout->addWidget(new QLabel(this->NistNames[i]), i + 1, 0);

    this->TestStatus[i] = new QLabel("Selection not generated");
    this->TestStatus[i]->setStyleSheet("QLabel { color : red; }");
    this->TableLayout->addWidget(this->TestStatus[i], i + 1, 1, Qt::AlignCenter);

    this->needRunTest[i] = new QCheckBox("");
    this->needRunTest[i]->setStyleSheet("QCheckBox { background-color : black }");
    this->needRunTest[i]->setChecked(true);
    this->TableLayout->addWidget(this->needRunTest[i], i + 1, 2, Qt::AlignCenter);
  }

  this->MainLayout->addLayout(TableLayout);

  this->Parent = parent;
}

TableWidget::~TableWidget() {
}