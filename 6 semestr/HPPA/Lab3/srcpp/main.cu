#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>

#include <chrono>
#include <fstream>

using std::cerr;
using std::cout;
using std::string;

size_t maxThreadsPerBlock = 64; //! must be a power of 2 and max 1024

__global__ void calculate(unsigned int *arrayGPU, unsigned int *resultGPU, size_t arraySize) {
	unsigned long index = threadIdx.x + blockIdx.x * blockDim.x;

	if (index > pow(arraySize / 2, 2))
		return; // if index is out of range

	size_t increment = arraySize / 2;
	increment = index / increment * arraySize;

	resultGPU[index * 4] = arrayGPU[index * 2 + increment + arraySize + 1];
	resultGPU[index * 4 + 1] = arrayGPU[index * 2 + increment + arraySize];
	resultGPU[index * 4 + 2] = arrayGPU[index * 2 + increment + 1];
	resultGPU[index * 4 + 3] = arrayGPU[index * 2 + increment];
}

/**
 * @brief Calculates grid and block size for CUDA kernel
 *
 * @param arraySize
 * @return unsigned*
 */
unsigned int *calcGridBlockSize(size_t arraySize) {
	unsigned int *result = new unsigned int[2];

	if (pow(arraySize / 2, 2) > maxThreadsPerBlock) {
		result[0] = pow(arraySize / 2, 2) / maxThreadsPerBlock;
		result[1] = maxThreadsPerBlock;
	} else {
		result[0] = 1;
		result[1] = pow(arraySize / 2, 2);
	}

	return result;
}

