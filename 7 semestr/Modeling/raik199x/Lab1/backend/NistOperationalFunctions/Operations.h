#pragma once

#include <QWidget>
#include <complex>
#include <cstdint>
#include <cstdlib>

#include "sharedVariables.h"
#include "thirdParty/qcustomplot/qcustomplot.h"

// Matrix functions
void GaussianElimination(uint8_t (*Matrix)[AmountOfBitsInVariable], size_t MatrixSize);
bool isLinearIndependet(uint8_t (*Matrix)[AmountOfBitsInVariable], size_t MatrixSize);

// Graphic manipulation functions
QCustomPlot *BuildSpectrumPlot(std::complex<double> *ComplexSelection, size_t SelectionSize);
QCustomPlot *BuildHistogramPlot(uint64_t *Selection, size_t SelectionSize, uint64_t MinimalValue,
                                uint64_t MaximalValue);
QCustomPlot *GetEmptyPlot(QString Title, QString XLabel, QString YLabel);

// File operations
std::vector<std::vector<bool>> GetTemplatesFromFile(QString pathToFile);

// Math functions
void   FastFourierTransform(std::complex<double> *a, size_t n);
double calculateMean(const uint64_t *array, size_t size);
// Calculating dispersion
double calculateDispersion(const uint64_t *array, size_t size, double mean);

// Algorithm functions
uint64_t GetMinimalValue(uint64_t *array, size_t size);
uint64_t GetMaximalValue(uint64_t *array, size_t size);
double  *findHighestAndLowest(const std::complex<double> *arr, size_t size);