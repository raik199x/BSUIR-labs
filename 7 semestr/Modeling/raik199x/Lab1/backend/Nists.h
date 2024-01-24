#pragma once
#include <QWidget>
#include <complex>
#include <cstddef>
#include <cstdint>

bool FrequencyMonobitTest(const uint64_t *Selection, const size_t SelectionSize);
bool FrequencyBlockTest(const uint64_t *Selection, const size_t SelectionSize, const uint16_t AmountOfBlocks);
bool TestForSequenceOfIdenticalBits(const uint64_t *Selection, const size_t SelectionSize);
bool TestForLongestRunOfOnesInABlock(const uint64_t *Selection, const size_t SelectionSize,
                                     const uint16_t AmountOfBlocks);
bool BinaryMatrixRankTest(const uint64_t *Selection, const size_t SelectionSize);
bool SpectrumTest(const uint64_t *Selection, const size_t SelectionSize);
bool NonOverlappingTemplateMatchingTest(const uint64_t *Selection, const size_t SelectionSize,
                                        const QString pathToFIle);
bool OverlappingTemplateMatchingTest(const uint64_t *Selection, const size_t SelectionSize,
                                     const QString pathToFIle);