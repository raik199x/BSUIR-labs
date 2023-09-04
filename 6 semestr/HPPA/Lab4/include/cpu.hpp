#pragma once

#include <chrono>
#include <random>
#include <array>

#include "geometry.hpp"
#include "global.hpp"

namespace CPU {
	using namespace Geometry3D;
	xyz_type MonteCarlo(const Plane planes[6], const xyz_type prng_limit, const size_t number_of_iterations);
}
