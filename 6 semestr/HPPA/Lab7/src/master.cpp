#include "../include/master.hpp"
#include "../include/global.hpp"
#include <iostream>
#include <mpich/mpi.h>
#include <string>

std::vector<std::string> GettingFiles(std::string folderPath) {
	// Getting files in a folder (file type: png, jpg, jpeg)
	std::vector<std::string> files;
	std::regex img_regex("(.*\\.png|.*\\.jpeg|.*\\.jpg)$");
	for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
		if (std::filesystem::is_regular_file(entry) && std::regex_match(entry.path().filename().string(), img_regex))
			files.push_back(entry.path());
	}
	return files;
}

bool receivingImage(std::string folderPath, int imagesReceived, int **imagesSize, int source) {
	int imageSizeCalculated = imagesSize[source - 1][0] * imagesSize[source - 1][1] * imagesSize[source - 1][2];
	short *receivedImage    = new short[imageSizeCalculated];
	MPI_Recv(receivedImage, imageSizeCalculated, MPI_SHORT, source, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	Mat image = ConvertShortToMat(receivedImage, imagesSize[source - 1][0], imagesSize[source - 1][1], imagesSize[source - 1][2]);

	// Save image
	return imwrite(folderPath + "Processed/" + std::to_string(imagesReceived + 1) + ".jpeg", image);
}

bool endTransmission(int amountOfProcesses) {
	int codes[3] = {-1, -1, -1};
	for (int i = 1; i < amountOfProcesses; i++)
		MPI_Send(codes, 3, MPI_INT, i, 0, MPI_COMM_WORLD);
	return true;
}

void MPImasterMainLoop(int myId, int amountOfProcesses, std::string folderPath, std::vector<std::string> files) {
	// Now, we are working
	std::string pathForProcessedImages = folderPath + "Processed/";
	if (!std::filesystem::exists(pathForProcessedImages))
		std::filesystem::create_directory(pathForProcessedImages);

	// We need somewhere to store size of images (height, width, channels)
	// of course for all Processes
	int **imagesSize = new int *[amountOfProcesses - 1];
	for (size_t i = 0; i < amountOfProcesses - 1; i++) {
		imagesSize[i]    = new int[3];
		imagesSize[i][0] = -1;
		imagesSize[i][1] = -1;
		imagesSize[i][2] = -1;
	}

	size_t imagesReceived = 0, imagesSent = 0;
	while (imagesReceived < files.size()) {
		int index = -1, nodeIndex = -1; // index stands for array were we does not include 0, node index stands for node number (including 0)
		// if we are already send all images, wait for all nodes to finish
		if (imagesSent == files.size()) {
			std::cout << "Waiting for all nodes to finish\n";
			int flag = 0;      // sets to 1 if message is available
			MPI_Status status; // to get source and tag of message
			while (flag == 0)
				MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
			receivingImage(folderPath, imagesReceived, imagesSize, status.MPI_SOURCE);
			imagesReceived++;
			continue;
		}

		// otherwise check if any node is free
		for (size_t i = 0; i < amountOfProcesses - 1; i++) {
			if (!(imagesSize[i][0] == -1) && !(imagesSize[i][1] == -1) && !(imagesSize[i][2] == -1))
				continue;
			index     = i;
			nodeIndex = index + 1;
			break;
		}

		// If no free nodes, wait for one
		if (index == -1) {
			int flag = 0;      // sets to 1 if message is available
			MPI_Status status; // to get source and tag of message
			while (flag == 0)
				MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
			receivingImage(folderPath, imagesReceived, imagesSize, status.MPI_SOURCE);
			nodeIndex            = status.MPI_SOURCE;	// node index is source of message
			index                = nodeIndex - 1;		// index is node index - 1
			imagesSize[index][0] = -1; 					// Clearing memory
			imagesSize[index][1] = -1;
			imagesSize[index][2] = -1;
			imagesReceived++;
		}

		// When found free node, send image to it
		// Convert image to short array
		Mat image        = imread(files[imagesSent]);
		short *imageData = ConvertMatToShort(image);
		// Saving image data
		imagesSize[index][0] = image.cols;
		imagesSize[index][1] = image.rows;
		imagesSize[index][2] = image.channels();
		// Send image to free node, first we will send how much data we will send
		assert_v(MPI_Send(imagesSize[index], 3, MPI_INT, nodeIndex, 0, MPI_COMM_WORLD) == MPI_SUCCESS, "Failed to send image size to node");
		assert_v(MPI_Send(imageData, image.rows * image.cols * image.channels(), MPI_SHORT, nodeIndex, 0, MPI_COMM_WORLD) == MPI_SUCCESS, "Failed to send image to node");
		imagesSent++;
	}
	endTransmission(amountOfProcesses);
	// deleting memory
	for (size_t i = 0; i < amountOfProcesses - 1; i++)
		delete[] imagesSize[i];
	delete[] imagesSize;
}