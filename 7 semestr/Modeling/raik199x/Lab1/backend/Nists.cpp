// Qt headers
#include <QLabel>
#include <QVBoxLayout>

#include <omp.h> // OpenMP

#include "NistOperationalFunctions/Operations.h"
#include "Nists.h"
#include "sharedVariables.h"

/**
 * @note IMPORTANT: for those who randomly searched for nist text, be aware of using this
 * Spectrum test, possibly, will be passed at any selection and there is not guarantee that other tests are
 * written correctly
 */

/**
 * @brief Performs the Frequency (Monobit) Test on a given selection of 64-bit unsigned integers.
 *
 * @param Selection A pointer to the first element of the selection of 64-bit unsigned integers.
 * @param SelectionSize The number of elements in the selection.
 * @return true/false
 */
bool FrequencyMonobitTest(const uint64_t *Selection, const size_t SelectionSize) {
  // Counting the number of ones in the binary sequence
  uint64_t OnesCounter = 0;
  uint64_t AllBits     = AmountOfBitsInVariable * SelectionSize;

  for (size_t i = 0; i < SelectionSize; i++) {
    OnesCounter += __builtin_popcountll(Selection[i]);
  }

  // Calculate the ratio of ones to all bits in the sequence
  double S = (double)AllBits / OnesCounter;

  return 1.8 < S && S < 2.2;
}

/**
 * @brief Runs a frequency block test on a given selection of bits.
 *
 * @param Selection Pointer to the selection of bits to test.
 * @param SelectionSize The size of the selection in bytes.
 * @param AmountOfBlocks The number of blocks to divide the selection into.
 * @return true/false
 */
