#include <criterion/criterion.h>

#include <array>

#include "../include/volume.hpp"
#include "../include/random.hpp"

using namespace Geometry3D;

#define TEST_CALIBRATION true

#define TEST_TETRAHEDRON true
#define TEST_HEXAHEDRON true
#define TEST_RANDOM true
#define TEST_VERTICES false

void test_hexahedron(const Point& A, const Point& B, const Point& C, const Point& E, const Point& F, const Point& G) {
	const Polyhedron polyhedron = MonteCarlo::Hexahedron(A, B, C, E, F, G);
	const auto canonical = Canonical::Hexahedron(polyhedron.vertices());
	cout << "Canonical: " << canonical << endl;
	const auto monte_carlo = MonteCarlo::method(polyhedron);
	cout << "Volume: " << monte_carlo << endl;
	const auto error = (canonical - monte_carlo) / monte_carlo * 100;
	cout << "Error: " << error << "%" << endl;
}

void test_tetrahedron(const Point& A, const Point& B, const Point& C, const Point& D) {
	const Polyhedron polyhedron = MonteCarlo::Tetrahedron(A, B, C, D);
	const auto canonical = Canonical::Tetrahedron(A, B, C, D);
	cout << "Canonical: " << canonical << endl;
	const auto monte_carlo = MonteCarlo::method(polyhedron);
	cout << "Volume: " << monte_carlo << endl;
	const auto error = (canonical - monte_carlo) / monte_carlo * 100;
	cout << "Error: " << error << "%" << endl;
}

#define Variant(number) std::cout << "Variant " << number << ":" << std::endl;

//! \note Example of how to use the Monte Carlo method to calculate the volume of a Hexahedron.
Test(MonteCarlo, v1, .disabled = !TEST_HEXAHEDRON) {
	Variant("1") {
		const Point A = {-1, -1, -1};
		const Point B = {-1,  1, -1};
		const Point C = { 1, -1, -1};
		const Point E = { 1,  1,  1};
		const Point F = {-1,  1,  1};
		const Point G = { 1, -1,  1};
		test_hexahedron(A, B, C, E, F, G);
	}
}

//! \note Example of how to use the Monte Carlo method to calculate the volume of a Hexahedron.
Test(MonteCarlo, v1_2, .disabled = !TEST_HEXAHEDRON) {
	Variant("1 * 2") {
		const Point A = {-2, -2, -2};
		const Point B = {-2,  2, -2};
		const Point C = { 2, -2, -2};
		const Point E = { 2,  2,  2};
		const Point F = {-2,  2,  2};
		const Point G = { 2, -2,  2};
		test_hexahedron(A, B, C, E, F, G);
	}
}
Test(MonteCarlo, v2, .disabled = !TEST_TETRAHEDRON) {
	Variant("2") {
		//const Point A(0, 0, -1), B(-1, 0, 0), C(0, -1, 0), D(1, 1, 1);
		const Point A = {0, 0, -1};
		const Point B = {-1, 0, 0};
		const Point C = {0, -1, 0};
		const Point D = {1, 1, 1};
		test_tetrahedron(A, B, C, D);
	}
}
Test(MonteCarlo, v3, .disabled = !TEST_TETRAHEDRON) {
	Variant("3") {
		//const Point A(0, 0, -0.7), B(-1.5, 0, 0), C(0, -0.2, 0), D(0.3, 0.4, 0.5);
		const Point A = {0, 0, -0.7};
		const Point B = {-1.5, 0, 0};
		const Point C = {0, -0.2, 0};
		const Point D = {0.3, 0.4, 0.5};
		test_tetrahedron(A, B, C, D);
	}
}
Test(MonteCarlo, v4, .disabled = !TEST_TETRAHEDRON) {
	Variant("4") {
		//const Point A(0.4, 0.5, -0.05), B(0.2, -1, -0.2), C(-2, -0.3, -0.5), D(-0.1, 0.05, 0.3);
		const Point A = {0.4, 0.5, -0.05};
		const Point B = {0.2, -1, -0.2};
		const Point C = {-2, -0.3, -0.5};
		const Point D = {-0.1, 0.05, 0.3};
		test_tetrahedron(A, B, C, D);
	}
}
Test(MonteCarlo, v5, .disabled = !TEST_TETRAHEDRON) {
	Variant("5") {
		//const Point A(0.6, 0.7, -0.1), B(0.1, -1.3, -0.27), C(-2.1, 0.4, -0.5), D(0.3, 0.1, 0.4);
		const Point A = {0.6, 0.7, -0.1};
		const Point B = {0.1, -1.3, -0.27};
		const Point C = {-2.1, 0.4, -0.5};
		const Point D = {0.3, 0.1, 0.4};
		test_tetrahedron(A, B, C, D);
	}
}
Test(MonteCarlo, v6, .disabled = !TEST_TETRAHEDRON) {
	Variant("6") {
		//const Point A(0.2, 0.2, -0.13), B(0.1, -0.83, -0.12), C(-1.4, 0.2, -0.8), D(-0.2, -0.1, 0.7);
		const Point A = {0.2, 0.2, -0.13};
		const Point B = {0.1, -0.83, -0.12};
		const Point C = {-1.4, 0.2, -0.8};
		const Point D = {-0.2, -0.1, 0.7};
		test_tetrahedron(A, B, C, D);
	}
}

