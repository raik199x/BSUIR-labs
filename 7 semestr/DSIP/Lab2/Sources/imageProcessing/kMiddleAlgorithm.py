import cv2
import random

from Sources.imageProcessing.drawing import DrawByCoordinates, GetRandomColor


def GetClusters(Centroids, Properties, AmountOfClusters):
    # Prepare clusters
    Clusters = []
    for _ in range(AmountOfClusters):
        Clusters.append([])

    # Fill clusters
    for Elongation in Properties:
        Differences = []
        for Centroid in Centroids:
            Differences.append(abs(Elongation - Centroid))

        # Find index of minimum value
        MinValue = min(Differences)
        MinIndex = Differences.index(MinValue)

        # Add to cluster
        Clusters[MinIndex].append(Elongation)

    return Clusters


def GetNewCentroids(Clusters):
    # Prepare new centroids
    NewCentroids = []

    # Fill new centroids
    for Cluster in Clusters:
        Sum = 0
        for Elongation in Cluster:
            Sum += Elongation
        if len(Cluster) == 0:
            NewCentroids.append(0)
        else:
            NewCentroids.append(Sum / len(Cluster))

    return NewCentroids


def GetElongationIndex(DesiredProperty, AllProperties):
    for i in range(len(AllProperties)):
        if DesiredProperty == AllProperties[i]:
            return i
    return -1


def k_middle_algorithm(colorfulImage, AmountOfCluster: int, ObjectsPixels: list, Properties: list):
    """
    :param colorfulImage: Image with objects
    :param AmountOfCluster: Amount of clusters to be found
    :param ObjectsPixels: List of objects with their pixels
    :param Elongations: List of elongations of objects (contains int values)
    """
    # Prepare centroids
    Centroids = []
    for _ in range(AmountOfCluster):
        Centroids.append(random.choice(Properties))

    # Prepare clusters
    Clusters = GetClusters(Centroids, Properties, AmountOfCluster)

    # Prepare new centroids
    NewCentroids = GetNewCentroids(Clusters)

    # While centroids are changing
    occurrence = 1
    while Centroids != NewCentroids:
        # Change centroids
        Centroids = NewCentroids

        # Prepare clusters
        Clusters = GetClusters(Centroids, Properties, AmountOfCluster)

        # Prepare new centroids
        NewCentroids = GetNewCentroids(Clusters)
        print("Iteration: ", occurrence)
        occurrence += 1

    # Draw clusters
    CopyProperties = Properties.copy()
    for _ in range(len(Clusters)):
        clusterColor = GetRandomColor()
        for Property in Clusters[_]:
            index = GetElongationIndex(Property, CopyProperties)
            DrawByCoordinates(colorfulImage, ObjectsPixels[index], clusterColor)
            CopyProperties[index] = -1

    cv2.imwrite("clusters.png", colorfulImage)
    print("Clusters are drawn.")
    return Clusters
