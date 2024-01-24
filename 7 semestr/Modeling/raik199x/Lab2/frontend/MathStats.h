#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

class MathStats : public QWidget {
  Q_OBJECT

public:
  MathStats(QWidget *parent = nullptr, size_t width = 0, size_t height = 0) {
    this->MathematicalMean.MathematicalMean = new QLineEdit;
    this->MathematicalMean.TheoreticalMean  = new QLineEdit;
    this->Dispersion.MathematicalDispersion = new QLineEdit;
    this->Dispersion.TheoreticalDispersion  = new QLineEdit;

    this->MathematicalMean.MathematicalMean->setReadOnly(true);
    this->MathematicalMean.TheoreticalMean->setReadOnly(true);
    this->Dispersion.MathematicalDispersion->setReadOnly(true);
    this->Dispersion.TheoreticalDispersion->setReadOnly(true);

    // Layouts
    QHBoxLayout *MainLayout = new QHBoxLayout(this);

    QVBoxLayout *MeansLayout = new QVBoxLayout;
    MeansLayout->addWidget(new QLabel("Mathematical Mean"));
    MeansLayout->addWidget(this->MathematicalMean.MathematicalMean);
    MeansLayout->addWidget(new QLabel("Theoretical Mean"));
    MeansLayout->addWidget(this->MathematicalMean.TheoreticalMean);

    QVBoxLayout *DispersionLayout = new QVBoxLayout;
    DispersionLayout->addWidget(new QLabel("Dispersion real"));
    DispersionLayout->addWidget(this->Dispersion.MathematicalDispersion);
    DispersionLayout->addWidget(new QLabel("Dispersion theoretical"));
    DispersionLayout->addWidget(this->Dispersion.TheoreticalDispersion);

    MainLayout->addLayout(MeansLayout);
    MainLayout->addLayout(DispersionLayout);
  }
  ~MathStats() {
  }

  /**
   * @brief Calculates the mean of an array
   *
   * @param array selection
   * @param size number of elements in the selection
   * @return double mean
   */
  template <typename ReturnType, typename SelectionType>
  ReturnType calculateMean(const SelectionType *array, const size_t size) {
    ReturnType mean = 0;
    for (size_t i = 0; i < size; i++) {
      mean += array[i];
    }
    mean /= size;
    return mean;
  }

  /**
   * @brief Calculates the dispersion of an array
   *
   * @param array selection
   * @param size number of elements in the selection
   * @param mean mean of the selection
   * @return double dispersion
   */
  template <typename ReturnType, typename SelectionType, typename MeanType>
  ReturnType calculateDispersion(const SelectionType *array, size_t size, MeanType mean) {
    ReturnType dispersion = 0;
    for (size_t i = 0; i < size; i++) {
      dispersion += pow(array[i] - mean, 2);
    }
    dispersion /= size;
    return dispersion;
  }

  struct Means {
    QLineEdit *MathematicalMean = nullptr;
    QLineEdit *TheoreticalMean  = nullptr;
  } MathematicalMean;
  struct Dispersion {
    QLineEdit *MathematicalDispersion = nullptr;
    QLineEdit *TheoreticalDispersion  = nullptr;
  } Dispersion;

private:
};