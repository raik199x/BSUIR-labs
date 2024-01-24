#include "Generators.h"

uint64_t *LehmerGenerator(size_t AmountOfNumbers, uint64_t seed, uint64_t m, uint64_t a) {
  uint64_t *results = new uint64_t[AmountOfNumbers];
  uint64_t  x       = seed;
  for (size_t i = 0; i < AmountOfNumbers; i++) {
    x          = (a * x) % m;
    results[i] = x;
  }
  return results;
}
