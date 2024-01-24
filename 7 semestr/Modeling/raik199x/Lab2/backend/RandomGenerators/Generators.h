#pragma once

#include <cstdint>
#include <cstdio>

#include <QMessageBox>

class Distributions {
public:
  Distributions();
  ~Distributions();

  double *uniform_distribution(const size_t SelectionSize, const uint64_t *Selection);
  double *normal_aka_gaussian_distribution(const size_t SelectionSize, const uint64_t *Selection);
  double *exponential_distribution(const size_t SelectionSize, const uint64_t *Selection, const double lambda);
  double *gamma_distribution(const size_t SelectionSize, const uint64_t *Selection);
  double *triangular_distribution(const size_t SelectionSize, const uint64_t *Selection);
  double *simpson_distribution(const size_t SelectionSize, const uint64_t *Selection);
};

uint64_t *LehmerGenerator(size_t AmountOfNumbers, uint64_t seed, uint64_t m, uint64_t a);
uint64_t *RandomMiddleSquaresNumbers(size_t AmountOfNumbers, uint64_t seed);