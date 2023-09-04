#pragma once

#include <vector>
#include <string>

#include "global.hpp"

#ifndef FULL_ACCESS
#define ACCESS protected
#else
#define ACCESS public
#endif

/*!
 * \namespace Geometry3D
 * \brief Namespace for all geometry related structures, constants and functions
 */
namespace Geometry3D {
	//! \brief Structures and functions

	/*!
	 * \struct Point
	 * \brief Structure for a 3D point, describes point's math and behaviour
	 * \note A point is a set of 3 coordinates (x, y, z)
	 */
	struct Point {
		xyz_type x, y, z;
		static bool collinear(const Point& p1, const Point& p2, const Point& p3);
		bool operator==(const Point& p) const;
		string operator() (void) const;
	};

	/*!
	 * \struct Vector
	 * \brief Structure for a 3D vector, describes vector's math and behaviour
	 * \note A vector is a set of 3 coordinates (x, y, z), but it is not a point more like a direction. Can be set from 2 points
	 */
	struct Vector {
		xyz_type x, y, z;
		Vector(const Point& p1, const Point& p2);
		static Vector cross(const Vector& v1, const Vector& v2);
		static xyz_type dot(const Vector& v1, const Vector& v2);
		xyz_type magnitude(void) const;
		Vector normalized(void) const;
		string operator() (void) const;

	ACCESS:
		Vector(const xyz_type x, const xyz_type y, const xyz_type z);
	};

	/*!
	 * \struct Plane
	 * \brief Structure for a 3D plane, describes plane's math and behaviour
	 * \note A plane is an infinite surface, defined by 3 points, and it is represented by the equation ax + by + cz + d = 0
	 */
	struct Plane {
		xyz_type a, b, c, d;
		Plane(const Point& p1, const Point& p2, const Point& p3);
		bool has(const Point& p) const;
		static bool intersect(const Plane& pl1, const Plane& pl2, const Plane& pl3, Point* const p);
		bool operator==(const Plane& pl) const;
		string operator() (void) const;

	ACCESS:
		Plane(const xyz_type a, const xyz_type b, const xyz_type c, const xyz_type d);
	};

	/*!
	 * \struct Line
	 * \brief Structure for a 3D line, describes line's math and behaviour
	 * \note A line is an infinite set of point that lie on a straight line between 2 points
	 */
	struct Line {
		Point p1, p2;
		bool has(const Point& p) const;
		Point project(const Point& p) const;
		bool operator==(const Line& l) const;
		string operator() (void) const;
		xyz_type length(void) const;
	};

	/*!
	 * \struct Polyhedron
	 * \brief Structure for a 3D polyhedron, describes polyhedron's math and behaviour
	 * \note A polyhedron is a solid figure bounded by flat surfaces, defined by a set of planes (surfaces)
	 */
	struct Polyhedron {
		std::vector<Plane> planes;
		Polyhedron(const std::vector<Plane>& planes);
		Polyhedron(const std::initializer_list<Plane>& planes);
		std::vector<Point> vertices(void) const;
		std::vector<Line> mesh(void) const;
		Point most_distant(void) const;

		void calibrate(void);
		bool has(const Point& p) const;

	ACCESS:
		void calibrate(const Point& p);
	};
}

namespace Geometry3D {
	//! \brief Constants

	//! \brief ORIGIN is the origin point (0, 0, 0)
	constexpr static const Point ORIGIN = {0, 0, 0};
	//! \brief ORIGIN_X is the line that goes from the origin to the point (1, 0, 0) - aka the x axis
	constexpr static const Line ORIGIN_X = {ORIGIN, {1, 0, 0}};
	//! \brief ORIGIN_Y is the line that goes from the origin to the point (0, 1, 0) - aka the y axis
	constexpr static const Line ORIGIN_Y = {ORIGIN, {0, 1, 0}};
	//! \brief ORIGIN_Z is the line that goes from the origin to the point (0, 0, 1) - aka the z axis
	constexpr static const Line ORIGIN_Z = {ORIGIN, {0, 0, 1}};
}
