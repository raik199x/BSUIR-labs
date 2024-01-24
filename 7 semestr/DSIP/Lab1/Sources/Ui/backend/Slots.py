from PySide6.QtCore import Slot
from PySide6.QtWidgets import QFileDialog, QMessageBox
from PySide6.QtGui import QPixmap

import matplotlib.pyplot as plt

from os import path

import cv2

import numpy as np

from Sources.Dip.histogram import CalculateHistogram
from Sources.Dip.filters import LowFrequencyFilter, HighFrequencyFilter
from Sources.Dip.preparation import Graphic


@Slot()
def SetImage(self):
    """
    @brief  This function is used to set image to the label
            It opens file dialog and gets path to the image
    """
    self.LeftImagePath, _ = QFileDialog.getOpenFileName(
        self, "Open Image", "", "Image Files (*.png *.jpeg *.jpg)")
    self.InputImage.setPixmap(QPixmap(self.LeftImagePath).scaled(
        self.ImageWidth, self.ImageHeight))
    self.InputImage.adjustSize()
    print(self.LeftImagePath)


@Slot(str)
def MakeHistogram(self, ImagePath):
    """
    @brief  This function is used to make histogram of the image
    """
    if ImagePath == "None":
        QMessageBox.critical(self, "Error", "Please set image first")
        return
    if not path.exists(ImagePath):  # Image does not exist
        QMessageBox.critical(self, "Error", "Image does not exist")
        return

    r_histogram, g_histogram, b_histogram = CalculateHistogram(ImagePath)
    if r_histogram is None:
        if g_histogram == "No file":
            return
        if g_histogram == "Not image":
            return

    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(15, 5))

    # Plot the histograms for R, G, and B channels
    ax1.plot(range(256), r_histogram, color='red', label='Red Channel')
    ax1.set_title('Red Channel Histogram')
    ax1.set_xlabel('Pixel Value')
    ax1.set_ylabel('Frequency')
    ax1.legend()

    ax2.plot(range(256), g_histogram, color='green', label='Green Channel')
    ax2.set_title('Green Channel Histogram')
    ax2.set_xlabel('Pixel Value')
    ax2.set_ylabel('Frequency')
    ax2.legend()

    ax3.plot(range(256), b_histogram, color='blue', label='Blue Channel')
    ax3.set_title('Blue Channel Histogram')
    ax3.set_xlabel('Pixel Value')
    ax3.set_ylabel('Frequency')
    ax3.legend()

    plt.tight_layout()
    plt.show()


@Slot()
def ApplyButton(self):
    """
    @brief  This function is used to apply image processing
    """
    if self.LeftImagePath == "None":
        QMessageBox.critical(self, "Error", "Please set image first")
        return
    if not path.exists(self.LeftImagePath):
        QMessageBox.critical(self, "Error", "No such image")
        return

    image = cv2.imread(self.LeftImagePath)
    if image is None:
        QMessageBox.critical(self, "Error", "Not image")
        return

    if self.EnablePreparation:
        if self.Graphic == "E":
            image = Graphic(image, "E", self.Fmin, self.Fmax, self.Gmin, self.Gmax)
        elif self.Graphic == "D":
            image = Graphic(image, "D", self.Fmin, self.Fmax, self.Gmin, self.Gmax)

    if self.EnableFiltering:
        if self.FilterMin:
            image = LowFrequencyFilter(image, self.FilterMinH)
        if self.FilterMax:
            image = HighFrequencyFilter(image, self.FilterMaxH)

    # saving image
    pathToSave = "data/Applied.jpg"
    cv2.imwrite(pathToSave, image)

    # setting image to the label
    self.RightImagePath = pathToSave
    self.ResultImage.setPixmap(QPixmap(pathToSave).scaled(400, 400))
    self.ResultImage.adjustSize()
    return
