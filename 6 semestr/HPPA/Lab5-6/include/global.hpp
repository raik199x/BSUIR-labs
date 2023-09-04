#pragma once

#ifndef __cplusplus
#error "This is a C++ only header file."
#endif

#include <iostream>

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
	if (!expect_v(condition, message)) {
		std::cerr << "Cuda last error: " << cudaGetErrorString(cudaGetLastError()) << std::endl;
		std::exit(EXIT_PANIC);
	}
};
