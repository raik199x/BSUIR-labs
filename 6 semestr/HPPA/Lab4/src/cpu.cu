#include "../include/cpu.hpp"
#include "../include/time.hpp"
#include "../include/random.hpp"
#include <ratio>

using namespace Geometry3D;

xyz_type prng(const xyz_type min, const xyz_type max) {
	return Random::Uniform::real<xyz_type>(min, max);
}

bool polyhedron_has_point(const Plane planes[6], const Point& p) {
	for (size_t i = 0; i < 6; ++ i) {
		const xyz_type value = planes[i].a * p.x + planes[i].b * p.y + planes[i].c * p.z + planes[i].d;
		if (value < 0)
			//! \note Does not belong to the Polyhedron
			return false;
	}
	//! \note Does belong to the Polyhedron
	return true;
}

xyz_type CPU::MonteCarlo(const Plane planes[6], const xyz_type prng_limit, const size_t number_of_iterations) {
	//! \note start clock
	const Clock start = Instant::now();

	//! \note Start Monte Carlo method -> iterate N_OF_ITERATIONS times
	size_t counter = 0;
	for (size_t iteration = 0; iteration < number_of_iterations; ++iteration) {
		//! \note Generate a random point in the new cube (random coordinates in the range [-prng_limit, prng_limit])
		const Point point = {
			prng(-prng_limit, prng_limit),
			prng(-prng_limit, prng_limit),
			prng(-prng_limit, prng_limit)
		};
		//! \note Check if the generated point is inside the polyhedron
		if (polyhedron_has_point(planes, point))
			//! \note Aka `Hit` -> the point is inside the polyhedron -> increment the counter
			++counter;
	}
	//! \note Calculate the hit ratio
	const auto ratio = (xyz_type)counter / number_of_iterations;

	//! \note stop clock
	const Clock stop = Instant::now();
	//! \note calculate time duration
	const Duration<std::milli> duration = stop - start;
	cout << "CPU time: " << duration.count() << "ms" << endl;
	return ratio;
}
