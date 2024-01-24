#include "Generators.h"

uint64_t *RandomMiddleSquaresNumbers(size_t AmountOfNumbers, uint64_t seed) {
  uint64_t *results = new uint64_t[AmountOfNumbers];
  uint64_t  x       = seed;

  for (size_t i = 0; i < AmountOfNumbers; i++) {
    // Square the seed
    __uint128_t SquareSeed = static_cast<__uint128_t>(x) * static_cast<__uint128_t>(x);

    // Use a mask to get the middle 64 bits
    uint64_t result = static_cast<uint64_t>((SquareSeed >> 32) & 0xFFFFFFFFFFFFFFFFULL);

    results[i] = result;
    x = result;
  }

  return results;
}