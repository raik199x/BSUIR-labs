#include "../include/global.hpp"
#include "../include/thresholdTransform.h"
#include <cstring>

__global__ void thresholdPerform(short *d_image, short *result, size_t width, size_t height, short B, short G, short R, size_t threshold) {
	unsigned long index = threadIdx.x + blockIdx.x * blockDim.x;

	if (index > width * height)
		return; // if index is out of range

	size_t resB = d_image[index * 3] - B;
	size_t resG = d_image[index * 3 + 1] - G;
	size_t resR = d_image[index * 3 + 2] - R;

	resB *= resB;
	resG *= resG;
	resR *= resR;

	size_t pixel = resB + resG + resR;
	// pixel        = sqrtf(pixel);

	// if (pixel > threshold) {
	if (pixel > (threshold * threshold)) {
		result[index * 3]     = 0;
		result[index * 3 + 1] = 0;
		result[index * 3 + 2] = 0;
	} else {
		result[index * 3]     = 255;
		result[index * 3 + 1] = 255;
		result[index * 3 + 2] = 255;
	}
}

short *performTransform(short *image, size_t width, size_t height, short B, short G, short R, size_t threshold, unsigned amountOfThreads) {
	// Allocating memory on the device
	short *d_image;
	cudaMallocHost((void **)&d_image, width * height * 3 * sizeof(short));
	//assert_v(cudaMalloc((void **)&d_image, width * height * 3 * sizeof(short)) == cudaSuccess, "cudaMalloc failed (pt1)!");

	// Copying data from host to device
	memcpy(d_image, image, width * height * 3 * sizeof(short));
	//assert_v(cudaMemcpy(d_image, image, width * height * 3 * sizeof(short), cudaMemcpyHostToDevice) == cudaSuccess, "cudaMalloc failed (pt2)!");

	// Creating result buffer for GPU
	short *resultGPU;
	cudaMallocHost((void **)&resultGPU, width * height * 3 * sizeof(short));
	//assert_v(cudaMalloc((void **)&resultGPU, width * height * 3 * sizeof(short)) == cudaSuccess, "cudaMalloc failed (pt3)!");

	// Calculating grid and block size
	dim3 gridSize(width * height / amountOfThreads + 1);
	dim3 blockSize(amountOfThreads);

	// Performing transform
	thresholdPerform<<<gridSize, blockSize>>>(d_image, resultGPU, width, height, B, G, R, threshold);

	// Check for cuda errors
	assert_v((std::string)cudaGetErrorString(cudaDeviceSynchronize()) == "no error", "Error: " + (std::string)cudaGetErrorString(cudaGetLastError()));

	// Copying data from device to host
	//assert_v(cudaMemcpy(image, resultGPU, width * height * 3 * sizeof(short), cudaMemcpyDeviceToHost) == cudaSuccess, "cudaMemcpy failed!");
	memcpy(image, resultGPU, width * height * 3 * sizeof(short));

	// Freeing memory
	//if (!expect_v(cudaFree(d_image) == cudaSuccess, "cudaFree failed (A)!") || !expect_v(cudaFree(resultGPU) == cudaSuccess, "cudaFree failed (B)!"))
	//	assert_v(false, "cudaFree failed!");
	cudaFreeHost(d_image);
	cudaFreeHost(resultGPU);

	return image;
}
