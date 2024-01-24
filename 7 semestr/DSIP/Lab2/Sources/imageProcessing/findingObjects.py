def SearchingEveryPixel(image, i, j, AmountOfObjects):
    height, width = image.shape
    coordinates = []
    stack = [(i, j)]

    while stack:
        current_i, current_j = stack.pop()
        coordinates.append([current_i, current_j])
        image[current_i, current_j] = AmountOfObjects

        # Check neighbors and add to stack if conditions are met
        neighbors = [(current_i - 1, current_j), (current_i + 1, current_j),
                     (current_i, current_j - 1), (current_i, current_j + 1)]

        for ni, nj in neighbors:
            if 0 <= ni < height and 0 <= nj < width and image[ni, nj] == 255:
                stack.append((ni, nj))

    return coordinates


def getCoordinatesOfObjects(image):
    processingImage = image.copy()

    height, width = processingImage.shape
    Objects = list()
    AmountOfObjects = 1

    for i in range(height):
        for j in range(width):
            if processingImage[i, j] == 255:
                Objects.append(SearchingEveryPixel(
                    processingImage, i, j, AmountOfObjects))
                AmountOfObjects += 1
    return Objects
