import cv2


def GetAllPixels(image):
    """
    @brief  This function is used to get all pixels with their colors
    """
    height, width, channels = image.shape
    red_pixels = []
    green_pixels = []
    blue_pixels = []
    for y in range(height):
        for x in range(width):
            blue, green, red = image[y, x]
            red_pixels.append(red)
            green_pixels.append(green)
            blue_pixels.append(blue)
    return red_pixels, green_pixels, blue_pixels


def CalculateHistogram(ImagePath):
    """
    @brief  This function is used to calculate histogram of the image
    """
    Image = cv2.imread(ImagePath)
    if Image is None:
        return None, "Not image", None

    r_channel, g_channel, b_channel = GetAllPixels(Image)

    # Calculate histogram
    r_histogram = [0] * 256
    g_histogram = [0] * 256
    b_histogram = [0] * 256

    for i in range(len(r_channel)):
        r_histogram[r_channel[i]] += 1
        g_histogram[g_channel[i]] += 1
        b_histogram[b_channel[i]] += 1

    return r_histogram, g_histogram, b_histogram
