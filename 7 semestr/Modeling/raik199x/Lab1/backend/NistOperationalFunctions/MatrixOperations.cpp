#include "Operations.h"

void GaussianElimination(uint8_t (*Matrix)[AmountOfBitsInVariable], size_t MatrixSize) {
  for (size_t j = 0; j < MatrixSize; j++) {
    // If the j-th bit of the j-th row is zero, swap the row with a row below it
    if (Matrix[j][j] == 0) {
      for (size_t k = j + 1; k < MatrixSize; k++) {
        if (Matrix[k][j] == 1) {
          for (size_t l = 0; l < MatrixSize; l++) {
            uint8_t temp = Matrix[j][l];
            Matrix[j][l] = Matrix[k][l];
            Matrix[k][l] = temp;
          }
          break;
        }
      }
    }

    // If the j-th bit of the j-th row is still zero, skip the row
    if (Matrix[j][j] == 0)
      continue;

    // Iterate through each row below the j-th row
    for (size_t k = j + 1; k < MatrixSize; k++) {
      // If the j-th bit of the k-th row is one, XOR the k-th row with the j-th row
      if (Matrix[k][j] == 1) {
        for (size_t l = 0; l < MatrixSize; l++) {
          Matrix[k][l] = Matrix[k][l] ^ Matrix[j][l];
        }
      }
    }
  }
}

bool isLinearIndependet(uint8_t (*Matrix)[AmountOfBitsInVariable], size_t MatrixSize) {
  // Perform Gaussian elimination on the matrix
  GaussianElimination(Matrix, MatrixSize);

  // Check if the rank of the matrix is MatrixSize, if the rank is MatrixSize, the test fails
  // Since matrices must be linear independet to have a rank of MatrixSize, this means that the selection is
  // not random
  bool isLinearIndependet = true;
  for (size_t j = 0; j < MatrixSize; j++) {
    bool RowIsZero = true;
    for (size_t k = 0; k < MatrixSize; k++) {
      if (Matrix[j][k] == 1) {
        RowIsZero = false;
        break;
      }
    }
    if (RowIsZero) {
      isLinearIndependet = false;
      break;
    }
  }
  return isLinearIndependet;
}