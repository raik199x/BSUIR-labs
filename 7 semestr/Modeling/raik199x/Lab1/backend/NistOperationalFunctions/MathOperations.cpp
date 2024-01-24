#include "Operations.h"

/**
 * @brief Calculates the mean of an array
 *
 * @param array selection
 * @param size number of elements in the selection
 * @return double mean
 */
double calculateMean(const uint64_t *array, size_t size) {
  double mean = 0;
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
double calculateDispersion(const uint64_t *array, size_t size, double mean) {
  double dispersion = 0;
  for (size_t i = 0; i < size; i++) {
    dispersion += pow(array[i] - mean, 2);
  }
  dispersion /= size;
  return dispersion;
}

void FastFourierTransform(std::complex<double> *a, size_t n) {
  if (n <= 1) {
    return;
  }

  std::complex<double> *even = new std::complex<double>[n / 2];
  std::complex<double> *odd  = new std::complex<double>[n / 2];

  for (size_t i = 0; i < n / 2; ++i) {
    even[i] = a[i * 2];
    odd[i]  = a[i * 2 + 1];
  }

  FastFourierTransform(even, n / 2);
  FastFourierTransform(odd, n / 2);

  for (size_t i = 0; i < n / 2; ++i) {
    std::complex<double> t = std::polar(1.0, -2.0 * M_PI * i / n) * odd[i];
    a[i]                   = even[i] + t;
    a[i + n / 2]           = even[i] - t;
  }

  delete[] even;
  delete[] odd;
}
