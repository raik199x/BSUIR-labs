#pragma once

#include <cstdint>
#include <cstdio>

#include <QMessageBox>

uint64_t *RandomLehmerNumbers(size_t AmountOfNumbers, uint64_t seed, uint64_t m, uint64_t a);
uint64_t *RandomMicNumbers(size_t AmountOfNumbers, uint64_t seed, uint64_t feedbackMask);
uint64_t *RandomMiddleSquaresNumbers(size_t AmountOfNumbers, uint64_t seed);
uint64_t *CplusplusNumbers(size_t AmountOfNumbers, uint64_t seed);