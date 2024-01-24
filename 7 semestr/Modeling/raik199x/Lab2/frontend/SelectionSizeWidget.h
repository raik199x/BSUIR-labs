#pragma once

#include <QLineEdit>
#include <QRadioButton>
#include <QWidget>

class SelectionSize : public QWidget {
  Q_OBJECT

public:
  SelectionSize(size_t width = 0, size_t height = 0);
  ~SelectionSize();

  QRadioButton **Values      = nullptr;
  QLineEdit     *CustomValue = nullptr;

  size_t CurrentSize = SValues[0];

  size_t SValue1           = 10'000;
  size_t SValue2           = 500'000;
  size_t SValue3           = 100'000'000;
  size_t AmountRadioButton = 4;
  size_t SValues[3]        = {SValue1, SValue2, SValue3};

public slots:
  void on_RadioButton_clicked();
  void on_LineEdit_Change();
};