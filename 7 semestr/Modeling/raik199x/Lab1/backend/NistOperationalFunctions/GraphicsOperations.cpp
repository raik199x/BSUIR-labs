#include <limits>

#include "Operations.h"

QCustomPlot *BuildHistogramPlot(uint64_t *Selection, size_t SelectionSize, uint64_t MinimalValue,
                                uint64_t MaximalValue) {
  // Building plot
  QCustomPlot *Plot = new QCustomPlot();

  // Create bars for the histogram
  QCPBars *bars = new QCPBars(Plot->xAxis, Plot->yAxis);
  // Plot->addPlottable(bars);

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
  Plot->xAxis->setLabel("Value");
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

QCustomPlot *GetEmptyPlot(QString Title, QString XLabel, QString YLabel) {
  // Building plot
  QCustomPlot *Plot = new QCustomPlot();
  Plot->addGraph();
  Plot->graph(0)->setPen(QPen(Qt::blue));
  Plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
  Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
  Plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
  Plot->graph(0)->data()->clear();

  // Setting axes
  Plot->xAxis->setLabel(XLabel);
  Plot->yAxis->setLabel(YLabel);

  // Setting y-axis range
  Plot->yAxis->setRange(0, UINT64_MAX);
  Plot->xAxis->setRange(0, 1);

  // Setting legend
  Plot->legend->setVisible(true);
  Plot->graph(0)->setName(Title);

  // Setting plot title
  Plot->plotLayout()->insertRow(0);
  Plot->plotLayout()->addElement(0, 0, new QCPTextElement(Plot, Title, QFont("sans", 12, QFont::Bold)));

  // Setting plot size
  Plot->setMinimumSize(800, 300);
  Plot->replot();

  // Returning plot
  return Plot;
}