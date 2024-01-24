import random

def GetRandomColor():
    """
    This function return random color
    """
    return (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))

def DrawByCoordinates(image, coordinates, color):
    """
    This function draw object by coordinates
    """
    for i in coordinates:
        image[i[0], i[1]] = color
    
    return image