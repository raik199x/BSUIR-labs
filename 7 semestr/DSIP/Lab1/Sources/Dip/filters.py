import cv2

import numpy as np

LowFrequencyH1Matrix = [[1, 1, 1], [1, 1, 1], [1, 1, 1]]
LowFrequencyH1Multiply = 1/9

LowFrequencyH2Matrix = [[1, 1, 1], [1, 2, 1], [1, 1, 1]]
LowFrequencyH2Multiply = 1/10

LowFrequencyH3Matrix = [[1, 2, 1], [2, 4, 2], [1, 2, 1]]
LowFrequencyH3Multiply = 1/16

HighFrequencyH1Matrix = [[-1, -1, -1], [-1, 9, -1], [-1, -1, -1]]
HighFrequencyH2Matrix = [[0, -1, 0], [-1, 5, -1], [0, -1, 0]]
HighFrequencyH3Matrix = [[1, -2, 1], [-2, 5, -2], [1, -2, 1]]


def LowFrequencyFilter(image, H):
    """
    Applies a low frequency filter to the input image using the specified filter matrix.

    Args:
        image (cv2 opened image): The input image to be filtered.
        H (int): The filter matrix to be used. Must be 1, 2, or 3.

    Returns:
        cv2 image: The filtered image.
    """
    Matrix = []
    Multiply = 0
    if H == 1:
        Matrix = LowFrequencyH1Matrix
        Multiply = LowFrequencyH1Multiply
    elif H == 2:
        Matrix = LowFrequencyH2Matrix
        Multiply = LowFrequencyH2Multiply
    elif H == 3:
        Matrix = LowFrequencyH3Matrix
        Multiply = LowFrequencyH3Multiply
    else:
        return image

    # Creating image where upper border repeats 2 times
    image = cv2.copyMakeBorder(image, 0, 0, 0, 0, cv2.BORDER_REPLICATE)

    # Get image size
    height, width, channel = image.shape

    # Loop through the image using a 3x3 kernel
    for i in range(1, height - 1):
        for j in range(1, width - 1):
            sum = 0  # Initialize the sum for this pixel
            for k in range(-1, 2):  # Iterate over rows of the kernel
                for l in range(-1, 2):  # Iterate over columns of the kernel
                    # Compute the weighted sum of neighboring pixels and the kernel values
                    # Add the product of the pixel value and the corresponding kernel value to the sum
                    sum = sum + (image[i + k][j + l] * Matrix[k + 1][l + 1])
            # Multiply the sum by the specified scaling factor (Multiply)
            # Update the pixel value with the filtered result
            image[i][j] = sum * Multiply

    # removing the upper border
    image = image[1:height - 1, 1:width - 1]

    return image


def HighFrequencyFilter(image, H):
    """
    Applies a high frequency filter to the input image using the specified filter matrix.

    Args:
        image (cv2 opened image): The input image to be filtered.
        H (int): The filter matrix to be used. Must be 1, 2, or 3.

    Returns:
        cv2 image: The filtered image.
    """
    Matrix = []
    if H == 1:
        Matrix = HighFrequencyH1Matrix
    elif H == 2:
        Matrix = HighFrequencyH2Matrix
    elif H == 3:
        Matrix = HighFrequencyH3Matrix
    else:
        return image

    # Create a copy of the input image to store the filtered result
    filtered_image = cv2.copyMakeBorder(image, 0, 0, 0, 0, cv2.BORDER_REPLICATE)

    # Get image size
    height, width, channel = image.shape

    # Create a kernel from the filter matrix
    kernel = np.array(Matrix)

    # Loop through the image using a 3x3 kernel
    for i in range(1, height - 1):
        for j in range(1, width - 1):
            # Initialize a sum array for each channel
            sum = np.zeros(channel, dtype=np.float32)
            for k in range(-1, 2):  # Iterate over rows of the kernel
                for l in range(-1, 2):  # Iterate over columns of the kernel
                    # Calculate the weighted sum of neighboring pixels and kernel values for each channel
                    sum += image[i + k, j + l] * kernel[k + 1, l + 1]

            # Subtract the weighted sum from the original pixel values for each channel
            filtered_pixel = image[i, j] - sum

            # Clip and convert the filtered pixel values to the valid range of [0, 255] for each channel
            filtered_pixel = np.clip(filtered_pixel, 0, 255).astype(np.uint8)

            # Update the pixel at (i, j) with the filtered result for each channel
            filtered_image[i, j] = filtered_pixel

    # removing the upper border
    filtered_image = filtered_image[1:height - 1, 1:width - 1]

    return filtered_image
