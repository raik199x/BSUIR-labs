#pragma once

#include <chrono>

//! \brief Convenient std::chrono::high_resolution_clock alias
using Instant = std::chrono::high_resolution_clock;

//! \brief Convenient std::chrono::high_resolution_clock::time_point alias
using Clock = std::chrono::high_resolution_clock::time_point;

//! \brief The template for convenient std::chrono::duration<long double, T> conversion
//! \tparam T Type of the time point
template <typename duration_type>
using Duration = std::chrono::duration<long double, duration_type>;
