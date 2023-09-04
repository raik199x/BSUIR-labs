#pragma once
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>

cv::Mat rotateImage(cv::Mat inputImage, double angle);
cv::Mat drawRectangular(cv::Mat inputImage, cv::Scalar rectangleColor, size_t rectangleWidth, size_t rectangleHeight);
cv::Mat drawTriangular(cv::Mat inputImage, cv::Scalar triangleColor, size_t triangleWidth, size_t triangleHeight);
cv::Mat deleteArtifactsHough(cv::Mat inputImage, size_t *houghResult, cv::Scalar rectangleColor);
