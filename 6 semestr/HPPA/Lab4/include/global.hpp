#pragma once

#include <iostream>

//! \note Type for x, y, z coordinates
using xyz_type = float;
//! \note Number of planes in polyhedron
constexpr static const size_t N_OF_PLANES = 6;
//! \note Epsilon for floating point comparison
constexpr static const xyz_type EPSILON = 1e-5;
//! \note Multiplier coefficient for the limit
constexpr static const xyz_type MULTIPLIER_COEFFICIENT = 1.0078125;

// Amount of points to randomly generate
constexpr static const size_t N_OF_ITERATIONS = (size_t)1 << 17;

//! \note The most frequent used elements of the standard library

using std::string;
constexpr static const auto endl = '\n';
using std::cout, std::cerr;
