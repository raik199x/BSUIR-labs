#pragma once

#ifndef __cplusplus
#error "This is a C++ header file"
#endif

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

constexpr static const std::int32_t EXIT_PANIC = 101;

/*!
 * \brief Expects condition to be true, otherwise - prints message
 * \param condition condition (expected too be true)
 * \param message error (failure) message
 * \return condition
 */
inline auto expect_v = [](const bool condition, const auto message) {
	if (!condition)
		std::cerr << message << std::endl;
	return condition;
};
/*!
 * \brief Expects condition to be true, otherwise - prints message and terminates the program with EXIT_PANIC code
 * \param condition condition (expected too be true)
 * \param message error (failure) message
 */
inline auto assert_v = [](const bool condition, const auto message) {
	if (!expect_v(condition, message))
		std::exit(EXIT_PANIC);
};

using namespace cv;
short *ConvertMatToShort(Mat inputImage);
Mat ConvertShortToMat(short *image, size_t width, size_t height, short channels);
