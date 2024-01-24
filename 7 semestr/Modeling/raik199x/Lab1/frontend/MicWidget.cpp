#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QSpacerItem>

#include "MicWidget.h"

MicWidget::MicWidget(QWidget *parent, size_t width, size_t height) : QWidget(parent) {
  const QRegularExpressionValidator *validator =
      new QRegularExpressionValidator(QRegularExpression("\\d{1,10}"));
  MicSeedLine = new QLineEdit;
  MicSeedLine->setValidator(validator);

  MicMaskLine = new QLineEdit;
  MicMaskLine->setText("94165236");
  MicMaskLine->setValidator(validator);

  QLabel *MicSeed = new QLabel("Seed");
  QLabel *MicMask = new QLabel("Mask");

  QFormLayout *MicMaskLayout = new QFormLayout;
  MicMaskLayout->addRow(MicMask, MicMaskLine);

  QHBoxLayout *MicSeedSettings = new QHBoxLayout;
  MicSeedSettings->addWidget(MicSeed);
  MicSeedSettings->addWidget(this->MicSeedLine);

  // Right sight of Mic
  QVBoxLayout *MicCenter = new QVBoxLayout;
  MicCenter->addLayout(MicSeedSettings);
  MicCenter->addLayout(MicMaskLayout);

  // Whole Mic
  QWidget *MicWidget = new QWidget(this);
  MicWidget->setMinimumSize(width, height);
  MicWidget->setLayout(MicCenter);
}

MicWidget::~MicWidget() {
}

void MicWidget::on_SeedCheckBox_StateChanged(int state) {
  this->MicSeedLine->setDisabled(state == Qt::Checked);
}