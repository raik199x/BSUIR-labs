#include <criterion/criterion.h>

#include <array>

#include "../include/global.hpp"
#include "../include/random.hpp"

#define TEST_DISTRIBUTION false

using namespace Random;

//! \note test the correctness of the pseudo-random number generator
Test(Uniform, Distribution, .disabled = !TEST_DISTRIBUTION) {
	const size_t N = pow(2, 21);
	const size_t LIMIT = 10;
	std::array<size_t, LIMIT> histogram;

	//! \note Test Integral Uniform PRNG
	histogram.fill(0);
	for (size_t i = 0; i < N; ++i) {
		const auto index = Uniform::integral<size_t>(0, LIMIT - 1);
		++histogram[index];
	}
	for (size_t i = 0; i < LIMIT; ++i) {
		const auto p = (double)histogram[i] / N * 100;
		cout << i << ": " << p << "%" << endl;
	}
	sleep(1);

	cout << endl;

	//! \note Test Real Uniform PRNG
	histogram.fill(0);
	for (size_t i = 0; i < N; ++i) {
		const auto index = std::round(Uniform::real<double>(0, LIMIT - 1));
		++histogram[index];
	}
	for (size_t i = 0; i < LIMIT; ++i) {
		const auto p = (double)histogram[i] / N * 100;
		cout << i << ": " << p << "%" << endl;
	}
	sleep(1);
}