bool FrequencyBlockTest(const uint64_t *Selection, const size_t SelectionSize,
                        const uint16_t AmountOfBlocks) {
  // Calculate the number of bits in each block
  uint64_t BitsInBlock = AmountOfBitsInVariable * SelectionSize / AmountOfBlocks;

  // Create a new widget to hold the results of the block test
  QWidget     *ResultWidget = new QWidget();
  QVBoxLayout *BigLayout    = new QVBoxLayout();
  ResultWidget->setLayout(BigLayout);

  // Iterate over each block
  for (size_t iter = 0, blocks = 0; iter < SelectionSize; iter += SelectionSize / AmountOfBlocks, blocks++) {
    // Check if the desired number of blocks has been processed
    if (blocks == AmountOfBlocks)
      break;

    // Extract a sub-selection corresponding to the current block
    uint64_t *SubSelection = new uint64_t[SelectionSize / AmountOfBlocks];
    memcpy(SubSelection, Selection + iter, SelectionSize / AmountOfBlocks * sizeof(uint64_t));

    bool result = FrequencyMonobitTest(SubSelection, SelectionSize / AmountOfBlocks);
    // Free memory allocated for the sub-selection
    free(SubSelection);

    if (!result) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Test for Sequence of Identical Bits
 *
 * @param Selection A pointer to an array of 32-bit unsigned integers to be tested.
 * @param SelectionSize The size of the Selection array.
 * @return true/false
 */
bool TestForSequenceOfIdenticalBits(const uint64_t *Selection, const size_t SelectionSize) {
  uint64_t AmountOfRows = 0;

  // Parallelize the loop using OpenMP and use reduction to sum the AmountOfRows
#pragma omp parallel for reduction(+ : AmountOfRows)
  for (size_t i = 0; i < SelectionSize; i++) {
    // Initialize LastBit with the least significant bit of the current element in the Selection array
    bool LastBit = Selection[i] & (1 << 0);

    // Iterate through each bit in the current element
    for (size_t j = 1; j < AmountOfBitsInVariable; j++) {
      // Extract the j-th bit of the current element
      bool CurrentBit = Selection[i] & (1 << j);

      // Check if the current bit is different from the last bit
      if (CurrentBit != LastBit) {
        // If different, increment the count of rows
        AmountOfRows++;
        LastBit = CurrentBit;
      }
    }
  }

  // Calculate the P value, which represents the proportion of rows in the sequence
  double P = (double)AmountOfRows / (SelectionSize * AmountOfBitsInVariable);
  return (P > 0.01);
}

/**
 * @brief Runs the Longest Run of Ones Test on the given selection of bits.
 *
 * @param Selection The selection of bits to test.
 * @param SelectionSize The size of the selection in 32-bit words.
 * @param AmountOfBlocks The number of blocks to divide the selection into.
 * @return true/false
 */
bool LongestRunOfOnesTest(const uint64_t *Selection, const size_t SelectionSize,
                          const uint16_t AmountOfBlocks) {
  // Calculate the number of bits in each block
  uint64_t BitsInBlock = AmountOfBitsInVariable * SelectionSize / AmountOfBlocks;

  // Initialize the count of failures to zero
  size_t countOfFailures = 0;

  // Parallelize the loop using OpenMP and use reduction to sum the countOfFailures
#pragma omp parallel for reduction(+ : countOfFailures)
  for (size_t iter = 0; iter < AmountOfBlocks; iter++) {
    // Initialize variables to track the longest run of ones in the current block
    uint64_t LongestRunOfOnes = 0;
    uint64_t CurrentRunOfOnes = 0;

    // Iterate through each word in the current block
    for (size_t i = 0; i < SelectionSize / AmountOfBlocks; i++) {
      // Iterate through each bit in the word
      for (size_t j = 0; j < AmountOfBitsInVariable; j++) {
        // Check if the j-th bit is a one
        if (Selection[i] & (1 << j)) {
          CurrentRunOfOnes++;
        } else {
          // If the run of ones is longer than the current longest run, update it
          if (CurrentRunOfOnes > LongestRunOfOnes)
            LongestRunOfOnes = CurrentRunOfOnes;
          CurrentRunOfOnes = 0;
        }
      }
    }

    // Compare the longest run of ones with NIST requirements
    if (BitsInBlock >= 128) {
      if (LongestRunOfOnes < floor(log2(BitsInBlock) - 2 * sqrt(BitsInBlock))) {
        countOfFailures++;
      }
    } else {
      if (LongestRunOfOnes < floor(log2(BitsInBlock) + 1)) {
        countOfFailures++;
      }
    }
  }
  return (countOfFailures <= (1.0 - 0.003) * AmountOfBlocks);
}

bool TestForLongestRunOfOnesInABlock(const uint64_t *Selection, const size_t SelectionSize,
                                     const uint16_t AmountOfBlocks) {
  // Calculate the number of bits in each block
  uint64_t BitsInBlock = AmountOfBitsInVariable * SelectionSize / AmountOfBlocks;

  // Initialize the count of failures to zero
  size_t countOfFailures = 0;

  // Parallelize the loop using OpenMP and use reduction to sum the countOfFailures
#pragma omp parallel for reduction(+ : countOfFailures)
  for (size_t iter = 0; iter < AmountOfBlocks; iter++) {
    // Initialize variables to track the longest run of ones in the current block
    uint64_t LongestRunOfOnes = 0;
    uint64_t CurrentRunOfOnes = 0;

    // Iterate through each word in the current block
    for (size_t i = 0; i < SelectionSize / AmountOfBlocks; i++) {
      // Iterate through each bit in the word
      for (size_t j = 0; j < AmountOfBitsInVariable; j++) {
        // Check if the j-th bit is a one
        if (Selection[i] & (1 << j)) {
          CurrentRunOfOnes++;
        } else {
          // If the run of ones is longer than the current longest run, update it
          if (CurrentRunOfOnes > LongestRunOfOnes)
            LongestRunOfOnes = CurrentRunOfOnes;
          CurrentRunOfOnes = 0;
        }
      }
    }

    // Compare the longest run of ones with NIST requirements
    if (BitsInBlock >= 128) {
      if (LongestRunOfOnes < floor(log2(BitsInBlock) - 2 * sqrt(BitsInBlock))) {
        countOfFailures++;
      }
    } else {
      if (LongestRunOfOnes < floor(log2(BitsInBlock) + 1)) {
        countOfFailures++;
      }
    }
  }
  return (countOfFailures <= (1.0 - 0.003) * AmountOfBlocks);
}

/**
 * @brief Binary Matrix Rank Test
 *
 * @param Selection Random selection of <AmountOfBitsInVariable>-bit unsigned integers
 * @param SelectionSize Size of the selection in <AmountOfBitsInVariable>-bit words
 * @return true/false
 */
bool BinaryMatrixRankTest(const uint64_t *Selection, const size_t SelectionSize) {
  // Creating a new widget to hold the results of the test
  QWidget     *ResultWidget = new QWidget();
  QVBoxLayout *Layout       = new QVBoxLayout();
  ResultWidget->setLayout(Layout);

  // If the selection size is less than <AmountOfBitsInVariable>, the test cannot be performed
  if (SelectionSize < AmountOfBitsInVariable) {
    return false;
  }

  uint64_t AmountOfSuccesses = 0;
  uint64_t AmountOfMatrices  = static_cast<uint64_t>(SelectionSize) / AmountOfBitsInVariable;
#pragma omp parallel for reduction(+ : AmountOfSuccesses)
  for (size_t i = 0; i < AmountOfMatrices; i++) {
    // Create matrix
    uint8_t Matrix[AmountOfBitsInVariable][AmountOfBitsInVariable];
    // Fill the matrix with bits from the selection
    for (size_t j = 0; j < AmountOfBitsInVariable; j++) {
      for (size_t k = 0; k < AmountOfBitsInVariable; k++) {
        Matrix[j][k] = (Selection[i * AmountOfBitsInVariable + j] & (1 << k)) ? 1 : 0;
      }
    }
    AmountOfSuccesses += isLinearIndependet(Matrix, AmountOfBitsInVariable) ? 0 : 1;
  }

  return (AmountOfSuccesses / AmountOfMatrices * 100 > 80);
}

bool SpectrumTest(const uint64_t *Selection, const size_t SelectionSize) {

  // Preparing data
  std::complex<double> *ComplexSelection = new std::complex<double>[SelectionSize];
#pragma omp parallel for
  for (size_t i = 0; i < SelectionSize; i++) {
    ComplexSelection[i] = std::complex<double>(Selection[i], 0);
  }
  // Calculating result
  FastFourierTransform(ComplexSelection, SelectionSize);

  double Tvalue = sqrt(log(1 / 0.05) * SelectionSize);
  double N0     = 0.95 * SelectionSize / 2;

  double d      = (floor(N0) - N0) / (sqrt(SelectionSize * 0.95 * 0.05 / 4));
  double Pvalue = erfc(fabs(d) / sqrt(2));

  // leaving
  delete[] ComplexSelection;
  return (Pvalue > 0.01);
}

// Function to perform non-overlapping template matching test
bool NonOverlappingTemplateMatchingTest(const uint64_t *Selection, const size_t SelectionSize,
                                        const QString pathToFIle) {
  // Read the template from the file
  std::vector<std::vector<bool>> templates = GetTemplatesFromFile(pathToFIle);

  // Check if the file was read successfully
  if (templates.empty()) {
    return false;
  }

  size_t *occurrences = new size_t[templates.size()];
  for (size_t i = 0; i < templates.size(); i++) {
    occurrences[i] = 0;
  }

#pragma omp parallel for reduction(+ : occurrences[ : templates.size()])
  // Iterate over each template
  for (size_t i = 0; i < templates.size(); i++) {
    // Iterate over each position in the selection
    for (size_t j = 0; j < SelectionSize;) {
      bool match = true;
      // Iterate over each bit in the current template
      for (size_t k = 0; k < templates[i].size(); k++) {
        // Check if the corresponding bit in the selection matches the template
        if (templates[i][k] != ((Selection[j] & (1 << k)) ? true : false)) {
          match = false;
          j++;
          break;
        }
      }
      // If there is a match, increment the occurrences count for the current template
      if (match) {
        occurrences[i]++;
        j += templates[i].size();
      }
    }
  }

  // Calculating percentage of appearances
  double *percentages = new double[templates.size()];
  for (size_t i = 0; i < templates.size(); i++) {
    percentages[i] = (double)occurrences[i] / SelectionSize;
    if (percentages[i] < 0.000001) { // If the percentage is too small, set it to zero
      percentages[i] = 0;
    }
  }

  // freeing memory
  delete[] occurrences;

  // Getting total percentage
  double totalPercentage = 0;
  for (size_t i = 0; i < templates.size(); i++) {
    totalPercentage += percentages[i];
  }
  return totalPercentage > 0.2 ? false : true;
}

// Function to perform overlapping template matching test
bool OverlappingTemplateMatchingTest(const uint64_t *Selection, const size_t SelectionSize,
                                     const QString pathToFIle) {
  // Read the template from the file
  std::vector<std::vector<bool>> templates = GetTemplatesFromFile(pathToFIle);

  // Check if the file was read successfully
  if (templates.empty()) {
    return false;
  }

  size_t *occurrences = new size_t[templates.size()];
  for (size_t i = 0; i < templates.size(); i++) {
    occurrences[i] = 0;
  }

#pragma omp parallel for reduction(+ : occurrences[ : templates.size()])
  // Iterate over each template
  for (size_t i = 0; i < templates.size(); i++) {
    // Iterate over each position in the selection
    for (size_t j = 0; j < SelectionSize; j++) {
      bool match = true;
      // Iterate over each bit in the current template
      for (size_t k = 0; k < templates[i].size(); k++) {
        // Check if the corresponding bit in the selection matches the template
        if (templates[i][k] != ((Selection[j] & (1 << k)) ? true : false)) {
          match = false;
          break;
        }
      }
      // If there is a match, increment the occurrences count for the current template
      if (match) {
        occurrences[i]++;
      }
    }
  }

  // Calculating percentage of appearances
  double *percentages = new double[templates.size()];
  for (size_t i = 0; i < templates.size(); i++) {
    percentages[i] = (double)occurrences[i] / (SelectionSize - templates[i].size());
    if (percentages[i] < 0.000001) { // If the percentage is too small, set it to zero
      percentages[i] = 0;
    } else if (percentages[i] > 1) { // If the percentage is too large, set it to one
      percentages[i] = 1;
    }
  }

  // freeing memory
  delete[] occurrences;

  // if any of percentages is higher 0.5 -> test failed
  for (size_t i = 0; i < templates.size(); i++) {
    if (percentages[i] > 0.2) {
      return false;
    }
  }

  return true;
}