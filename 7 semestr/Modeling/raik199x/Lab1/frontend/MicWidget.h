#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class MicWidget : public QWidget {
  Q_OBJECT

public:
  MicWidget(QWidget *parent = nullptr, size_t width = 0, size_t height = 0);
  ~MicWidget();

  // Method of inverse congruence
  QLineEdit *MicSeedLine;
  QLineEdit *MicMaskLine;

public slots:
  void on_SeedCheckBox_StateChanged(int state);
  // void on_StartRandom_clicked();
};