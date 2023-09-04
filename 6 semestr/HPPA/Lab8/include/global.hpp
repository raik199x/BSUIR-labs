#pragma once

#include <iostream>
#include <filesystem>

#define EXIT_PANIC 101

inline auto expect_v = [](const bool condition, const auto sender, const auto message) -> bool {
	if (!condition)
		std::cerr << "Error (" << sender << "): " << message << std::endl;
	return condition;
};

inline auto assert_v = [](const bool condition, const auto sender, const auto message) -> void {
	if (!expect_v(condition, sender, message))
		std::exit(EXIT_PANIC);
};

inline auto panic_v = [](const auto sender, const auto message) -> void {
	assert_v(false, sender, message);
};

using std::string;
using std::cin, std::cout, std::cerr, std::clog;

inline static const auto endl = '\n';

namespace fs = std::filesystem;
