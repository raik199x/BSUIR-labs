#include "Generators.h"

uint64_t *RandomMicNumbers(size_t AmountOfNumbers, uint64_t seed, uint64_t feedbackMask) {
  uint64_t *results = new uint64_t[AmountOfNumbers];

  for (size_t i = 0; i < AmountOfNumbers; i++) {
    // Mix bits using XOR and left shift
    seed ^= (seed << 13) | (seed >> 19);
    // Apply feedback mask
    seed ^= feedbackMask;
    // Use the value as the generated pseudo-random number
    results[i] = seed;
  }
  return results;
}