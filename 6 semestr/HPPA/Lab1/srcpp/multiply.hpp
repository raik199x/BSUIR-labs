#pragma once

#include <immintrin.h>

#include "table.hpp"

namespace table
{
	template <typename data_type = auto_template_type, const size_t matrix_size = auto_matrix_size, const size_t table_size = auto_table_size>
	auto mul_auto_vector(Table_t<data_type> C, const Table_t<data_type> A, const Table_t<data_type> B)
	{
		//C[i][k] = 0;
		//matrix::fill<data_type, m, n>(C[index_i][index_k], 0);
		for (size_t table_i = 0; table_i < table_size; ++table_i) for (size_t table_j = 0; table_j < table_size; ++table_j)
			for (size_t matrix_i = 0; matrix_i < matrix_size; ++matrix_i) for (size_t matrix_j = 0; matrix_j < matrix_size; ++matrix_j)
				C[table_i][table_j][matrix_i][matrix_j] = 0;

		auto start = std::chrono::high_resolution_clock::now();

		for (size_t table_i = 0; table_i < table_size; ++table_i)
			for (size_t table_k = 0; table_k < table_size; ++table_k)
				for (size_t table_j = 0; table_j < table_size; ++table_j)
				{
					data_type tmp[matrix_size][matrix_size];
					//C[i][k] += A[i][j] * B[j][k];
					//matrix::mul<data_type, m, o>(tmp, A[index_i][index_j], B[index_j][index_k]);
					//matrix::add(C[index_i][index_k], C[index_i][index_k], tmp);
					for (size_t matrix_i = 0; matrix_i < matrix_size; ++matrix_i)
					{
						for (size_t matrix_k = 0; matrix_k < matrix_size; ++matrix_k)
						{
							tmp[matrix_i][matrix_k] = 0;
							for (size_t j = 0; j < matrix_size; ++j)
								tmp[matrix_i][matrix_k] += A[table_i][table_j][matrix_i][j] * B[table_j][table_k][j][matrix_k];
						}
					}
					for (size_t matrix_i = 0; matrix_i < matrix_size; ++matrix_i)
					{
						for (size_t matrix_k = 0; matrix_k < matrix_size; ++matrix_k)
							C[table_i][table_k][matrix_i][matrix_k] += tmp[matrix_i][matrix_k];
					}
				}
		auto stop = std::chrono::high_resolution_clock::now();
		return duration_cast<std::chrono::duration<double>>(stop - start);
	}
	template <typename data_type = auto_template_type, const size_t matrix_size = auto_matrix_size, const size_t table_size = auto_table_size>
	auto mul_manual_vector(Table_t<data_type> C, const Table_t<data_type> A, const Table_t<data_type> B)
	{
		//C[i][k] = 0; <-> matrix::fill<data_type, m, n>(C[index_i][index_k], 0);
		for (size_t table_i = 0; table_i < table_size; ++table_i)
			for (size_t table_j = 0; table_j < table_size; ++table_j)
				for (size_t matrix_i = 0; matrix_i < matrix_size; ++matrix_i)
					for (size_t matrix_j = 0; matrix_j < matrix_size; ++matrix_j)
						C[table_i][table_j][matrix_i][matrix_j] = 0;

		auto start = std::chrono::high_resolution_clock::now();

		for (size_t table_i = 0; table_i < table_size; ++table_i)
			for (size_t table_k = 0; table_k < table_size; ++table_k)
				for (size_t table_j = 0; table_j < table_size; ++table_j)
					for (size_t matrix_i = 0; matrix_i < matrix_size; ++matrix_i)
					{
						data_type arr_tmp[matrix_size];

						__m256 op_common = _mm256_setr_m128(_mm_load_ps(A[table_i][table_j][matrix_i]), _mm_load_ps(A[table_i][table_j][matrix_i]));
						__m256 op2 = _mm256_setr_m128(_mm_setr_ps(B[table_j][table_k][0][0], B[table_j][table_k][1][0], B[table_j][table_k][2][0], B[table_j][table_k][3][0]),
													  _mm_setr_ps(B[table_j][table_k][0][1], B[table_j][table_k][1][1], B[table_j][table_k][2][1], B[table_j][table_k][3][1]));
						__m256 product1 = _mm256_mul_ps(op_common, op2);

						product1 = _mm256_hadd_ps(product1, product1); // product1 = (product1[0] + product1[1], product1[2] + product1[3], product1[4] + product1[5], product1[6] + product1[7])
						product1 = _mm256_hadd_ps(product1, product1); // product1 = (product1[0] + product1[1] + product1[2] + product1[3], product1[4] + product1[5] + product1[6] + product1[7], product1[0] + product1[1] + product1[2] + product1[3], product1[4] + product1[5] + product1[6] + product1[7])
						arr_tmp[0] = _mm256_cvtss_f32(product1);
						arr_tmp[1] = _mm_cvtss_f32(_mm256_extractf128_ps(product1, 1));

						__m256 op4 = _mm256_setr_m128(_mm_setr_ps(B[table_j][table_k][0][2], B[table_j][table_k][1][2], B[table_j][table_k][2][2], B[table_j][table_k][3][2]),
													  _mm_setr_ps(B[table_j][table_k][0][3], B[table_j][table_k][1][3], B[table_j][table_k][2][3], B[table_j][table_k][3][3]));
						__m256 product2 = _mm256_mul_ps(op_common, op4);

						product2 = _mm256_hadd_ps(product2, product2); // product2 = (product2[0] + product2[1], product2[2] + product2[3], product2[4] + product2[5], product2[6] + product2[7])
						product2 = _mm256_hadd_ps(product2, product2); // product2 = (product2[0] + product2[1] + product2[2] + product2[3], product2[4] + product2[5] + product2[6] + product2[7], product2[0] + product2[1] + product2[2] + product2[3], product2[4] + product2[5] + product2[6] + product2[7])
						arr_tmp[2] = _mm256_cvtss_f32(product2);
						arr_tmp[3] = _mm_cvtss_f32(_mm256_extractf128_ps(product2, 1));

						_mm_store_ps(C[table_i][table_k][matrix_i], _mm_add_ps(_mm_load_ps(C[table_i][table_k][matrix_i]), _mm_load_ps(arr_tmp)));
					}

		auto stop = std::chrono::high_resolution_clock::now();
		return duration_cast<std::chrono::duration<double>>(stop - start);
	}
}
