#pragma once

#include <random>

namespace Random::Uniform
{
	template<typename T>
	inline T real(const T min, const T max) {
		std::random_device rdev;
		return std::uniform_real_distribution<T>(std::min(min, max), std::max(min, max))(rdev);
	}
	template<typename T>
	inline T integral(const T min, const T max) {
		std::random_device rdev;
		return std::uniform_int_distribution<T>(std::min(min, max), std::max(min, max))(rdev);
	}
}
