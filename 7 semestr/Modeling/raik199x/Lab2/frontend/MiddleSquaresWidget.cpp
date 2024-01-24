#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>

#include "MiddleSquaresWidget.h"

MiddleSquaresWidget::MiddleSquaresWidget(QWidget *parent, size_t width, size_t height) : QWidget(parent) {
  const QRegularExpressionValidator *validator =
      new QRegularExpressionValidator(QRegularExpression("\\d{1,19}"));

  this->SeedLine = new QLineEdit;
  this->SeedLine->setValidator(validator);
  this->SeedLine->setText("564613");

  QLabel *MiddleSquaresSeed = new QLabel("Seed");

  // Right side of middle squares

  QHBoxLayout *upperMiddleSquaresSeedLayout = new QHBoxLayout;
  upperMiddleSquaresSeedLayout->addWidget(MiddleSquaresSeed);
  upperMiddleSquaresSeedLayout->addWidget(this->SeedLine);

  // Whole middle squares
  QWidget *MiddleSquaresWidget = new QWidget(this);
  MiddleSquaresWidget->setMinimumSize(width, height);

  QHBoxLayout *MiddleSquaresAlgorithm = new QHBoxLayout();
  MiddleSquaresAlgorithm->addLayout(upperMiddleSquaresSeedLayout);
  MiddleSquaresWidget->setLayout(MiddleSquaresAlgorithm);
  MiddleSquaresAlgorithm->addSpacerItem(new QSpacerItem(0, 60, QSizePolicy::Minimum, QSizePolicy::Expanding));
  MiddleSquaresWidget->setLayout(MiddleSquaresAlgorithm);
}

MiddleSquaresWidget::~MiddleSquaresWidget() {
}

void MiddleSquaresWidget::on_SeedCheckBox_StateChanged(int state) {
  this->SeedLine->setDisabled(state == Qt::Checked);
}