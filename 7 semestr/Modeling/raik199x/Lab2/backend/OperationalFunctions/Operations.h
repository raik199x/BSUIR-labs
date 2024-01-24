#pragma once

#include <QWidget>
#include <complex>
#include <cstdint>
#include <cstdlib>

#include "thirdParty/qcustomplot/qcustomplot.h"

// Graphic manipulation functions
template <typename T>
QCustomPlot *BuildHistogramPlot(T *Selection, size_t SelectionSize, T MinimalValue, T MaximalValue) {
  // Building plot
  QCustomPlot *Plot = new QCustomPlot();

  // Create bars for the histogram
  QCPBars *bars = new QCPBars(Plot->xAxis, Plot->yAxis);

  // Set up for a bar chart
  bars->setPen(QPen(Qt::blue));
  bars->setBrush(QBrush(QColor(0, 0, 255, 20)));

  // Building data for histogram
  const size_t numBins = 256; // Adjust the number of bins as needed

  // Calculate bin width
  double binWidth = static_cast<double>(MaximalValue - MinimalValue) / numBins;

  // Initialize bins
  std::vector<uint64_t> bins(numBins, 0);

  // Count occurrences in each bin
  for (size_t i = 0; i < SelectionSize; ++i) {
    // Determine the bin for the current value
    size_t binIndex = static_cast<size_t>((Selection[i] - MinimalValue) / binWidth);

    // Increment the corresponding bin
    bins[binIndex]++;
  }

  // Set the data for bars
  QVector<double> xData, yData;
  for (size_t i = 0; i < numBins; ++i) {
    double binCenter = MinimalValue + (i + 0.5) * binWidth;
    xData.push_back(binCenter);
    yData.push_back(bins[i]);
  }
  bars->setData(xData, yData);

  // Setting axes
  Plot->xAxis->setLabel("Value (10^-3)");
  Plot->yAxis->setLabel("Frequency");

  // Setting y-axis range
  Plot->yAxis->setRange(0, *std::max_element(yData.begin(), yData.end())); // Adjusted the y-axis range
  Plot->xAxis->setRange(MinimalValue, MaximalValue);

  // Setting legend
  Plot->legend->setVisible(true);
  bars->setName("Histogram");

  // Setting plot title
  Plot->plotLayout()->insertRow(0);
  Plot->plotLayout()->addElement(0, 0, new QCPTextElement(Plot, "Histogram", QFont("sans", 12, QFont::Bold)));

  // Setting plot size
  Plot->setMinimumSize(800, 300);
  Plot->replot();

  // Returning plot
  return Plot;
}

QCustomPlot *GetEmptyPlot(QString Title, QString XLabel, QString YLabel);

// Algorithm functions
template <typename T>
T GetMinimalValue(T *array, size_t size) {
  T min = array[0];
  for (size_t i = 1; i < size; i++) {
    if (array[i] < min) {
      min = array[i];
    }
  }
  return min;
}

template <typename T>
T GetMaximalValue(T *array, size_t size) {
  T max = array[0];
  for (size_t i = 1; i < size; i++) {
    if (array[i] > max) {
      max = array[i];
    }
  }
  return max;
}
