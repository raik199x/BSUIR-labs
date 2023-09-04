#pragma once

#include <opencv2/core/mat.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
#include <mpich/mpi.h>

std::vector<std::string> GettingFiles(std::string folderPath);
void MPImasterMainLoop(int myId, int amountOfProcesses, std::string folderPath, std::vector<std::string> files);
bool receivingImage(std::string folderPath, int imagesReceived, int **imagesSize, int source);
bool endTransmission(int amountOfProcesses);