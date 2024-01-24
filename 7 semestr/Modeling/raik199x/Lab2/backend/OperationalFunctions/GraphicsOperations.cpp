#include <limits>

#include "Operations.h"

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