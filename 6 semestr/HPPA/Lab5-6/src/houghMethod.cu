#include "../include/global.hpp"
#include "../include/houghMethod.h"

__global__ void HoughPerform(short *d_image, size_t *result, size_t width, size_t height, double ratio) {
	size_t index = threadIdx.x + blockIdx.x * blockDim.x;
	if (index > width * height)
		return; // if index is out of range

	// skipping dark pixels
	if (d_image[index * 3] == 0 && d_image[index * 3 + 1] == 0 && d_image[index * 3 + 2] == 0)
		return;

	int x = 0, y = 0;
	// We have a problem, that we need to check whole line, but we don't know how long it is (since it's one dimensional array)
	// So we need to decided what is the actual row now (like is is was a 2D array) using a proportion
	// Current index  = x
	// Last index (width * height) = height
	// after that we round up the value to get current row
	size_t current_x_limit = (size_t)ceil((double)(index * height) / (width * height));
	// trying to calculate size on x axis
	for (size_t i = index; i < current_x_limit * width; i++)
		if (d_image[i * 3] == 255 && d_image[i * 3 + 1] == 255 && d_image[i * 3 + 2] == 255)
			x++;
		else
			break;

	if (x == 0)
		return;

	// trying to calculate size on y axis
	for (size_t i = index; i < width * height; i += width)
		if (d_image[i * 3] == 255 && d_image[i * 3 + 1] == 255 && d_image[i * 3 + 2] == 255)
			y++;
		else
			break;

	if (y == 0)
		return;

	double ratioGPU = (double)x / y;
	if (ratioGPU < ratio - 0.1 || ratioGPU > ratio + 0.1)
		return;

	x--;
	y--; //! For real dunno why it gets 1 pixel more
	// Now we need to check if there is 2 more lines that create rectangle
	// checking x axis (bottom left -> bottom right)
	for (size_t i = index + y * width; i < index + y * width + x; i++)
		if (d_image[i * 3] != 255 && d_image[i * 3 + 1] != 255 && d_image[i * 3 + 2] != 255)
			return;

	// checking y axis (top right -> bottom right)
	for (size_t i = index + x; i < index + x + y * width; i += width)
		if (d_image[i * 3] != 255 && d_image[i * 3 + 1] != 255 && d_image[i * 3 + 2] != 255)
			return;

	// if we are here, it means that we found 4 lines, so we can return
	if (result[0] != 0)
		return;
	result[0] = index;
	result[1] = x;
	result[2] = y;
}

size_t *performHough(short *image, size_t width, size_t height, unsigned amountOfThreads) {
	/*
	Due to the fact, that condition of the lab is very brief,
	we assume that we want to find exactly 4 lines, which are
	parallel to the x and y axis (and as a result the figure is a rectangle).
	*/

	// Allocating cuda memory
	short *d_image;
	assert_v(cudaMalloc(&d_image, width * height * 3 * sizeof(short)) == cudaSuccess, "cudaMalloc failed!");

	// Copying data from host to device
	assert_v(cudaMemcpy(d_image, image, width * height * 3 * sizeof(short), cudaMemcpyHostToDevice) == cudaSuccess, "cudaMemcpy failed (host to device)!");

	// Creating result buffer for GPU
	size_t *resultGPU;
	assert_v(cudaMalloc(&resultGPU, 3 * sizeof(size_t)) == cudaSuccess, "cudaMalloc failed!");
	assert_v(cudaMemset(resultGPU, 0, 3 * sizeof(size_t)) == cudaSuccess, "cudaMemset failed (zeroes before kernel)!");

	// Calculating grid and block size
	dim3 gridSize(width * height / amountOfThreads + 1);
	dim3 blockSize(amountOfThreads);

	// Performing Haaf transform
	double ratio = 2.0 / 3.0; // as given in the lab (0.666667)
	HoughPerform<<<gridSize, blockSize>>>(d_image, resultGPU, width, height, ratio);
	assert_v((std::string)cudaGetErrorString(cudaDeviceSynchronize()) == "no error", "Haaf transform failed!");

	// Copying data from device to host
	size_t *result = new size_t[3];
	assert_v(cudaMemcpy(result, resultGPU, 3 * sizeof(size_t), cudaMemcpyDeviceToHost) == cudaSuccess, "cudaMemcpy failed (device to host)!");

	// Freeing memory
	if (!expect_v(cudaFree(d_image) == cudaSuccess, "cudaFree failed (A)!") || !expect_v(cudaFree(resultGPU) == cudaSuccess, "cudaFree failed (B)!"))
		assert_v(false, "cudaFree failed!");

	if (result[0] == 0)
		return NULL; // if we didn't find anything, return NULL

	return result;
}
