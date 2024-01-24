#pragma once

#include <QRadioButton>
#include <QString>
#include <QWidget>

class DistributionMenu : public QWidget {
  Q_OBJECT

public:
  DistributionMenu(size_t width = 0, size_t height = 0);
  ~DistributionMenu();

  QRadioButton **distributionName = nullptr;

  const static size_t numOfDistributions = 6;
  QString names[numOfDistributions]      = {"Uniform", "Gaussian", "Exponential", "Gamma", "Triangular", "Simpson"};
};