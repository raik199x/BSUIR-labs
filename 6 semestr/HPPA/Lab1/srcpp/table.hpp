#pragma once

#include <chrono>

#include "matrix.hpp"

/*!
 * \brief Table_t is a four-dimensional array of elements of type data_type
 */
template<typename data_type>
using Table_t = data_type****;

/*!
 * \brief Table is a short alias for Table_t<auto_template_type>
 */
using Table = Table_t<auto_template_type>;

/*!
 * \namespace table
 * \brief Contains functions for working with tables
 */
namespace table
{
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size, const size_t p = auto_table_size,
			  const size_t q = auto_table_size>
	Table_t<data_type> create(const data_type v = 0)
	{
		Table_t<data_type> M = new Matrix_t<data_type>*[p];
		for (size_t k = 0; k < p; ++k)
		{
			M[k] = new Matrix_t<data_type>[q];
			for (size_t l = 0; l < q; ++l)
				M[k][l] = matrix::create<data_type, m, n>(v);
		}
		return M;
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size, const size_t p = auto_table_size,
			  const size_t q = auto_table_size>
	void destroy(Table_t<data_type> T)
	{
		for (size_t k = 0; k < p; ++k)
		{
			for (size_t l = 0; l < q; ++l)
				matrix::destroy<data_type, m, n>(T[k][l]);
			delete[] T[k];
		}
		return delete[] T;
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size, const size_t p = auto_table_size,
			  const size_t q = auto_table_size>
	string to_string(const Table_t<data_type> T)
	{
		string result = "T [ ";
		for (size_t k = 0; k < p; ++k)
		{
			result += "{ ";
			for (size_t l = 0; l < q; ++l)
				result += matrix::to_string<data_type, m, n>(T[k][l]) + ", ";
			result.pop_back();	result.pop_back();
			result += " }, ";
		}
		result.pop_back();	result.pop_back();
		return result + " ]";
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size, const size_t p = auto_table_size,
			  const size_t q = auto_table_size>
	void fill(Table_t<data_type> T, const Matrix_t<data_type> v)
	{
		for (size_t k = 0; k < p; ++k)
			for (size_t l = 0; l < q; ++l)
				matrix::move<data_type, m, n>(T[k][l], v);
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size, const size_t p = auto_table_size,
			  const size_t q = auto_table_size>
	void move(Table_t<data_type> T,  const Table_t<data_type> V)
	{
		for (size_t k = 0; k < p; ++k)
			for (size_t l = 0; l < q; ++l)
				matrix::move<data_type, m, n>(T[k][l], V[k][l]);
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size, const size_t p = auto_table_size,
			  const size_t q = auto_table_size>
	bool equal(const Table_t<data_type> A, const Table_t<data_type> B)
	{
		for (size_t k = 0; k < p; ++k)
			for (size_t l = 0; l < q; ++l)
				if (!matrix::equal(A[k][l], B[k][l]))
					return false;
		return true;
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size, const size_t p = auto_table_size,
			  const size_t q = auto_table_size>
	void add(Table_t<data_type> C, const Table_t<data_type> A, const Table_t<data_type> B)
	{
		for (size_t k = 0; k < p; ++k)
			for (size_t l = 0; l < q; ++l)
				matrix::add<data_type, m, n>(C[k][l], A[k][l], B[k][l]);
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size, const size_t o = auto_matrix_size,
			  const size_t p = auto_table_size, const size_t q = auto_table_size,
			  const size_t r = auto_table_size>
	auto mul(Table_t<data_type> C, const Table_t<data_type> A, const Table_t<data_type> B)
	{
		Matrix_t<data_type> tmp = matrix::create<data_type, m, o>();
		auto start = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < p; ++i)
			for (size_t k = 0; k < r; ++k)
			{
				//C[i][k] = 0;
				matrix::fill<data_type, m, n>(C[i][k], 0);
				for (size_t j = 0; j < q; ++j)
				{
					//C[i][k] += A[i][j] * B[j][k];
					matrix::mul<data_type, m, o>(tmp, A[i][j], B[j][k]);
					matrix::add(C[i][k], C[i][k], tmp);
				}
			}
		auto stop = std::chrono::high_resolution_clock::now();
		matrix::destroy<data_type, m, o>(tmp);
		return duration_cast<std::chrono::duration<double>>(stop - start);
	}
}
