#pragma once

#include "global.hpp"
#include "config.hpp"

/*!
 * \brief Matrix_t is a two-dimensional array of elements of type data_type
 */
template<typename data_type>
using Matrix_t = data_type**;

/*!
 * \brief Matrix is a short alias for Matrix_t<auto_template_type>
 */
using Matrix = Matrix_t<auto_template_type>;

/*!
 * \namespace matrix
 * \brief Contains functions for working with matrices
 */
namespace matrix
{
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size>
	Matrix_t<data_type> create(const data_type v = 0)
	{
		Matrix_t<data_type> M = new data_type*[m];
		for (size_t i = 0; i < m; ++i)
		{
			M[i] = new data_type[n];
			for (size_t j = 0; j < n; ++j)
				M[i][j] = v;
		}
		return M;
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size>
	void destroy(Matrix_t<data_type> M)
	{
		for (size_t i = 0; i < m; ++i)
			delete[] M[i];
		return delete[] M;
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size>
	string to_string(const Matrix_t<data_type> M)
	{
		string result = "M [ ";
		for (size_t i = 0; i < m; ++i)
		{
			result += "{ ";
			for (size_t j = 0; j < n; ++j)
				result += std::to_string(M[i][j]) + ", ";
			result.pop_back();	result.pop_back();
			result += " }, ";
		}
		result.pop_back();	result.pop_back();
		return result + " ]";
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size>
	void fill(Matrix_t<data_type> M, const data_type v)
	{
		for (size_t i = 0; i < m; ++i)
			for (size_t j = 0; j < n; ++j)
				M[i][j] = v;
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size>
	void move(Matrix_t<data_type> M, const Matrix_t<data_type> V)
	{
		for (size_t i = 0; i < m; ++i)
			for (size_t j = 0; j < n; ++j)
				M[i][j] = V[i][j];
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size>
	bool equal(const Matrix_t<data_type> A, const Matrix_t<data_type> B)
	{
		for (size_t i = 0; i < m; ++i)
			for (size_t j = 0; j < n; ++j)
				if (A[i][j] != B[i][j])
					return false;
		return true;
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size>
	void add(Matrix_t<data_type> C, const Matrix_t<data_type> A, const Matrix_t<data_type> B)
	{
		for (size_t i = 0; i < m; ++i)
			for (size_t j = 0; j < n; ++j)
				C[i][j] = A[i][j] + B[i][j];
	}
	template <typename data_type = auto_template_type, const size_t m = auto_matrix_size,
			  const size_t n = auto_matrix_size, const size_t o = auto_matrix_size>
	void mul(Matrix_t<data_type> C, const Matrix_t<data_type> A, const Matrix_t<data_type> B)
	{
		for (size_t i = 0; i < m; ++i)
			for (size_t k = 0; k < o; ++k)
			{
				C[i][k] = 0;
				for (size_t j = 0; j < n; ++j)
					C[i][k] += A[i][j] * B[j][k];
			}
	}
}
