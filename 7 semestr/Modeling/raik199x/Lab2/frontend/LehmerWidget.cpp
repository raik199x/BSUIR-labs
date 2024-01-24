#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>

#include "LehmerWidget.h"

LehmerWidget::LehmerWidget(QWidget *parent, size_t width, size_t height) : QWidget(parent) {
  this->setBaseSize(width, height);
  const QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("\\d{1,22}"));

  // Form for const A
  this->ConstALine = new QLineEdit;
  this->ConstALine->setValidator(validator);
  this->ConstALine->setText("6364136223846793005");

  QLabel *ConstA = new QLabel("Const A:");

  QFormLayout *ConstALayout = new QFormLayout;
  ConstALayout->addRow(ConstA, this->ConstALine);

  // Form for const M
  this->ConstMLine = new QLineEdit;
  this->ConstMLine->setValidator(validator);
  this->ConstMLine->setText("18446744073709551557");

  QLabel *ConstM = new QLabel("Const M:");

  QFormLayout *ConstMLayout = new QFormLayout;
  ConstMLayout->addRow(ConstM, this->ConstMLine);

  // Form for seed
  this->LehmerSeedLine = new QLineEdit;
  this->LehmerSeedLine->setValidator(validator);
  this->LehmerSeedLine->setText("123");
  QLabel *Seed = new QLabel("Seed");

  QHBoxLayout *upperSeedLayout = new QHBoxLayout;
  upperSeedLayout->addWidget(Seed);
  upperSeedLayout->addWidget(this->LehmerSeedLine);

  QVBoxLayout *LehmerAlgorithm = new QVBoxLayout;
  LehmerAlgorithm->addLayout(ConstALayout);
  LehmerAlgorithm->addLayout(ConstMLayout);
  LehmerAlgorithm->addLayout(upperSeedLayout);
  LehmerAlgorithm->addSpacerItem(new QSpacerItem(0, 30, QSizePolicy::Minimum, QSizePolicy::Expanding));

  // Whole Lehmer
  QWidget *LehmerWidget = new QWidget(this);
  LehmerWidget->setMinimumSize(width, height);
  LehmerWidget->setLayout(LehmerAlgorithm);
}

LehmerWidget::~LehmerWidget() {
}

void LehmerWidget::on_SeedCheckBox_StateChanged(int state) {
  this->LehmerSeedLine->setDisabled(state == Qt::Checked);
}