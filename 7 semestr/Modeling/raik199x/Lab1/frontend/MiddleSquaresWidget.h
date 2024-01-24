#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class MiddleSquaresWidget : public QWidget {
  Q_OBJECT

public:
  MiddleSquaresWidget(QWidget *parent = nullptr, size_t width = 0, size_t height = 0);
  ~MiddleSquaresWidget();

  // Method of middle squares
  QLineEdit *SeedLine;

public slots:
  void on_SeedCheckBox_StateChanged(int state);
  // void on_RandomButton_clicked();
};