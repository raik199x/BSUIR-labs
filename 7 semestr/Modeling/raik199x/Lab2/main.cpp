#include "mainwindow.h"
#include <QApplication>
#include <omp.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Get number of threads
  int threads = omp_get_max_threads();
  // Set number of threads
  omp_set_num_threads(threads == 1 ? 1 : threads - 1);

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}