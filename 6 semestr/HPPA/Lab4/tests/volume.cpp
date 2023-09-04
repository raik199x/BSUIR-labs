#include <cmath>

#include "../include/time.hpp"
#include "../include/volume.hpp"
#include "../include/random.hpp"

using namespace Geometry3D;

/*!
 * \fn Tetrahedron(const Point& A, const Point& B, const Point& C, const Point& D) -> const Polyhedron
 * \brief Construct a tetrahedron from 4 points
 * \param A First point
 * \param B Second point
 * \param C Third point
 * \param D Fourth point
 * \return Tetrahedron
 */
const Polyhedron MonteCarlo::Tetrahedron(const Point& A, const Point& B, const Point& C, const Point& D) {
	//! \note construct planes from points
	Plane ABC = {A, B, C};
	Plane ABD = {A, B, D};
	Plane BCD = {B, C, D};
	Plane CAD = {C, A, D};
	//! \note construct polyhedron from planes
	Polyhedron tetrahedron = {ABC, ABD, BCD, CAD};
	tetrahedron.calibrate();
	return tetrahedron;
}

/*!
 * \fn Hexahedron(const Point& A, const Point& B, const Point& C, const Point& E, const Point& F, const Point& G) -> const Polyhedron
 * \brief Construct a hexahedron from 6 points
 * \param A First point
 * \param B Second point
 * \param C Third point
 * \param E Fourth point
 * \param F Fifth point
 * \param G Sixth point
 * \return Hexahedron
 */
const Polyhedron MonteCarlo::Hexahedron(const Point& A, const Point& B, const Point& C, const Point& E, const Point& F, const Point& G) {
	//! \note construct planes from points
	Plane ABC = {A, B, C};
	Plane ABF = {A, B, F};
	Plane ACG = {A, C, G};
	Plane FEG = {F, E, G};
	Plane EGC = {E, G, C};
	//! \note Should be EFD plane, but point D was not given
	//! \note in the task description so I used point B from
	//! \note the same plane instead. It's not a problem.
	Plane EFB = {E, F, B};
	//! \note construct polyhedron from planes
	Polyhedron hexahedron = {ABC, ABF, ACG, FEG, EGC, EFB};
	hexahedron.calibrate();
	return hexahedron;
}

const Polyhedron MonteCarlo::Pentahedron(const Point& A, const Point& B, const Point& C, const Point& D, const Point S) {
	//! \note construct planes from points
	Plane ABC = {A, B, C};
	Plane ABS = {A, B, S};
	Plane BCS = {B, C, S};
	Plane CDS = {C, D, S};
	Plane DAS = {D, A, S};
	//! \note construct polyhedron from planes
	Polyhedron pentahedron = {ABC, ABS, BCS, CDS, DAS};
	pentahedron.calibrate();
	return pentahedron;
}

/*!
 * \fn get_max_coordinate(const Point& point) -> xyz_type
 * \brief Returns maximum coordinate of the given point.
 * 
 * \param point Point to get maximum coordinate of.
 * \return Maximum coordinate of the given point.
 */
xyz_type get_max_coordinate(const Point& point) {
	return std::max(std::abs(point.x), std::max(std::abs(point.y), std::abs(point.z)));
}

/*!
 * \fn method(const Polyhedron& polyhedron) -> xyz_type
 * \brief Calculates volume of the given polyhedron using Monte Carlo method.
 * 
 * \param polyhedron Polyhedron to calculate volume of.
 * \return Volume of the given polyhedron.
 */
