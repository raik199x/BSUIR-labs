#include "../include/slave.hpp"
#include "../include/global.hpp"
#include <iostream>

void MPIslaveMainLoop(int myId, std::string pathToExecutable) {
	int source = 0; // master
	while (true) {
		// First we get size of image
		int imageSize[3];
		MPI_Recv(&imageSize, 3, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		if(imageSize[0] == -1 && imageSize[1] == -1 && imageSize[2] == -1)
			break;
			
		int imageSizeCalculated = imageSize[0] * imageSize[1] * imageSize[2];
		// then we allocate memory for image
		short *imageData = new short[imageSizeCalculated];
		// then we receive image
		MPI_Recv(imageData, imageSizeCalculated, MPI_SHORT, source, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// now we temporary saving image on disk
		std::string tempImageName = "tempNode" + std::to_string(myId) + ".png"; 
		Mat image = ConvertShortToMat(imageData, imageSize[0], imageSize[1], imageSize[2]);
		imwrite(tempImageName, image);

		// preparing command for running lab5-6 executable
		std::string savePath = "node" + std::to_string(myId) + "/";
		std::string command = pathToExecutable + " " + tempImageName + " -noLogs -onlyResult -savePath " + savePath;
		// running lab5-6 executable
		system(command.c_str());

		// reading result image
		Mat resultImage = imread(savePath + "resultImage.jpeg");

		// converting result image to short array
		short *resultImageData = ConvertMatToShort(resultImage);

		// deleting temporary files
		std::filesystem::remove(tempImageName);
		// and removing folder with images
		std::filesystem::remove_all(savePath);

		// sending result image to master
		MPI_Send(resultImageData, imageSizeCalculated, MPI_SHORT, source, 0, MPI_COMM_WORLD);

		// deleting allocated memory
		delete[] imageData;
		delete[] resultImageData;
	}
}