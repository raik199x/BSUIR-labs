#include "../include/gpu.hpp"
#include <array>

#include <cstddef>
#include <cuda_runtime.h>
#include <curand_kernel.h>

__global__ void kernel(xyz_type prng_limit, size_t number_of_iterations, GPU::Plane *const gpuPlanes, unsigned long long *const device_result) {
	size_t currentIndex = threadIdx.x + blockIdx.x * blockDim.x;
	
	if (currentIndex > number_of_iterations)
		return;
	//__shared__ size_t hit[1024];
	// hit[threadIdx.x] = 0;
	__syncthreads();

	curandState state;
	curand_init(clock64(), currentIndex, 0, &state);

	struct coordinates {
		xyz_type x;
		xyz_type y;
		xyz_type z;
	};
	struct coordinates rCoord;

	// the problem of curand is that it generates values from [0,1), so we need to manipulate values
	rCoord.x = prng_limit * curand_uniform(&state);
	rCoord.y = prng_limit * curand_uniform(&state);
	rCoord.z = prng_limit * curand_uniform(&state);

	float temp[3];
	temp[0] = curand_uniform(&state);
	temp[1] = curand_uniform(&state);
	temp[2] = curand_uniform(&state);

	if(temp[0] < 0.4)
		rCoord.x *= -1;
	if(temp[1] < 0.4)
		rCoord.y *= -1;
	if(temp[2] < 0.4)
		rCoord.z *= -1;

	// checking if point is in a polyhedron
	bool isInside = true;
	for (size_t i = 0; i < 6; ++i) {
		const xyz_type value = gpuPlanes[i].a * rCoord.x + gpuPlanes[i].b * rCoord.y + gpuPlanes[i].c * rCoord.z + gpuPlanes[i].d;
		if (value < 0) {
			isInside = false;
			break;
		}
	}
	if (isInside)
		atomicAdd(device_result, 1llu);
	
	// hit[threadIdx.x] = 1;

	//__syncthreads();
	//if (currentIndex == number_of_iterations - 1) {
	//	printf("block(%d, %d) thread(%d, %d): %f / %ld\n",
	//	       blockIdx.x, blockIdx.y, threadIdx.x, threadIdx.y,
	//	       (*device_result), number_of_iterations);
	//	(*device_result) = (*device_result) / number_of_iterations;
	//}
}

std::array<size_t, 2> GPU::calculateDim(size_t AmountOfThreads) {
	std::array<size_t, 2> result;
	if (N_OF_ITERATIONS < AmountOfThreads) {
		result[0] = N_OF_ITERATIONS;
		result[1] = 1;
	} else {
		result[0] = AmountOfThreads;
		result[1] = (N_OF_ITERATIONS + AmountOfThreads - 1) / AmountOfThreads;
	}
	return result;
}

xyz_type GPU::initCudaCalc(const Geometry3D::Plane planes[6], const xyz_type prng_limit, size_t AmountOfThreads) {
	// Allocate memory for the result on the device
	unsigned long long *device_result;
	cudaMalloc((void **)&device_result, sizeof(unsigned long long));
	cudaMemset(device_result, 0, sizeof(float));

	// Since we cannot send structure with methods of host, we need to recreate struct that will contain only data
	Plane gpuPlanes[6];
	for (int i = 0; i < 6; i++)
		gpuPlanes[i] = {planes[i].a, planes[i].b, planes[i].c, planes[i].d};

	// Allocate memory for the struct on the device
	Plane *devPlanes;
	cudaMalloc(&devPlanes, 6 * sizeof(Plane));
	// Copy the data from the host to the device
	cudaMemcpy(devPlanes, gpuPlanes, 6 * sizeof(Plane), cudaMemcpyHostToDevice);

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	std::array<size_t, 2> size = calculateDim(AmountOfThreads);
	dim3 threads(size[0]);
	dim3 amountOfBlocks(size[1]);

	cudaEventRecord(start, nullptr);
	kernel<<<amountOfBlocks, threads>>>(prng_limit, N_OF_ITERATIONS, devPlanes, device_result);
	cudaDeviceSynchronize(); // ensure all CUDA work has completed

	cudaEventRecord(stop, nullptr);
	cudaEventSynchronize(stop);

	float millisecondsGPU = 0;
	cudaEventElapsedTime(&millisecondsGPU, start, stop);
	std::cout << "Kernel execution time: " << millisecondsGPU << " ms" << std::endl;

	// report cuda state (error / no error)
	std::cerr << "Cuda status: " << cudaGetErrorString(cudaGetLastError()) << std::endl;

	// Copy the result from the device to the host
	unsigned long long *host_result = new unsigned long long;
	cudaMemcpy(host_result, device_result, sizeof(unsigned long long), cudaMemcpyDeviceToHost);

	cudaFree(device_result);
	cudaFree(devPlanes);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	return (long double)(*host_result) / N_OF_ITERATIONS;
}

void GPU::outputCudaInfo(void) {
	int deviceCount = 0;
	cudaGetDeviceCount(&deviceCount);
	for (int i = 0; i < deviceCount; ++i) {
		cudaDeviceProp deviceProp;
		cudaGetDeviceProperties(&deviceProp, i);
		cout << "\tDevice " << i << ": " << deviceProp.name << endl;
		cout << "\tTotal global memory: " << deviceProp.totalGlobalMem << endl;
		cout << "\tShared memory per block: " << deviceProp.sharedMemPerBlock << endl;
		cout << "\tMajor revision number: " << deviceProp.major << endl;
		cout << "\tMinor revision number: " << deviceProp.minor << endl;
	}
}
