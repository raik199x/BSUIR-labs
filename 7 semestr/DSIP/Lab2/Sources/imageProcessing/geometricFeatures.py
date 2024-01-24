def GetAreaSize(AllPixels):
    """
    Calculate the area size of a given pixel list
    :param AllPixels: list of pixels
    :return: area size
    """
    return len(AllPixels)


def GetPerimeter(AllPixels):
    Perimeter = 0

    for i in range(len(AllPixels)):
        neighbors = [(AllPixels[i][0] - 1, AllPixels[i][1]), (AllPixels[i][0] + 1, AllPixels[i][1]),
                     (AllPixels[i][0], AllPixels[i][1] - 1), (AllPixels[i][0], AllPixels[i][1] + 1)]

        for ni, nj in neighbors:
            if (ni, nj) not in AllPixels:
                Perimeter += 1

    return Perimeter


def GetCompactness(AreaSize, Perimeter):
    """
    Calculate the compactness of a given area size and perimeter
    :param AreaSize: area size
    :param Perimeter: perimeter
    :return: compactness
    """
    return (Perimeter ** 2) / AreaSize


def GetCenterMoment(AllPixels, i, j):
    # Calculate the centroid of the pixel set
    x_center = sum(i for i, j in AllPixels) / len(AllPixels)
    y_center = sum(j for i, j in AllPixels) / len(AllPixels)

    # Calculate the i,j-th central moment of the pixel set
    res = 0
    for x, y in AllPixels:
        res += pow(x - x_center, i) * pow(y - y_center, j)

    return res


def GetElongation(AllPixels):
    # Calculate the second-order central moments of the pixel set
    m20 = GetCenterMoment(AllPixels, 2, 0)
    m02 = GetCenterMoment(AllPixels, 0, 2)
    m11 = GetCenterMoment(AllPixels, 1, 1)

    # Calculate the elongation of the pixel set
    d = pow(pow(m20 - m02, 2) + 4 * pow(m11, 2), 0.5)

    if (m20 + m02 - d) == 0: # avoiding division by zero
        return 0

    return (m20 + m02 + d) / (m20 + m02 - d)
