#pragma once

#include "geometry.hpp"

/*!
 * \namespace MonteCarlo
 * \brief Monte-Carlo method for polyhedron volume calculation
 */
namespace MonteCarlo {
	using namespace Geometry3D;

	const Polyhedron Tetrahedron(const Point& A, const Point& B, const Point& C, const Point& D);
	const Polyhedron Hexahedron(const Point& A, const Point& B, const Point& C, const Point& E, const Point& F, const Point& G);
	const Polyhedron Pentahedron(const Point& A, const Point& B, const Point& C, const Point& D, const Point S);
	xyz_type method(const Polyhedron &polyhedron);
}

/*!
 * \namespace Canonical
 * \brief Canonical method for polyhedron volume calculation
 */
namespace Canonical {
	using namespace Geometry3D;

	xyz_type Tetrahedron(const Point& A, const Point& B, const Point& C, const Point& D);
	xyz_type Hexahedron(const std::vector<Point>& vertices);
	xyz_type Pentahedron(const std::vector<Point>& vertices);
}
