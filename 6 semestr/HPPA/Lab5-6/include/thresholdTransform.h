#pragma once
#include <iostream>

short *performTransform(short *image, size_t width, size_t height, short B, short G, short R, size_t threshold, unsigned amountOfThreads);