int main(int argc, char **argv) {
	size_t arraySize;
	bool nfs = false;
	if (argc == 1) {
		cout << "Lab3 - CUDA" << std::endl
				 << "Usage: ./run <arraySize> <options>" << std::endl
				 << "Options:" << std::endl
				 << "nfs - no file save, program won't write output of functions into files" << std::endl
				 << "threads <number> - set number of threads per block (default 64)" << std::endl
				 << "Example: ./run 1024 nfs threads 64" << std::endl
				 << "-----------------------------------------------------------------------" << std::endl;
		return 0;
	} else { // checkers
		try {
			arraySize = std::stoul(argv[1]);
		} catch (...) {
			cerr << "Failed to convert '" << argv[1] << "' to size_t" << std::endl;
			return 1;
		}

		for (int i = 2; i < argc; i++) {
			if (argv[i] == string("nfs")) {
				nfs = true;
			} else if (argv[i] == string("threads")) {
				try {
					maxThreadsPerBlock = std::stoul(argv[i + 1]);
					if (maxThreadsPerBlock % 2 != 0)
						throw("Threads per block must be a power of 2");
					else if (maxThreadsPerBlock > 1024)
						throw("Threads per block must be less than 1024");
					else if (maxThreadsPerBlock == 0)
						throw("Threads per block must be greater than 0");
				} catch (string error) {
					cout << error << std::endl;
					return 1;
				} catch (...) {
					cerr << "Failed to convert '" << argv[i + 1] << "' to size_t" << std::endl;
					return 1;
				}
			}
		}
	}
	if ((arraySize == 0) || (arraySize % 4 != 0)) {
		cout << "Array size must be a multiple of 4" << std::endl;
		return 1;
	} else
		cout << "Size is set to " << arraySize * arraySize << std::endl;

	const size_t realSize = arraySize * arraySize;

	// Create an array of ${arraySize} integers
	unsigned int *arrayHostBase = new unsigned int[arraySize * arraySize];
	for (size_t i = 0; i < realSize; i++)
		arrayHostBase[i] = i;

	// initializing result for cpu
	unsigned int *resultCPU = new unsigned int[arraySize * arraySize];

	// calculating
	size_t resultIndex = 0;
	auto startCPU = std::chrono::steady_clock::now();

	// arraySize * 2 because we need to skip 2 rows
	for (size_t z = 0; z < arraySize * arraySize; z += arraySize * 2)
		// x+=2 because we need to skip 2 elements
		for (size_t x = 0; x < arraySize; x += 2) {
			resultCPU[resultIndex++] = arrayHostBase[z + x + arraySize + 1];
			resultCPU[resultIndex++] = arrayHostBase[z + x + arraySize];
			resultCPU[resultIndex++] = arrayHostBase[z + x + 1];
			resultCPU[resultIndex++] = arrayHostBase[z + x];
		}

	resultIndex = 0; // just in case
	auto stopCPU = std::chrono::steady_clock::now();
	auto millisecondsCPU = std::chrono::duration_cast<std::chrono::duration<float>>(stopCPU - startCPU).count() * 1000.0;
	cout << "CPU execution time: "
			 << millisecondsCPU
			 << " ms" << std::endl;

	// allocating memory on GPU
	unsigned int *resultGPU, *arrayGPU;
	cudaMalloc((void **)&arrayGPU, realSize * sizeof(unsigned int));
	cudaMalloc((void **)&resultGPU, realSize * sizeof(unsigned int));

	cudaMemcpy(arrayGPU, arrayHostBase, realSize * sizeof(unsigned int), cudaMemcpyHostToDevice); // copying data to GPU

	// calculating on GPU
	// calculating grid and block size
	unsigned int *gridBlockSize = calcGridBlockSize(arraySize);
	dim3 gridSize(gridBlockSize[0]);
	dim3 blockSize(gridBlockSize[1]);

	cout << "Blocks in grid: " << gridSize.x << std::endl
			 << "Threads in blocks: " << blockSize.x << std::endl;
	// Measure the time taken
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start, nullptr);
	calculate<<<gridSize, blockSize>>>(arrayGPU, resultGPU, arraySize);
	cudaEventRecord(stop, nullptr);
	cudaEventSynchronize(stop);

	float millisecondsGPU = 0;
	cudaEventElapsedTime(&millisecondsGPU, start, stop);

	std::cout << "Kernel execution time: " << millisecondsGPU << " ms" << std::endl;
	std::cout << "Ratio: " << millisecondsCPU / millisecondsGPU << std::endl;

	// copying data back to CPU
	unsigned int *bufferGPU = new unsigned int[realSize];
	cudaMemcpy(bufferGPU, resultGPU, realSize * sizeof(unsigned int), cudaMemcpyDeviceToHost);

	// report cuda state (error / no error)
	std::cerr << "Cuda status: " << cudaGetErrorString(cudaGetLastError()) << std::endl;

	// output data into files
	if (!nfs) {
		//! \note This is a much faster way to write data into files. For 16*1024 elements it takes about 39.0~40.9~42.5 seconds to write data into files.
		//! \note Plus it adds newlines after every 4 elements.
		FILE *outputCPU = fopen("build/outputCPU.txt", "w");
		FILE *outputGPU = fopen("build/outputGPU.txt", "w");
		if (outputCPU && outputGPU) {
			auto startWriting = std::chrono::steady_clock::now();
			for (size_t i = 0; i < realSize; i += 4) {
				fprintf(outputCPU, "%u %u %u %u\n", *(resultCPU + i), *(resultCPU + i + 1), *(resultCPU + i + 2), *(resultCPU + i + 3));
				fprintf(outputGPU, "%u %u %u %u\n", *(bufferGPU + i), *(bufferGPU + i + 1), *(bufferGPU + i + 2), *(bufferGPU + i + 3));
			}
			auto stopWriting = std::chrono::steady_clock::now();
			cout << "Writing time: "
					 << std::chrono::duration_cast<std::chrono::duration<float>>(stopWriting - startWriting).count() * 1000.0
					 << " ms" << std::endl;
		} else
			cerr << "Failed to open one of output files!" << std::endl;
		if (outputCPU)
			fclose(outputCPU);
		if (outputGPU)
			fclose(outputGPU);
	}
	// free memory CPU
	delete[] arrayHostBase;
	delete[] resultCPU;
	delete[] bufferGPU;
	delete[] gridBlockSize;
	// free memory GPU
	cudaFree(arrayGPU);
	cudaFree(resultGPU);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	return 0;
}
