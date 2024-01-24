import cv2
import numpy as np


def Grayscale(image):
    res = []

    for line in image:
        res_line = []
        for r, g, b in line:
            color = r * 0.3 + g * 0.59 + b * 0.11
            res_line.append(color)
        res.append(res_line)

    return np.array(res, dtype=np.uint8)


def ImageBinarization(grayscaleImage, threshold):
    # Image size
    height, width = grayscaleImage.shape

    # Binarize the image
    binarized_image = [[0] * width for _ in range(height)]
    for i in range(height):
        for j in range(width):
            if grayscaleImage[i, j] > threshold:
                binarized_image[i][j] = 255  # Set to white
            else:
                binarized_image[i][j] = 0    # Set to black

    # Convert the list to a NumPy array
    binarized_image = np.array(binarized_image, dtype=np.uint8)

    cv2.imwrite("binarized_image.png", binarized_image)
    return binarized_image


def median_filter(image, size):
    """
    Median filter
    :param image: image to filter
    :param size: filter size
    :return: filtered image
    """
    height, width = image.shape
    filtered_image = [[0] * width for _ in range(height)]

    for i in range(height):
        for j in range(width):
            # Get the filter area
            filter_area = []
            for k in range(i - size // 2, i + size // 2 + 1):
                for l in range(j - size // 2, j + size // 2 + 1):
                    if 0 <= k < height and 0 <= l < width:  # if pixel is not beyond the border
                        filter_area.append(image[k][l])

            # Sort the filter area
            filter_area.sort()

            # Get the median
            filtered_image[i][j] = filter_area[len(filter_area) // 2]

    # Convert the list to a NumPy array
    filtered_image = np.array(filtered_image, dtype=np.uint8)
    cv2.imwrite("median_filtered_image.png", filtered_image)
    return filtered_image