xyz_type randomize(void) {
	return Random::Uniform::real<xyz_type>(1, 3);
}

Test(MonteCarlo, random, .disabled = !TEST_RANDOM) {
	Variant("Random Tetrahedron") {
		const Point A = {-1 * randomize(), -1 * randomize(), -1 * randomize()};
		const Point B = {-1 * randomize(),  1 * randomize(), -1 * randomize()};
		const Point C = { 1 * randomize(), -1 * randomize(), -1 * randomize()};
		const Point D = { 1 * randomize(),  1 * randomize(),  1 * randomize()};
		cout << "A " << A() << endl
			 << "B " << B() << endl
			 << "C " << C() << endl
			 << "D " << D() << endl;
		test_tetrahedron(A, B, C, D);
	}
}

//! \note Test Polyhedron calibrate
Test(Polyhedron, Calibrate, .disabled = !TEST_CALIBRATION) {
	const Point A = {-1, -1, -1};
	const Point B = {-1,  1, -1};
	const Point C = { 1, -1, -1};
	const Point E = { 1,  1,  1};
	const Point F = {-1,  1,  1};
	const Point G = { 1, -1,  1};
	Polyhedron polyhedron = MonteCarlo::Hexahedron(A, B, C, E, F, G);
	const Point point = {0, 0, 0};
	// polyhedron.calibrate(point);
	polyhedron.calibrate();
	cr_expect(polyhedron.has(point));
	const auto vertices = polyhedron.vertices();
	for (const auto& vertex : vertices)
		cr_expect(polyhedron.has(vertex));
	cr_expect(polyhedron.has({0.5, 0.5, 0.5}));
	cr_expect(polyhedron.has({-0.5, -0.5, -0.5}));
	cr_expect(!polyhedron.has({-1, -1, 10}));
}

//! \note Test Polyhedron vertices
Test(Polyhedron, Vertices, .disabled = !TEST_VERTICES) {
	const Point A = {-1, -1, -1};
	const Point B = { 1, -1, -1};
	const Point C = { 1,  1, -1};
	const Point D = {-1,  1, -1};
	const Point S = { 0,  0,  3};
	const Point E = {A.x, A.y, 1};
	const Point F = {B.x, B.y, 1};
	const Point G = {C.x, C.y, 1};
	
	const Plane ABC = {A, B, C};
	const Plane ABS = {A, B, S};
	const Plane BCS = {B, C, S};
	const Plane CDS = {C, D, S};
	const Plane DAS = {D, A, S};
	const Plane EFG = {E, F, G};

	Polyhedron polyhedron = {ABC, ABS, BCS, CDS, DAS, EFG};
	const auto vertices = polyhedron.vertices();
	cout << "Vertices: " << vertices.size() << endl;
	for (const auto& vertex : vertices)
		cout << vertex() << endl;
	polyhedron.calibrate();

	cr_expect(polyhedron.has(A));
	cr_expect(polyhedron.has(B));
	cr_expect(polyhedron.has(C));
	cr_expect(polyhedron.has(D));
	cr_expect(!polyhedron.has(E));
	cr_expect(!polyhedron.has(F));
	cr_expect(!polyhedron.has(G));
	cr_expect(!polyhedron.has(S));
	cr_expect(polyhedron.has(ORIGIN));

	const auto canonical = Canonical::Hexahedron(polyhedron.vertices());
	cout << "Canonical: " << canonical << endl;
	const auto monte_carlo = MonteCarlo::method(polyhedron);
	cout << "Volume: " << monte_carlo << endl;
	const auto error = (canonical - monte_carlo) / monte_carlo * 100;
	cout << "Error: " << error << "%" << endl;
}

//! \note Test Polyhedron vertices
Test(Polyhedron, Pentahedron) {
	const Point A = {-1, -1, -1};
	const Point B = { 1, -1, -1};
	const Point C = { 1,  1, -1};
	const Point D = {-1,  1, -1};
	const Point S = { 0,  0,  3};	
	const Polyhedron pentahedron = MonteCarlo::Pentahedron(A, B, C, D, S);

	const auto vertices = pentahedron.vertices();
	cout << "Vertices: " << vertices.size() << endl;
	for (const auto& vertex : vertices)
		cout << vertex() << endl;

	cr_expect(pentahedron.has(A));
	cr_expect(pentahedron.has(B));
	cr_expect(pentahedron.has(C));
	cr_expect(pentahedron.has(D));
	cr_expect(pentahedron.has(S));
	cr_expect(pentahedron.has(ORIGIN));

	const auto canonical = Canonical::Pentahedron(pentahedron.vertices());
	cout << "Canonical: " << canonical << endl;
	const auto monte_carlo = MonteCarlo::method(pentahedron);
	cout << "Volume: " << monte_carlo << endl;
	const auto error = (canonical - monte_carlo) / monte_carlo * 100;
	cout << "Error: " << error << "%" << endl;
}
