#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class LehmerWidget : public QWidget {
  Q_OBJECT

public:
  LehmerWidget(QWidget *parent = nullptr, size_t width = 0, size_t height = 0);
  ~LehmerWidget();

  QLineEdit *ConstALine;
  QLineEdit *ConstMLine;
  QLineEdit *LehmerSeedLine;

public slots:
  void on_SeedCheckBox_StateChanged(int state);
  // void on_RandomButton_clicked();
};