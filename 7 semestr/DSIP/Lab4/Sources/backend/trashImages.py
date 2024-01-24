from os import path, mkdir
from shutil import rmtree
from Sources.globalVariables import DataFolder, ThrashedFolder
from Sources.backend.fileOperations import GetFiles
from uuid import uuid4
import numpy as np
import cv2
import random


def DeleteTrash():
    if path.exists(DataFolder+ThrashedFolder):
        rmtree(DataFolder+ThrashedFolder)


def TrashImage(FolderWithImages: str, AmountForEach: int, ChanceOfThrashing: int):
    # Checking of trashed folder exists
    if not path.exists(DataFolder+ThrashedFolder):
        mkdir(DataFolder+ThrashedFolder)

    # assumes that all files in folder is images
    Images = GetFiles(FolderWithImages)

    for File in Images:
        for _ in range(AmountForEach):
            image = cv2.imread(FolderWithImages + File)
            if image is None:
                continue
            height, width = image.shape[:2]
            # Iterating through image pixels
            for y in range(height):
                for x in range(width):
                    # thrashing
                    if random.random() < ChanceOfThrashing / 100:
                        if np.array_equal(image[y, x], (255, 255, 255)):
                            image[y, x] = (0, 0, 0)
                        else:
                            image[y, x] = (255, 255, 255)
            # Generate a unique filename with extension
            OutputFile = str(uuid4()) + '.jpg'
            cv2.imwrite(DataFolder + ThrashedFolder + OutputFile, image)
