#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <climits>
#include <cstring>

#include "config.hpp"
#include "global.hpp"

using byte = char;

constexpr static const uint64_t maximalNumberOfBlocks = cache / element_size;

void init_array(std::unique_ptr<byte[]>& buffer, const uint64_t N)
{
	size_t blockSize = cache / N;
	size_t blocksNumber = blockSize / element_size;
	
	using element_t = uint8_t[element_size];
	using block_t = element_t[maximalNumberOfBlocks];
	memset(buffer.get(), '\0', cache * Nmax);
	
	block_t *cache = reinterpret_cast<block_t*>(buffer.get());
	
	for (size_t bankIndex = 0; bankIndex < N; ++bankIndex) {
		for (size_t blockIndex = 0; blockIndex < blocksNumber; ++blockIndex) {
			if (bankIndex < N - 1) {
				reinterpret_cast<uint64_t&>(cache[bankIndex][blockIndex]) = (blockIndex + ((bankIndex + 1) * maximalNumberOfBlocks)) * element_size;
			}
		}
	}
	reinterpret_cast<uint64_t&>(cache[N - 1][blocksNumber - 1]) = 0;
	
	/*/// \note Clear the array
	for (uint64_t index = 0; index < Nmax * cache; ++index)
		aligned_array[index] = 0;
	using element_t = byte[64];
	auto reinterpret_array = reinterpret_cast<element_t*>(aligned_array);
	/// \note Running through all columns to initialize cells in the same column
	for (uint64_t index_column = 0; index_column < block_size; ++index_column)
		/// \note Running through all rows to initialize cells in all rows of the current column
		for (uint64_t index_row = 0; index_row < N; ++index_row)
			/// \note If the next row exists, then we set the pointer to the next row
			if (index_row + 1 < N)
				reinterpret_cast<uint64_t&>(aligned_array[index_row * block_size + index_column * 64]) = (index_row + 1) * block_size + index_column * 64;
			/// \note If it doesn't, then we set the pointer to the first row of the next column
			else
				/// \note But only if the next column exists
				if (index_column + 1 < block_size)
					/// \note Value: (index_row = 0) * block_size + (index_column + 1) * 64;
					reinterpret_cast<uint64_t&>(aligned_array[index_row * block_size + index_column * 64]) = (index_column + 1) * 64;
				/// \note If it doesn't, then we set the pointer to the first row of the first column
				else
					/// \note Value: (index_row = 0) * block_size + (index_column = 0) * 64;
					reinterpret_cast<uint64_t&>(aligned_array[index_row * block_size + index_column * 64]) = 0;
	*/
	/*
	/// \note Running through all columns to initialize cells in the same column
	for (uint64_t index_column = 0; index_column < block_size / 64; ++index_column)
		/// \note Running through all rows to initialize cells in all rows of the current column
		for (uint64_t index_row = 0; index_row < N; ++index_row)
			/// \note If the next row exists, then we set the pointer to the next row
			if (index_row + 1 < N)
				reinterpret_cast<uint64_t&>(aligned_array[index_row * block_size + index_column * 64]) = (index_row + 1) * block_size + index_column * 64;
			/// \note If it doesn't, then we set the pointer to the first row of the next column
			else
				/// \note But only if the next column exists
				if (index_column + 1 < block_size / 64)
					/// \note Value: (index_row = 0) * block_size + (index_column + 1) * 64;
					reinterpret_cast<uint64_t&>(aligned_array[index_row * block_size + index_column * 64]) = (index_column + 1) * 64;
				/// \note If it doesn't, then we set the pointer to the first row of the first column
				else
					/// \note Value: (index_row = 0) * block_size + (index_column = 0) * 64;
					reinterpret_cast<uint64_t&>(aligned_array[index_row * block_size + index_column * 64]) = 0;
	*/
}

int main(void)
{
	std::unique_ptr<byte[]> buffer(new byte[Nmax * cache]);
	cout << "| N | Time (ms) |       t       |" << endl;
	cout << "|" << string(3,'-') << "|"<< string(11,'-') << "|" << string(15, '-') << "|" << endl;

	uint64_t t = 0;

	for (uint64_t N = 1; N <= Nmax; ++N)
	{
		const uint64_t block_size = cache / N;
		init_array(buffer, N);

		auto start = std::chrono::steady_clock::now();

		for (uint64_t k = 0; k < INT_MAX; ++k)
			t = reinterpret_cast<const uint64_t&>(buffer[t]);

		auto end = std::chrono::steady_clock::now();

		cout << '|' << setw(3) << N << '|' << setw(11) << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << '|' << setw(15) << t << '|' << endl;
	}

	return 0;
}
