#include <cmath>
#include <random>

#include "Generators.h"

Distributions::Distributions(){};
Distributions::~Distributions(){};

double *Distributions::uniform_distribution(const size_t SelectionSize, const uint64_t *Selection) {
  double *resultSelection = new double[SelectionSize];

#pragma omp parallel for
  for (size_t i = 0; i < SelectionSize; i++) {
    resultSelection[i] = static_cast<double>(Selection[i]) / UINT64_MAX;
  }

  return resultSelection;
}

double *Distributions::normal_aka_gaussian_distribution(const size_t SelectionSize, const uint64_t *Selection) {
  double *resultSelection = new double[SelectionSize];

  uint8_t n = 6; // 6 : 12
#pragma omp parallel for
  for (int i = 0; i < SelectionSize; i++) {
    resultSelection[i] = 0;
    for (int j = i; j < i + n; j++) {
      resultSelection[i] += Selection[j] - 3;
    }
    resultSelection[i] *= sqrt(2);
    resultSelection[i] /= UINT64_MAX;
  }

  return resultSelection;
}

double *Distributions::exponential_distribution(const size_t SelectionSize, const uint64_t *Selection,
                                                const double lambda) {
  double *resultSelection = new double[SelectionSize];

#pragma omp parallel for
  for (size_t i = 0; i < SelectionSize; i++) {
    double R           = static_cast<double>(Selection[i]) / UINT64_MAX; // Converting to [0, 1]
    resultSelection[i] = -1.0 / lambda * log(R);
  }

  return resultSelection;
}

double *Distributions::gamma_distribution(const size_t SelectionSize, const uint64_t *Selection) {
  double *resultSelection = new double[SelectionSize];

  double  lambda = 1.4;
  uint8_t steps  = 7;

#pragma omp parallel for
  for (size_t i = 0; i < SelectionSize; i++) {
    double acc = 1.0;
    for (uint8_t step = 0; step < steps; step++) {
      acc *= static_cast<double>(Selection[i + step]) / UINT64_MAX;
    }
    resultSelection[i] = (-1.0 / lambda) * std::log(acc);
    if (resultSelection[i] > 20)
      resultSelection[i] = 20;
  }

  return resultSelection;
}

double *Distributions::triangular_distribution(const size_t SelectionSize, const uint64_t *Selection) {
  double *resultSelection = new double[SelectionSize];

#pragma omp parallel for
  for (size_t i = 0; i < SelectionSize; i += 2) {
    double r1              = static_cast<double>(Selection[i]) / UINT64_MAX;
    double r2              = static_cast<double>(Selection[i + 1]) / UINT64_MAX;
    resultSelection[i]     = r1 > r2 ? r1 : r2;
    resultSelection[i + 1] = r1 > r2 ? r1 : r2;
  }

  return resultSelection;
}

double *Distributions::simpson_distribution(const size_t SelectionSize, const uint64_t *Selection) {
  double *resultSelection = new double[SelectionSize];

#pragma omp parallel for
  for (size_t i = 0; i < SelectionSize; i++) {
    double U1          = static_cast<double>(Selection[i]) / UINT64_MAX;
    double U2          = static_cast<double>(Selection[(i + 1) % SelectionSize]) / UINT64_MAX;
    resultSelection[i] = U1 + U2;
  }

  return resultSelection;
}