#include <filesystem>
#include <iostream>
#include <map>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <utility>

#include "../include/cudaRotate.h"
#include "../include/global.hpp"
#include "../include/houghMethod.h"
#include "../include/opencvFunctions.h"
#include "../include/thresholdTransform.h"

// Condition 6
/*
marker = rectangular, color (RGB) C = (0 255 255)

(a) Используя пороговое преобразования обнаружить маркер цвета C согласно варианту.
(b) Используя преобразования Хафа определить параметры маркера;
(c) Используя выданное преобразование восстановить изображение.

*/
using namespace cv;

// since usually 1 pixel represents 8 bit, it's better to convert it to char (1 byte)
short *ConvertMatToShort(Mat inputImage) {
	// Get the image properties
	size_t rows      = inputImage.rows;
	size_t cols      = inputImage.cols;
	size_t channels  = inputImage.channels();
	size_t imageSize = rows * cols * channels;

	// Allocate memory on the CUDA device
	short *d_image = new short[imageSize];

	// copy the data from the OpenCV input image to the CUDA device memory
	for (size_t i = 0; i < imageSize; i++)
		d_image[i] = inputImage.data[i];

	return d_image;
}

Mat ConvertShortToMat(short *image, size_t width, size_t height, short channels) {
	size_t imageSize = width * height * channels;
	Mat result       = Mat(height, width, CV_8UC3);

	for (size_t i = 0; i < imageSize; i++)
		result.data[i] = image[i];
	return result;
}

void savingImage(std::string DirectoryNameForImages, std::string imageName, Mat image, std::string noLogs, std::string onlyResult, std::string LogMessage) {
	bool result;
	if (onlyResult == "true" && imageName == "/resultImage.jpeg")
		result = imwrite(DirectoryNameForImages + imageName, image);
	else if (onlyResult == "true")
		result = true;
	else if (onlyResult == "false")
		result = imwrite(DirectoryNameForImages + imageName, image);

	if (noLogs == "false") {
		if (result == true)
			std::cout << LogMessage << std::endl;
		else{
			std::cout << "Error with: " << LogMessage << std::endl;
			exit(1);
		}
	}
}

int main(int argc, char **argv) {
	// checkers
	if (argc < 2) {
		std::cout << "usage: ./run <Image path> <options>" << std::endl
		          << "Options:" << std::endl
		          << "\t-noLogs -- does not output anything in terminal" << std::endl
		          << "\t-onlyResult -- saves only result image" << std::endl
		          << "\t-savePath <path> -- saves images in given path" << std::endl
		          << "\t-threadsPerBlock <number> -- sets threads per block" << std::endl;
		return 0;
	}

	std::string filePath = argv[1];
	assert_v(std::filesystem::exists(filePath), "File does not exists");

	Mat image = imread(filePath);
	assert_v(image.total() <= pow(2, 32), "Image is too big");

	// inserting default options
	std::map<std::string, std::string> options;
	options.insert(std::make_pair("-noLogs", "false"));
	options.insert(std::make_pair("-onlyResult", "false"));
	options.insert(std::make_pair("-savePath", "imageProcessing"));
	options.insert(std::make_pair("-threadsPerBlock", "1024"));
	// parsing options
	for (int i = 2; i < argc; i++) {
		std::string option = argv[i];
		auto it            = options.find(option);
		if (it == options.end()) {
			std::cout << "Unknown option: " << option << std::endl;
			return 0;
		}
		if (option == "-threadsPerBlock") {
			i++;
			int threadsPerBlock;
			try {
				threadsPerBlock = std::stoul(argv[i]);
				if (threadsPerBlock > 1024)
					throw("Threads per block must be less than 1024");
				else if (threadsPerBlock == 0)
					throw("Threads per block must be greater than 0");
			} catch (std::string error) {
				std::cout << error << std::endl;
				return 0;
			}
			options[option] = std::to_string(threadsPerBlock);
		} else if (option == "-savePath")
			options[option] = argv[++i];
		else
			options[option] = "true";
	}

	// Configuration variables
	const std::string DirectoryNameForImages = options["-savePath"];
	// image is 2:3
	const size_t rectangleWidth  = 100 * 2;
	const size_t rectangleHeight = 100 * 3;
	// Since in lab given RGB format, and OpenCV get BGR format, we need to rotate
	const cv::Scalar rectangleColor(255, 255, 0);
	const unsigned threshold       = 100;
	const unsigned amountOfThreads = std::stoul(options["-threadsPerBlock"]);

	std::filesystem::create_directory(DirectoryNameForImages);
	// Saving original image
	savingImage(DirectoryNameForImages, "/0.originalImage.jpeg", image, options["-noLogs"], options["-onlyResult"], "Saving original Image");

	// Rotating image
	image = rotateImage(image, 55.0);
	savingImage(DirectoryNameForImages, "/1.rotatedImage.jpeg", image, options["-noLogs"], options["-onlyResult"], "OpenCV rotated image");
	Mat imageCopy = image.clone(); // saving for future use

	// Drawing rectangular
	image  = drawRectangular(image, rectangleColor, rectangleWidth, rectangleHeight);
	savingImage(DirectoryNameForImages, "/2.drawRectangular.jpeg", image, options["-noLogs"], options["-onlyResult"], "OpenCV: rectangular draw");

	// Drawing triangular
	image  = drawTriangular(image, rectangleColor, rectangleWidth, rectangleHeight);
	savingImage(DirectoryNameForImages, "/3.drawTriangular.jpeg", image, options["-noLogs"], options["-onlyResult"], "OpenCV: triangular draw");

	// threshold transformation
	Size size              = image.size();
	short *thresholdResult = performTransform(ConvertMatToShort(image), size.width, size.height, rectangleColor[0], rectangleColor[1], rectangleColor[2], threshold, amountOfThreads);

	// Create a cv::Mat object from the raw image data
	Mat thresholdResultMat = ConvertShortToMat(thresholdResult, size.width, size.height, 3);
	savingImage(DirectoryNameForImages, "/4.thresholdResult.jpeg", thresholdResultMat, options["-noLogs"], options["-onlyResult"], "CUDA: threshold transformation");

	// Hough transformation
	size_t *houghResult = performHough(thresholdResult, size.width, size.height, amountOfThreads);
	assert_v(houghResult != NULL, "Hough transformation failed");
	if(options["-noLogs"] == "false")
		std::cout << "CUDA: hough transformation successfully" << std::endl;

	// Creating image from hough transformation
	imageCopy = deleteArtifactsHough(imageCopy, houghResult, rectangleColor);
	savingImage(DirectoryNameForImages, "/5.HoughRestored.jpeg", imageCopy, options["-noLogs"], options["-onlyResult"], "CUDA + CPU + OpenCV: Image rectangle restored with hough");
	image = imageCopy.clone();

	// Rotating Image
	short *rotatedImage = performRotate(ConvertMatToShort(image), size.width, size.height, amountOfThreads);
	Mat rotateImageMat  = ConvertShortToMat(rotatedImage, size.width, size.height, 3);
	std::string resultImageName  = options["-onlyResult"] == "true" ? "/resultImage.jpeg" : "/6.rotateImage.jpeg";
	savingImage(DirectoryNameForImages, resultImageName, rotateImageMat, options["-noLogs"], options["-onlyResult"], "CUDA: image rotated successfully");

	return 0;
}
