#include <opencv2/core/types.hpp>

#include "../include/opencvFunctions.h"

cv::Mat rotateImage(cv::Mat inputImage, double angle) {
	double radians = angle * CV_PI / 180.0;                           // converting to radians
	cv::Point2f center(inputImage.cols / 2.0, inputImage.rows / 2.0); // Get the center of rotation

	// Create the rotation matrix
	cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);

	// Perform the rotation
	cv::Mat rotatedImage;
	cv::warpAffine(inputImage, rotatedImage, rotationMatrix, inputImage.size());

	return rotatedImage;
}

cv::Mat drawRectangular(cv::Mat inputImage, cv::Scalar rectangleColor, size_t rectangleWidth, size_t rectangleHeight) {
	// Get the center of the image
	cv::Point center(inputImage.cols / 2, inputImage.rows / 2);

	// Calculate the coordinates of the top-left and bottom-right corners of the rectangle
	cv::Point topLeft(center.x - rectangleWidth / 2, center.y - rectangleHeight / 2);
	cv::Point bottomRight(center.x + rectangleWidth / 2, center.y + rectangleHeight / 2);

	// Draw the rectangle on the image
	cv::rectangle(inputImage, topLeft, bottomRight, rectangleColor, 1, cv::LINE_AA);

	return inputImage;
}

cv::Mat drawTriangular(cv::Mat inputImage, cv::Scalar triangleColor, size_t triangleWidth, size_t triangleHeight) {
	// Get the height and width of the image
	int height = inputImage.rows;
	int width  = inputImage.cols;

	// Calculate the coordinates of the triangle vertices
	cv::Point pt1(inputImage.cols / 2, inputImage.rows / 2);
	cv::Point pt2(width - 2, height - 2); // Right bottom
	cv::Point pt3(2, height - 2);         // Left bottom

	// Draw the triangle by connecting the points with lines
	cv::line(inputImage, pt1, pt2, triangleColor, 1, cv::LINE_AA);
	cv::line(inputImage, pt2, pt3, triangleColor, 1, cv::LINE_AA);
	cv::line(inputImage, pt3, pt1, triangleColor, 1, cv::LINE_AA);

	return inputImage;
}

cv::Mat deleteArtifactsHough(cv::Mat inputImage, size_t *houghResult, cv::Scalar rectangleColor) {
	// Get the height and width of the image
	size_t height = inputImage.rows;
	size_t width  = inputImage.cols;

	// since houghResult[0] is taken from one dimensional array, we need to understand in which line it was taken
	// of course using proportional values
	size_t y = (size_t)ceil((double)(houghResult[0] * height) / (width * height));
	size_t x = (size_t)ceil((double)(houghResult[0] * width) / (width * height));

	// Calculate the coordinates of the triangle vertices
	cv::Point pt1(x, y);
	cv::Point pt2(x + houghResult[1], y);
	cv::Point pt3(x, y + houghResult[2]);
	cv::Point pt4(x + houghResult[1], y + houghResult[2]);

	// Draw the triangle by connecting the points with lines
	cv::line(inputImage, pt1, pt2, rectangleColor, 1, cv::LINE_AA);
	cv::line(inputImage, pt2, pt4, rectangleColor, 1, cv::LINE_AA);
	cv::line(inputImage, pt4, pt3, rectangleColor, 1, cv::LINE_AA);
	cv::line(inputImage, pt3, pt1, rectangleColor, 1, cv::LINE_AA);

	return inputImage;
}
