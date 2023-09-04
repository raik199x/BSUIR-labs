#pragma once

#include <string>
#include <mpich/mpi.h>
#include <filesystem>

void MPIslaveMainLoop(int myId, std::string pathToExecutable);