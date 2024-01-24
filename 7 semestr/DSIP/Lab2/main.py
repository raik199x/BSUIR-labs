import sys
import cv2

from Sources.imageProcessing.processing import Grayscale, ImageBinarization, median_filter
from Sources.imageProcessing.findingObjects import getCoordinatesOfObjects
from Sources.imageProcessing.drawing import DrawByCoordinates, GetRandomColor
from Sources.imageProcessing.kMiddleAlgorithm import k_middle_algorithm
from Sources.imageProcessing.geometricFeatures import GetAreaSize, GetPerimeter, GetCompactness, GetElongation
"""
1*. if needed, make brightness correction
2. Make from colorful image to binary image
3. threshold image binarization
4. Select four-connected areas in the image
5. Determine the properties of objects, calculate a system of features for the object products presented in the image (area, perimeter, compactness,
elongation, statistical moments).
6. Using the k-means algorithm and the similarity measure specified by the teacher author, determine whether an object belongs
to one of the clusters (classes).
"""

if __name__ == "__main__":
    possibleProperty = ["perimeter", "area", "compactness", "elongation"]
    if len(sys.argv) < 6:
        print("Error: Not enough arguments")
        print("Usage: python main.py <path_to_image> <threshold> <filter_size> <AmountOfClusters> <Property>")
        print("Possible property: 1) perimeter 2) area 3) compactness 4) elongation ")
        sys.exit(1)

    pathToImage = sys.argv[1]
    threshold = int(sys.argv[2])
    filter_size = int(sys.argv[3])
    AmountOfCluster = int(sys.argv[4])
    propertyType = sys.argv[5]

    if propertyType not in possibleProperty:
        print("Unknown property")
        sys.exit(1)

    image = cv2.imread(pathToImage, cv2.IMREAD_COLOR)
    bin_image = ImageBinarization(Grayscale(image), threshold)
    print("Image binarization is done.")

    filtered_image = median_filter(bin_image, filter_size)
    print("Median filter is done.")

    # Getting objects with all their pixels
    ObjectsPixels = getCoordinatesOfObjects(filtered_image)

    # draw objects
    for i in ObjectsPixels:
        DrawByCoordinates(image, i, GetRandomColor())
    cv2.imwrite("objects.png", image)
    print("Objects are drawn.")

    print("Objects characteristics:")
    Property = []
    for num, objectPixels in enumerate(ObjectsPixels):
        print("------------------------")
        Area = int(GetAreaSize(objectPixels))
        Perimeter = int(GetPerimeter(objectPixels))
        Compactness = int(GetCompactness(Area, Perimeter))
        Elongation = int(GetElongation(objectPixels))

        print("Object number: ", num + 1)
        print("Area: ", str(Area))
        print("Perimeter: ", str(Perimeter))
        print("Compactness: ", str(Compactness))
        print("Elongation: ", str(Elongation))

        if propertyType == "elongation":
            Property.append(Elongation)
        elif propertyType == "area":
            Property.append(Area)
        elif propertyType == "perimeter":
            Property.append(Perimeter)
        elif propertyType == "compactness":
            Property.append(Compactness)
        else:
            print("Error: property not found")
            sys.exit(1)
    print("------------------------")

    k_middle_algorithm(cv2.imread(
        pathToImage, cv2.IMREAD_COLOR), AmountOfCluster, ObjectsPixels, Property)
