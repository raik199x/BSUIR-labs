#include "Operations.h"

uint64_t GetMinimalValue(uint64_t *array, size_t size) {
  uint64_t min = array[0];
  for (size_t i = 1; i < size; i++) {
    if (array[i] < min) {
      min = array[i];
    }
  }
  return min;
}
uint64_t GetMaximalValue(uint64_t *array, size_t size) {
  uint64_t max = array[0];
  for (size_t i = 1; i < size; i++) {
    if (array[i] > max) {
      max = array[i];
    }
  }
  return max;
}

// Function to find the highest and lowest absolute values
double *findHighestAndLowest(const std::complex<double> *arr, size_t size) {
  if (size == 0) {
    return nullptr;
  }

  double highest = std::abs(arr[0].real()); // Initialize with the first element's absolute value
  double lowest  = std::abs(arr[0].real());

  for (size_t i = 1; i < size; ++i) {
    double absValue = std::abs(arr[i].real());

    // Update highest and lowest values
    if (absValue > highest) {
      highest = absValue;
    }
    if (absValue < lowest) {
      lowest = absValue;
    }
  }
  static double result[2];
  result[0] = highest;
  result[1] = lowest;
  return result;
}