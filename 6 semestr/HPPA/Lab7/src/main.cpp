#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <vector>

#include <mpich/mpi.h>

#include "../include/global.hpp"
#include "../include/master.hpp"
#include "../include/slave.hpp"

// TODO:
// * Create folder with images (and add images, near 20)
// * Lab5-6 - remove obsolete step-by-step image solving (save only result image)
// * Get path of lab5-6 executable from IMAGE_PROCESSING_PATH environment variable
// * Read images with OpenCV and convert them to one of mpi types (for mpi types check docs)
// * Send to nodes images
// * Get images from nodes and save in folder name "Processed"
// * Test master
// * Test slave
// ! test on multiple nodes
// * Test png, jpg image types

/*!
 * \brief Finalize MPI and terminate program with exit code EXIT_PANIC
 */
inline auto MPI_Panic = []() {
	assert_v(MPI_Finalize() == MPI_SUCCESS, "Failed to finalize MPI");
	std::exit(EXIT_PANIC);
};

int main(int argc, char **argv) {
	assert_v(argc == 2, "Specify path to folder with images");
	std::string folderPath = argv[1];
	if (folderPath[folderPath.size() - 1] != '/')
		folderPath += '/';
	assert_v(std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath), "Specified folder does not exist");
	char *executable = getenv("IMAGE_PROCESSING_PATH");
	assert_v(executable != nullptr, "Environment variable IMAGE_PROCESSING_PATH is not set");
	assert_v(MPI_Init(&argc, &argv) == MPI_SUCCESS, "Failed to initialize MPI");

	int myId, amountOfProcesses;
	assert_v(MPI_Comm_rank(MPI_COMM_WORLD, &myId) == MPI_SUCCESS, "Failed to get process ID");
	assert_v(MPI_Comm_size(MPI_COMM_WORLD, &amountOfProcesses) == MPI_SUCCESS, "Failed to get amount of processes");

	if (!expect_v(amountOfProcesses >= 2, "You need to run this program with at least 2 processes"))
		MPI_Panic();

	if (myId == 0) {
		// Getting files in a folder (file type: png, jpg, jpeg)
		auto files = GettingFiles(folderPath);
		if (!expect_v(files.size() != 0, "No images found in folder"))
			MPI_Panic();

		// Main loop
		auto start = std::chrono::high_resolution_clock::now();
		MPImasterMainLoop(myId, amountOfProcesses, folderPath, files);
		auto stop = std::chrono::high_resolution_clock::now();

		auto duration     = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		double time_spent = duration.count() / 1000.0;
		std::cout << "Done! Time spend: " << time_spent << " seconds" << std::endl;
	} else {
		std::string pathToExecutable(executable);
		MPIslaveMainLoop(myId, pathToExecutable);
	}
	assert_v(MPI_Finalize() == MPI_SUCCESS, "Failed to finalize MPI");
}
