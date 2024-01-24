import cv2
import numpy as np

# https://studfile.net/preview/9736890/page:14/
def Graphic(image, graphic, fmin, fmax, gmin, gmax):
    """
    @brief  This function is used to make graphic E
    """
    # Get image size
    height, width = image.shape[:2]
    # Create new image
    new_image = image.copy()
    # Iterate over image
    for i in range(height):
        for j in range(width):
            # Get pixel value
            pixel = image[i, j]
            # Calculate new value
            if(graphic == "E"):
                new_value = (pixel - fmin) * ((gmax - gmin) / (fmax - fmin)) + gmin
            elif(graphic == "D"):
                new_value = (pixel - gmin) * ((fmax - fmin) / (gmax - gmin)) + fmin
            else:
                return None
            # Check if new value is in range
            new_value = np.clip(new_value, 0, 255).astype(np.uint8)
            # Set new value
            new_image[i, j] = new_value
    return new_image