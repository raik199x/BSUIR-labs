#include <fstream>
#include <vector>

#include "Operations.h"

std::vector<std::vector<bool>> GetTemplatesFromFile(QString pathToFile) {
  std::vector<std::vector<bool>> templates;
  // trying to open file
  std::fstream file;
  file.open(pathToFile.toStdString(), std::ios::in);
  if (!file.is_open()) {
    return templates;
  }

  std::string fileContent;
  while (std::getline(file, fileContent)) {
    std::vector<bool> templateVector;
    for (auto &i : fileContent) {
      if (i == '1') {
        templateVector.push_back(true);
      } else if (i == '0') {
        templateVector.push_back(false);
      }
    }

    // Add the template vector to the templates vector
    templates.push_back(templateVector);
  }

  return templates;
}
