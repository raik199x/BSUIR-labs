#pragma once
#include "geometry.hpp"
#include "global.hpp"
#include <array>

namespace GPU {
struct Plane {
	xyz_type a, b, c, d;
};

xyz_type initCudaCalc(const Geometry3D::Plane planes[6], const xyz_type prng_limit, size_t AmountOfThreads);
std::array<size_t, 2> calculateDim(size_t AmountOfThreads);
void outputCudaInfo(void);
} // namespace GPU