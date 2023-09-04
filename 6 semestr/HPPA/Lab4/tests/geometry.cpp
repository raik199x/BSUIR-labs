#include <cmath>
#include <cassert>

#include "../include/geometry.hpp"

using namespace Geometry3D;

using std::to_string;

/*!
 * \fn approx_equal(const xyz_type a, const xyz_type b) -> bool
 * \brief Check if 2 values are approximately equal
 * \note 2 values are approximately equal if the difference between them is less than EPSILON
 * \note Floating point numbers are not usually exactly equal, so we need to check if they are approximately equal
 * 
 * \param a 1st value
 * \param b 2nd value
 * \return true if the 2 values are approximately equal
 * \return false otherwise
 */
static bool approx_equal(const xyz_type a, const xyz_type b) {
	return std::abs(a - b) < EPSILON;
}
/*!
 * \fn determinant(xyz_type a11, xyz_type a12, xyz_type a13, xyz_type a21, xyz_type a22, xyz_type a23, xyz_type a31, xyz_type a32, xyz_type a33) -> xyz_type
 * \brief Calculate determinant of a 3x3 matrix
 * \note The determinant of a 3x3 matrix is calculated by the following formula:
 * \note det(A) = a11 * a22 * a33 + a21 * a32 * a13 + a12 * a23 * a31 - a31 * a22 * a13 - a11 * a32 * a23 - a12 * a21 * a33
 * 
 * \param a11 1st element of the 1st row
 * \param a12 2nd element of the 1st row
 * \param a13 3rd element of the 1st row
 * \param a21 1st element of the 2nd row
 * \param a22 2nd element of the 2nd row
 * \param a23 3rd element of the 2nd row
 * \param a31 1st element of the 3rd row
 * \param a32 2nd element of the 3rd row
 * \param a33 3rd element of the 3rd row
 * \return Determinant of the 3x3 matrix
 */
xyz_type determinant(xyz_type a11, xyz_type a12, xyz_type a13, xyz_type a21, xyz_type a22, xyz_type a23, xyz_type a31, xyz_type a32, xyz_type a33) {
	return a11 * a22 * a33 + a21 * a32 * a13 + a12 * a23 * a31 - a31 * a22 * a13 - a11 * a32 * a23 - a12 * a21 * a33;
}

/*!
 * \fn Point::collinear(const Point& p1, const Point& p2, const Point& p3) -> bool
 * \brief Check if 3 points are collinear
 * \note 3 points are collinear if they are on the same line which means that they cannot define a plane
 * 
 * \param p1 Point 1
 * \param p2 Point 2
 * \param p3 Point 3
 * \return true if the 3 points are collinear
 * \return false otherwise
 */
bool Point::collinear(const Point& p1, const Point& p2, const Point& p3) {
	Vector v1(p1, p2);
	Vector v2(p1, p3);
	Vector cross = Vector::cross(v1, v2);
	return approx_equal(cross.x, 0) && approx_equal(cross.y, 0) && approx_equal(cross.z, 0);
}
/*!
 * \fn Point::operator==(const Point& p) const -> bool
 * \brief Check if 2 points are equal
 * \note 2 points are equal if they have the same coordinates
 * 
 * \param p Point to compare with
 * \return true if the 2 points are equal
 * \return false otherwise
 */
bool Point::operator==(const Point& p) const {
	return approx_equal(x, p.x) && approx_equal(y, p.y) && approx_equal(z, p.z);
}
/*!
 * \fn Point::operator()(void) const -> string
 * \brief Get a string representation of the point
 * \note The string representation is the form `(x, y, z)`
 * 
 * \return string String representation of the point
 */
string Point::operator()(void) const {
	return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
}

/*!
 * \fn Vector::Vector(const Point& p1, const Point& p2)
 * \brief Construct a new Vector:: Vector object
 * \note Vector is constructed from raw coordinates' values
 * 
 * \param x X coordinate
 * \param y Y coordinate
 * \param z Z coordinate
 * 
 * \warning this constructor is protected and should not be used directly
 */
Vector::Vector(const xyz_type x, const xyz_type y, const xyz_type z) : x(x), y(y), z(z) {};
/*!
 * \fn Vector::Vector(const Point& p1, const Point& p2)
 * \brief Construct a new Vector:: Vector object
 * \note Vector is constructed from 2 points
 * 
 * \param p1 Point 1 - the starting point of the vector
 * \param p2 Point 2 - the ending point of the vector
 */
