#include <criterion/criterion.h>

#include "../include/time.hpp"
#include "../include/global.hpp"

#define TEST_CLOCK false

Test(Instant, test, .disabled = !TEST_CLOCK) {
	const auto start = Instant::now();
	sleep(1);
	const auto stop = Instant::now();
	const Duration<std::milli> period = stop - start;
	cout << period.count() << " ms" << endl;
	sleep(1);
}
