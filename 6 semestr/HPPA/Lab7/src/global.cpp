#include "../include/global.hpp"

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