xyz_type MonteCarlo::method(const Polyhedron &polyhedron) {
	//! \note start clock
	const Clock start = Instant::now();

	//! \note Find the most distant vertex from the ORIGIN, aka (0, 0, 0)
	const auto most_distant_vertex = polyhedron.most_distant();
	cout << "\tMost distant vertex: " << most_distant_vertex() << endl;
	//! \note Calculate the limit of PRN generation (half of the cube's side)
	const auto limit = get_max_coordinate(most_distant_vertex);
	cout << "\tLimit: " << limit << endl;
	//! \note Multiply the limit by the coefficient to get the new limit (to avoid the case when the original volume equals the new one, happens when the polyhedron is a cube)
	const auto new_limit = limit * MULTIPLIER_COEFFICIENT;
	cout << "\tNew limit: " << new_limit << endl;
	//! \note Calculate the side of the new cube
	const auto cube_side = 2 * new_limit;
	//! \note Calculate the volume of the new cube
	const auto new_volume = cube_side * cube_side * cube_side;

	//! \note Start Monte Carlo method -> iterate N_OF_ITERATIONS times
	size_t counter = 0;
	for (size_t iteration = 0; iteration < N_OF_ITERATIONS; ++iteration) {
		//! \note Generate a random point in the new cube (random coordinates in the range [-new_limit, new_limit])
		const Point point =  {
			Random::Uniform::real<xyz_type>(-new_limit, new_limit),
			Random::Uniform::real<xyz_type>(-new_limit, new_limit),
			Random::Uniform::real<xyz_type>(-new_limit, new_limit)
		};
		//! \note Check if the generated point is inside the polyhedron
		if (polyhedron.has(point))
			//! \note Aka `Hit` -> the point is inside the polyhedron -> increment the counter
			++counter;
	}
	//! \note Calculate the volume of the polyhedron using the counter and the volume of the new cube
	const auto hit_ratio = (xyz_type)counter / N_OF_ITERATIONS;

	//! \note stop clock
	const Clock stop = Instant::now();
	//! \note print the time duration
	const Duration<std::milli> duration = stop - start;
	cout << "\tDuration: " << duration.count() << " ms" << endl;
	cout << "\tHit ratio: " << hit_ratio * 100.0l << "%" << endl;
	return new_volume * hit_ratio;
}

xyz_type Canonical::Tetrahedron(const Point& a, const Point& b, const Point& c, const Point& d) {
	//! \note Calculate the volume of the tetrahedron
	//! \note Find Vectors AD, AB, AC
	const Vector ad = {a, d};
	const Vector ab = {a, b};
	const Vector ac = {a, c};
	//! \note Calculate the volume of the tetrahedron
	const Vector cross = Vector::cross(ab, ac);
	const auto dot = Vector::dot(ad, cross);
	const auto volume = dot / 6;
	return volume < 0 ? -volume : volume;
}

xyz_type Canonical::Hexahedron(const std::vector<Point>& vertices) {
	//! \note Calculate the volume of the hexahedron
	const size_t n = vertices.size();
	xyz_type volume = 0;
	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < n; ++j)
			for (size_t k = 0; k < n; ++k)
				for (size_t l = 0; l < n; ++l) {
					if (vertices[i] == vertices[j] || vertices[i] == vertices[k] || vertices[i] == vertices[l] ||
						vertices[j] == vertices[i] || vertices[j] == vertices[k] || vertices[j] == vertices[l] ||
						vertices[k] == vertices[i] || vertices[k] == vertices[j] || vertices[k] == vertices[l] ||
						vertices[l] == vertices[i] || vertices[l] == vertices[j] || vertices[l] == vertices[k])
						continue;
					if (Plane(vertices[j], vertices[k], vertices[l]).has(vertices[i]) ||
						Plane(vertices[i], vertices[k], vertices[l]).has(vertices[j]) ||
						Plane(vertices[i], vertices[j], vertices[l]).has(vertices[k]) ||
						Plane(vertices[i], vertices[j], vertices[k]).has(vertices[l]))
						continue;
					volume += Tetrahedron(vertices[i], vertices[j], vertices[k], vertices[l]);
				}
	return volume / 240;
}

xyz_type Canonical::Pentahedron(const std::vector<Point>& vertices) {
	//! \note Calculate the volume of the pyramid
	const size_t n = vertices.size();
	xyz_type volume = 0;
	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < n; ++j)
			for (size_t k = 0; k < n; ++k)
				for (size_t l = 0; l < n; ++l) {
					if (vertices[i] == vertices[j] || vertices[i] == vertices[k] || vertices[i] == vertices[l] ||
						vertices[j] == vertices[i] || vertices[j] == vertices[k] || vertices[j] == vertices[l] ||
						vertices[k] == vertices[i] || vertices[k] == vertices[j] || vertices[k] == vertices[l] ||
						vertices[l] == vertices[i] || vertices[l] == vertices[j] || vertices[l] == vertices[k])
						continue;
					if (Plane(vertices[j], vertices[k], vertices[l]).has(vertices[i]) ||
						Plane(vertices[i], vertices[k], vertices[l]).has(vertices[j]) ||
						Plane(vertices[i], vertices[j], vertices[l]).has(vertices[k]) ||
						Plane(vertices[i], vertices[j], vertices[k]).has(vertices[l]))
						continue;
					volume += Tetrahedron(vertices[i], vertices[j], vertices[k], vertices[l]);
				}
	return volume / 48;
}
