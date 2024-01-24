#include <QHBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>

#include "SelectionSizeWidget.h"

SelectionSize::SelectionSize(size_t width, size_t height) {
  this->setMinimumSize(width, height);
  QVBoxLayout *MainLayout   = new QVBoxLayout(this);
  QHBoxLayout *RadioButtons = new QHBoxLayout();

  // Selection size
  this->Values = new QRadioButton *[this->AmountRadioButton];
  for (uint8_t i = 0; i < this->AmountRadioButton; i++) {
    this->Values[i] = new QRadioButton(this);
    RadioButtons->addWidget(this->Values[i]);
    connect(this->Values[i], &QRadioButton::clicked, this, &SelectionSize::on_RadioButton_clicked);
    if (i == this->AmountRadioButton - 1) {
      this->Values[i]->setText("Custom");
      continue;
    }
    this->Values[i]->setText(QString::number(this->SValues[i]));
  }
  this->Values[0]->setChecked(true);
  this->CustomValue = new QLineEdit(this);
  this->CustomValue->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{1,9}"), this));
  this->CustomValue->setEnabled(false);
  this->CustomValue->setText("0");
  connect(this->CustomValue, &QLineEdit::textChanged, this, &SelectionSize::on_LineEdit_Change);

  MainLayout->addLayout(RadioButtons);
  MainLayout->addWidget(this->CustomValue);
}

SelectionSize::~SelectionSize() {
}

void SelectionSize::on_RadioButton_clicked() {
  if (this->Values[this->AmountRadioButton - 1]->isChecked()) {
    this->CustomValue->setEnabled(true);
    return;
  }
  this->CustomValue->setEnabled(false);
}

void SelectionSize::on_LineEdit_Change() {
  bool result;
  this->CurrentSize = this->CustomValue->text().toULongLong(&result);
  if (!result) {
    this->CurrentSize = 0;
    this->CustomValue->setText("0");
  }
}