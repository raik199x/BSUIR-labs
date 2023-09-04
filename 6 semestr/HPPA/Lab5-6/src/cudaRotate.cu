#include "../include/global.hpp"
#include "../include/cudaRotate.h"

__global__ void Rotate(short *Source, unsigned srcPitch, short *Destination, size_t width, size_t height, float angle) {
	int tid = threadIdx.x + blockIdx.x * blockDim.x;

	if (tid >= width * height)
		return; // if index is out of range

	int x = tid % width;
	int y = tid / width;

	float cosAngle = cos(angle * M_PI / 180.0f);
	float sinAngle = sin(angle * M_PI / 180.0f);

	int centerX = width / 2;
	int centerY = height / 2;

	int x1 = x - centerX;
	int y1 = y - centerY;

	int x2 = static_cast<int>(round(x1 * cosAngle - y1 * sinAngle));
	int y2 = static_cast<int>(round(x1 * sinAngle + y1 * cosAngle));

	int x3 = x2 + centerX;
	int y3 = y2 + centerY;

	if (x3 >= 0 && x3 < width && y3 >= 0 && y3 < height) {
		int tid2 = y3 * srcPitch + x3 * 3;

		Destination[tid * 3]     = Source[tid2];
		Destination[tid * 3 + 1] = Source[tid2 + 1];
		Destination[tid * 3 + 2] = Source[tid2 + 2];
	}
}

short *performRotate(short *image, size_t width, size_t height, unsigned amountOfThreads) {
	// Checking if width or height is 0
	assert_v(width != 0, "Width is 0");
	assert_v(height != 0, "Height is 0");

	// Allocating cuda memory
	short *d_image;
	size_t srcPitch;
	assert_v(cudaMallocPitch(&d_image, &srcPitch, width * 3 * sizeof(short), height) == cudaSuccess, "cudaMalloc failed");

	// Copying data from host to device
	assert_v(cudaMemcpy2D(d_image, srcPitch, image, width * 3 * sizeof(short), width * 3 * sizeof(short), height, cudaMemcpyHostToDevice) == cudaSuccess, "cudaMemcpy failed (host to device)!");

	// Creating result buffer for GPU
	short *resultGPU;
	assert_v(cudaMalloc(&resultGPU, height * width * 3 * sizeof(short)) == cudaSuccess, "cudaMalloc failed");

	// Calculating grid and block size
	dim3 gridSize(width * height / amountOfThreads + 1);
	dim3 blockSize(amountOfThreads);

	// Perform rotation
	float angle = 360 - 55;
	Rotate<<<gridSize, blockSize>>>(d_image, srcPitch / sizeof(short), resultGPU, width, height, angle);
	assert_v((std::string)cudaGetErrorString(cudaDeviceSynchronize()) == "no error", "Rotation failed!");

	// Copying data from device to host
	short *result = new short[width * 3 * height];
	assert_v(cudaMemcpy(result, resultGPU, width * 3 * height * sizeof(short), cudaMemcpyDeviceToHost) == cudaSuccess, "cudaMemcpy failed (device to host)!");

	// Freeing memory
	if (!expect_v(cudaFree(d_image) == cudaSuccess, "cudaFree failed (A)") || !expect_v(cudaFree(resultGPU) == cudaSuccess, "cudaFree failed (B)"))
		assert_v(false, "cudaFree failed");

	return result;
}
