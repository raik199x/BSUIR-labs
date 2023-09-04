#include <criterion/criterion.h>

#include "rand_gen.hpp"
#include "multiply.hpp"

/*!
 * \brief Test for matrix::create, matrix::fill, matrix::equal, matrix::add, matrix::destroy
 */
Test(Simple, Matrix1, .disabled = false)
{
	using namespace matrix;

	auto M1 = create(0.0f);
	auto M2 = create(2.0f);
	auto M3 = create(4.0f);

	fill(M1, 2.0f);
	cr_expect(equal(M1, M2), "M1 is not filled!\n");
	add(M2, M2, M1);
	cr_expect(equal(M3, M2), "M1 + M2 != M3!\n");

	destroy(M1);
	destroy(M2);
	destroy(M3);
}

/*!
 * \brief Test for matrix::move, matrix::mul
 */
Test(Simple, Matrix2, .disabled = false)
{
	using namespace matrix;
	auto_template_type x = 2.0f;

	auto A = create(1.0f);
	auto B = create(2.0f);
	auto C = create(3.0f);

	auto R = create<auto_template_type>(auto_matrix_size * x * x);

	move(A, B);
	cr_expect(equal(A, B), "A != B!\n");
	mul(C, A, B);
	cr_expect(equal(R, C), "C = A * B != R!\n");

	destroy(C);
	destroy(B);
	destroy(A);
	destroy(R);
}

/*!
 * \brief Test for table::create, table::fill, table::equal, table::add, table::destroy
 */
Test(Simple, Table1, .disabled = false)
{
	using namespace table;

	auto T1 = create(0u);
	auto T2 = create(2u);
	auto T3 = create(4u);

	auto M = matrix::create(2u);

	fill(T1, M);
	cr_expect(equal(T1, T2), "T1 is not filled!\n");
	add(T2, T2, T1);
	cr_expect(equal(T3, T2), "T1 + T2 != T3!\n");

	destroy(T1);
	destroy(T2);
	destroy(T3);
}

/*!
 * \brief Test for table::move, table::mul
 */
Test(Simple, Table2, .disabled = false)
{
	using namespace table;
	auto_template_type x = 2.0f;

	auto A = create(1.0f);
	auto B = create(x);
	auto C = create(3.0f);

	auto R = create<auto_template_type>(auto_table_size * auto_matrix_size * x * x);

	move(A, B);
	cr_expect(equal(A, B), "A != B!\n");
	cout << "mul(): " << mul(C, A, B) << endl;
	cr_expect(equal(R, C), "C = A * B != R!\n");

	destroy(C);
	destroy(B);
	destroy(A);
	destroy(R);
}

/*!
 * \brief Test for table::mul_auto_vector and table::mul_manual_vector
 */
Test(AutoAndManual, TableMultiplication, .disabled = false)
{
	using namespace table;
	cout << "Auto and manual vectorization" << endl;

	auto A = create(1.0f);
	auto C1 = create(0.0f);
	auto C2 = create(0.0f);
	auto C3 = create(0.0f);

	cout << "mul(): " << mul(C1, A, A) << endl
		 << "mul_auto_vector(): " << mul_auto_vector(C2, A, A) << endl
		 << "mul_manual_vector(): " << mul_manual_vector(C3, A, A) << endl;
	cr_expect(equal(C1, C2), "C1 != C2! results of mul() and mul_auto_vector() are not the same");
	cr_expect(equal(C1, C3), "C1 != C3! results of mul() and mul_manual_vector() are not the same");

	destroy(C3);
	destroy(C2);
	destroy(C1);
	destroy(A);
}

/*!
 * \brief Test for int rand_gen
 */
Test(Random, IntGen, .disabled = false)
{
	cout << "Int rand_gen" << endl;
	for (size_t i = 0; i < 10; ++i)
		cout << rand_gen<int, uniform_int_distribution<int>>(10, 15) << ' ';
	cout << endl;
	for (size_t i = 0; i < 10; ++i)
		cout << rand_gen<int, uniform_int_distribution<int>>(20, 25) << ' ';
	cout << endl;
	for (size_t i = 0; i < 10; ++i)
		cout << rand_gen<int, uniform_int_distribution<int>>(1500, 1600) << ' ';
	cout << endl;
}

/*!
 * \brief Test for double rand_gen
 */
Test(Random, DoubleGen, .disabled = false)
{
	cout << "Double rand_gen" << endl;
	for (size_t i = 0; i < 10; ++i)
		cout << rand_gen<double, uniform_real_distribution<double>>(10, 15) << ' ';
	cout << endl;
	for (size_t i = 0; i < 10; ++i)
		cout << rand_gen<double, uniform_real_distribution<double>>(20, 25) << ' ';
	cout << endl;
	for (size_t i = 0; i < 10; ++i)
		cout << rand_gen<double, uniform_real_distribution<double>>(1500, 1600) << ' ';
	cout << endl;
}

/*!
 * \brief Test for double rand_gen in range (0 - 100)
 */
Test(Random, DoubleGen0_100, .disabled = false)
{
	cout << "Double rand_gen (0 - 100)" << endl;
	for (size_t i = 0; i < 100; ++i)
		cout << rand_gen<double, uniform_real_distribution<double>>(0, 100) << ' ';
	cout << endl;
}

/*!
 * \brief Test for AVX multiplication
 */
Test(Multiplication, AVX, .disabled = true)
{
	float A[1][1][1][4] = { { { { 2.0f, 3.0f, 3.0f, 4.0f } } } };
	float B[1][1][4][1] = { { { { 5.0f }, { 5.0f} , { 6.0f} , { 6.0f} } } };
	constexpr static const auto table_i = 0;
	constexpr static const auto table_j = 0;
	constexpr static const auto table_k = 0;
	constexpr static const auto matrix_i = 0;
	constexpr static const auto matrix_k = 0;
	__m128 product = _mm_mul_ps(_mm_load_ps(A[table_i][table_j][matrix_i]),
								_mm_setr_ps(B[table_j][table_k][0][matrix_k], B[table_j][table_k][1][matrix_k],
											B[table_j][table_k][2][matrix_k], B[table_j][table_k][3][matrix_k]));
	for (size_t i = 1; i < 4; ++i)
		cout << product[i] << endl;
	cout << _mm_cvtss_f32(product) << endl;
	cout << product[0] + product[1] + product[2] + product[3] << endl;
	product = _mm_hadd_ps(product, product);
	product = _mm_hadd_ps(product, product);
	cout << product[0] << endl;
	cout << product[1] << endl;
	cout << product[2] << endl;
	cout << product[3] << endl;
}
