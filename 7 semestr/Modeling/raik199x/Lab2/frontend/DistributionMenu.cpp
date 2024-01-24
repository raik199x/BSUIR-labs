#include "DistributionMenu.h"
#include <QGridLayout>

DistributionMenu::DistributionMenu(size_t width, size_t height) {
  this->setMinimumSize(width, height);
  QGridLayout *MainLayout = new QGridLayout(this);

  this->distributionName = new QRadioButton *[this->numOfDistributions];
  for (uint8_t i = 0; i < this->numOfDistributions; i++) {
    this->distributionName[i] = new QRadioButton();
    this->distributionName[i]->setText(this->names[i]);
    MainLayout->addWidget(distributionName[i], i % 2, i % 3);
  }
  this->distributionName[0]->setChecked(true);
}

DistributionMenu::~DistributionMenu() {
}