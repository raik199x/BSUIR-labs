#include <random>

#include "Generators.h"

uint64_t *CplusplusNumbers(size_t AmountOfNumbers, uint64_t seed) {
  std::mt19937_64                         generator(seed);
  std::uniform_int_distribution<uint64_t> distribution(0, 0xFFFFFFFFFFFFFFFF);
  uint64_t                               *result = new uint64_t[AmountOfNumbers];
  for (size_t i = 0; i < AmountOfNumbers; i++) {
    result[i] = distribution(generator);
  }
  return result;
}