Vector::Vector(const Point& p1, const Point& p2) : x(p2.x - p1.x), y(p2.y - p1.y), z(p2.z - p1.z) {};
/*!
 * \fn Vector::cross(const Vector& v1, const Vector& v2) -> Vector
 * \brief Calculate the cross product of 2 vectors
 * \note The cross product is a vector perpendicular to both vectors
 * 
 * \param v1 Vector 1
 * \param v2 Vector 2
 * \return Vector Cross product of the 2 vectors
 */
Vector Vector::cross(const Vector& v1, const Vector& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}
/*!
 * \fn Vector::dot(const Vector& v1, const Vector& v2) -> xyz_type
 * \brief Calculate the dot product of 2 vectors
 * \note The dot product is a scalar value
 * 
 * \param v1 Vector 1
 * \param v2 Vector 2
 * \return xyz_type Dot product of the 2 vectors
 */
xyz_type Vector::dot(const Vector& v1, const Vector& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
/*!
 * \fn Vector::magnitude(void) const -> xyz_type
 * \brief Calculate the magnitude of the vector
 * \note The magnitude is the length of the vector
 * 
 * \return xyz_type Magnitude of the vector
 */
xyz_type Vector::magnitude(void) const {
	return sqrt(x * x + y * y + z * z);
}
/*!
 * \fn Vector::normalized(void) const -> Vector
 * \brief Get a normalized vector
 * \note A normalized vector is a vector with a magnitude of 1 and the same direction as the original vector
 * \note Normalization is the process of scaling the vector to have a magnitude of 1
 * 
 * \return Vector Normalized vector
 * 
 * \warning The magnitude of the vector must not be 0, otherwise the program will crash
 */
Vector Vector::normalized(void) const {
	const xyz_type mag = magnitude();
	assert(mag != 0);
	return { x / mag, y / mag, z / mag };
}
string Vector::operator() (void) const {
	return "<" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ">";
}

/*!
 * \fn Plane::Plane(const Point& p1, const Point& p2, const Point& p3)
 * \brief Construct a new Plane:: Plane object
 * \note A plane is defined by 3 points that are not collinear so assertion is used to check that
 * \note If the 3 points are not collinear, then the plane can ve defined and represented by the equation
 * 
 * \param p1 Point 1
 * \param p2 Point 2
 * \param p3 Point 3
 * 
 * \warning The 3 points must not be collinear, otherwise the program will crash
 */
Plane::Plane(const Point& p1, const Point& p2, const Point& p3) {
	if (Point::collinear(p1, p2, p3)) {
		cerr << "! " << p1() << " ! " << p2() << " ! " << p3() << " !" << endl;
		cerr << "! " << Vector::cross(Vector(p1, p2), Vector(p1, p3))() << " !" << endl;
		throw std::invalid_argument("Points are collinear");
	}
	Vector v1 = Vector(p1, p2);
	Vector v2 = Vector(p1, p3);
	Vector v3 = Vector::cross(v1, v2);
	a = v3.x;
	b = v3.y;
	c = v3.z;
	d = -(v3.x * p1.x + v3.y * p1.y + v3.z * p1.z);
}
/*!
 * \fn Plane::has(const Point& p) const -> bool
 * \brief Check if a point is on the plane
 * \note A point is on the plane if the equation of the plane is satisfied
 * 
 * \param p Point to check
 * \return true if the point is on the plane
 * \return false otherwise
 */
bool Plane::has(const Point& p) const {
	return approx_equal(a * p.x + b * p.y + c * p.z + d, 0);
}
/*!
 * \fn Plane::intersect(const Plane& pl1, const Plane& pl2, const Plane& pl3, Point* const p) -> bool
 * \brief Calculate the intersection of 3 planes
 * \note The intersection of 3 planes is a point
 * 
 * \param pl1 Plane 1
 * \param pl2 Plane 2
 * \param pl3 Plane 3
 * \param p Pointer to the point where the intersection will be stored
 * \return true if the 3 planes intersect
 * \return false otherwise
 */
bool Plane::intersect(const Plane& pl1, const Plane& pl2, const Plane& pl3, Point* const p) {
	xyz_type D = determinant(pl1.a, pl1.b, pl1.c, pl2.a, pl2.b, pl2.c, pl3.a, pl3.b, pl3.c);
	if (approx_equal(D, 0))
		return false;
	xyz_type Dx = determinant(-pl1.d, pl1.b, pl1.c, -pl2.d, pl2.b, pl2.c, -pl3.d, pl3.b, pl3.c);
	xyz_type Dy = determinant(pl1.a, -pl1.d, pl1.c, pl2.a, -pl2.d, pl2.c, pl3.a, -pl3.d, pl3.c);
	xyz_type Dz = determinant(pl1.a, pl1.b, -pl1.d, pl2.a, pl2.b, -pl2.d, pl3.a, pl3.b, -pl3.d);
	// p->x = Dx / D;
	// p->y = Dy / D;
	// p->z = Dz / D;
	*p = {Dx / D, Dy / D, Dz / D};
	return true;
}
/*!
 * \fn Plane::operator==(const Plane& pl) const -> bool
 * \brief Check if 2 planes are equal
 * \note 2 planes are equal if they have the same coefficients
 * 
 * \param pl Plane to compare
 * \return true if the planes are equal
 * \return false otherwise
 */
bool Plane::operator==(const Plane& pl) const {
	return approx_equal(a, pl.a) && approx_equal(b, pl.b) && approx_equal(c, pl.c) && approx_equal(d, pl.d);
}
/*!
 * \fn Plane::operator()(void) const -> std::string
 * \brief Get a string representation of the plane
 * \note The string representation is the equation of the plane
 * 
 * \return std::string String representation of the plane
 */
string Plane::operator() (void) const {
	return to_string(a) + "x + " + to_string(b) + "y + " + to_string(c) + "z + " + to_string(d) + " = 0";
}
/*!
 * \fn Plane::Plane(const xyz_type a, const xyz_type b, const xyz_type c, const xyz_type d)
 * \brief Fill the coefficients of the plane's equation with the given raw values
 * \note Plane's equation: ax + by + cz + d = 0
 * 
 * \param a a coefficient
 * \param b b coefficient
 * \param c c coefficient
 * \param d d coefficient
 * 
 * \warning This is a protected constructor, should not be used directly
 */
Plane::Plane(const xyz_type a, const xyz_type b, const xyz_type c, const xyz_type d) : a(a), b(b), c(c), d(d) {};

/*!
 * \fn Line::has(const Point& p) const -> bool
 * \brief Check if a point belongs to the line
 * 
 * \param p Point to check
 * \return true if the point belongs to the line
 * \return false otherwise
 * 
 * \warning Points must be collinear, otherwise the program will crash
 * \warning It might be a good idea to check if the point is collinear before calling this function
*/
bool Line::has(const Point& p) const {
	assert(Point::collinear(p1, p2, p));
	const auto min_x = std::min(p1.x, p2.x);
	const auto max_x = std::max(p1.x, p2.x);
	const auto min_y = std::min(p1.y, p2.y);
	const auto max_y = std::max(p1.y, p2.y);
	const auto min_z = std::min(p1.z, p2.z);
	const auto max_z = std::max(p1.z, p2.z);
	return min_x <= p.x && p.x <= max_x &&
		   min_y <= p.y && p.y <= max_y &&
		   min_z <= p.z && p.z <= max_z;
}
/*!
 * \fn Line::project(const Point& p) const -> Point
 * \brief Project a point on the line
 * 
 * \param p Point to project
 * \return Point Projected point
 * 
 * \warning The dot product of Vector(p1, p2) and Vector(p1, p2) must not be 0, otherwise the program will crash
 */
Point Line::project(const Point& p) const {
	Vector v1 = Vector(p1, p2);
	Vector v2 = Vector(p1, p);
	auto divisor = Vector::dot(v1, v1);
	assert(divisor != 0);
	xyz_type t = Vector::dot(v1, v2) / divisor;
	return { p1.x + t * v1.x, p1.y + t * v1.y, p1.z + t * v1.z };
}
/*!
 * \fn Line::operator==(const Line& l) const -> bool
 * \brief Check if 2 lines are equal
 * \note 2 lines are equal if they have the same points, in any order
 * 
 * \param l Line to compare
 * \return true if the lines are equal
 * \return false otherwise
 */
bool Line::operator==(const Line& l) const {
	return (p1 == l.p1 && p2 == l.p2) || (p1 == l.p2 && p2 == l.p1);
}
/*!
 * \fn Line::operator()(void) const -> string
 * \brief Return a string representation of the line
 * \note The string representation is in the form [p1, p2]
 * 
 * \return string String representation of the line
 */
string Line::operator()(void) const {
	return "[" + p1() + ", " + p2() + "]";
}
/*!
 * \fn Line::length(void) const -> xyz_type
 * \brief Return the length of the line
 * 
 * \return xyz_type Length of the line
 */
xyz_type Line::length(void) const {
	return Vector(p1, p2).magnitude();
}

/*!
 * \fn Polyhedron::Polyhedron(const std::vector<Plane>& planes)
 * \brief Construct a new Polyhedron::Polyhedron object
 * 
 * \param planes Planes that define the polyhedron
 * 
 * \todo Add an assert to check if the planes are coplanar
 */
Polyhedron::Polyhedron(const std::vector<Plane>& planes) : planes(planes) {};
/*!
 * \fn Polyhedron::Polyhedron(const std::initializer_list<Plane>& planes)
 * \brief Construct a new Polyhedron::Polyhedron object
 * 
 * \param planes Planes that define the polyhedron
 * 
 * \todo Add an assert to check if the planes are coplanar
 */
Polyhedron::Polyhedron(const std::initializer_list<Plane>& planes) : planes(planes) {};
/*!
 * \fn Polyhedron::vertices(void) const -> std::vector<Point>
 * \brief Return the vertices of the polyhedron
 * \note The vertices are the intersection of 3 planes of the polyhedron
 * 
 * \return std::vector<Point> Vertices of the polyhedron (no duplicates)
 */
std::vector<Point> Polyhedron::vertices(void) const {
	std::vector<Point> result;
	std::vector<std::pair<Point, size_t>> tmp;
	const size_t n = planes.size();
	for (size_t i = 0; i < n; ++i)
		for (size_t j = i + 1; j < n; ++j)
			for (size_t k = j + 1; k < n; ++k) {
				Point p = ORIGIN;
				if (Plane::intersect(planes[i], planes[j], planes[k], &p)) {
					if (!planes[i].has(p) || !planes[j].has(p) || !planes[k].has(p)) {
						cerr << "! " << p() << " !" << endl
							 << "i " << planes[i].a * p.x + planes[i].b * p.y + planes[i].c * p.z + planes[i].d << endl
							 << "j " << planes[j].a * p.x + planes[j].b * p.y + planes[j].c * p.z + planes[j].d << endl
							 << "k " << planes[k].a * p.x + planes[k].b * p.y + planes[k].c * p.z + planes[k].d << endl;
						throw std::runtime_error("Invalid point solution");
					}
					if (p.operator==({0, 0, -1}) || p.operator==({0, 0, 1}))
						cout << "SUS: " << planes[i]() << " " << planes[j]() << " " << planes[k]() << endl;
					bool found = false;
					for (auto& point : tmp)
						if (point.first == p) {
							found = true;
							++point.second;
						}
					if (!found)
						tmp.push_back(std::make_pair(p, 1));
				}
			}
	for (auto& point : tmp)
		//if (point.second == 1)
			result.push_back(point.first);
	return result;
}
/*!
 * \fn Polyhedron::mesh(void) const -> std::vector<Line>
 * \brief Return the mesh of the polyhedron
 * \note The mesh is the set of all the lines that connect the vertices of the polyhedron
 * 
 * \return std::vector<Line> Mesh of the polyhedron
 */
std::vector<Line> Polyhedron::mesh(void) const {
	auto points = vertices();
	const size_t num_of_vertices = points.size();
	std::vector<Line> result;
	for (size_t i = 0; i < num_of_vertices; ++i)
		for (size_t j = i + 1; j < num_of_vertices; ++j)
			result.push_back({ points[i], points[j] });
	return result;
}
/*!
 * \fn Polyhedron::most_distant(void) const -> Point
 * \brief Return the most distant vertex of the polyhedron from the origin
 * \note The most distant vertex is the one that is the farthest from the origin
 * \note This data is used to calculate limits of the PRNG (pseudo-random number generator) for Monte-Carlo method
 * 
 * \return Point Most distant vertex of the polyhedron from the origin
 */
Point Polyhedron::most_distant(void) const {
	const std::vector<Point> points = vertices();
	Point result = ORIGIN;
	xyz_type max_distance = 0;
	for (const auto& point : points) {
		const auto distance = Line({ORIGIN, point}).length();
		if (distance <= max_distance)
			continue;
		max_distance = distance;
		result = point;
	}
	return result;
}

void Polyhedron::calibrate(const Point& p) {
	for (auto& plane : planes) {
		const auto value = plane.a * p.x + plane.b * p.y + plane.c * p.z + plane.d;
		if (value > 0)
			continue;
		plane.a *= -1;
		plane.b *= -1;
		plane.c *= -1;
		plane.d *= -1;
	}
}

void Polyhedron::calibrate(void) {
	const auto points = vertices();
	Point calibrator = ORIGIN;
	for (const auto& point : points)
		calibrator = {calibrator.x + point.x, calibrator.y + point.y, calibrator.z + point.z};
	const xyz_type n = points.size();
	calibrator = {calibrator.x / n, calibrator.y / n, calibrator.z / n};
	calibrate(calibrator);
}

bool Polyhedron::has(const Point& p) const {
	for (const auto& plane : planes) {
		// Plane is `ax + by + cz + d = 0`
		const auto value = plane.a * p.x + plane.b * p.y + plane.c * p.z + plane.d;
		if (value < 0)
			return false;
	}
	return true;
}
