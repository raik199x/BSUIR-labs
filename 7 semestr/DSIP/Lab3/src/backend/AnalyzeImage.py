import cv2
import math


def isImage(filePath):
    try:
        # Try to read the file using cv2.imread()
        img = cv2.imread(filePath)

        # Check if the image is None (read unsuccessful)
        if img is None:
            return False
        else:
            return True
    except Exception as e:
        # Handle exceptions (e.g., file not found, etc.)
        print(f"Error: {e}")
        return False


def isSquareImage(imagePath):
    try:
        # Try to read the image using cv2.imread()
        img = cv2.imread(imagePath)

        # Check if the image is None (read unsuccessful)
        if img is None:
            return False

        # Get the dimensions of the image
        height, width, _ = img.shape

        # Check if the image is square (width equal to height)
        return width == height

    except Exception as e:
        # Handle exceptions (e.g., file not found, etc.)
        print(f"Error: {e}")
        return False


def isWidthSquare2(imagePath):
    try:
        # Try to read the image using cv2.imread()
        img = cv2.imread(imagePath)

        # Check if the image is None (read unsuccessful)
        if img is None:
            return False

        # Get the width of the image
        width = img.shape[1]

        # Check if the width is a power of 2
        return math.log2(width).is_integer()

    except Exception as e:
        # Handle exceptions (e.g., file not found, etc.)
        print(f"Error: {e}")
        return False
