from PySide6.QtCore import Qt
from PySide6.QtWidgets import (
    QWidget,
    QVBoxLayout,
    QGridLayout,
    QSpinBox,
    QPushButton,
    QLabel,
    QHBoxLayout,
    QMessageBox,
    QDoubleSpinBox,
    QScrollArea,
    QFrame,
)
from PySide6.QtGui import QImage, QPixmap
import cv2
import numpy as np


class Decomposition(QWidget):
    def __init__(self, parent):
        super().__init__()

        self.parent = parent

        # Create a scroll area
        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)

        # Create a widget to hold the content
        content_widget = QWidget(scrollArea)
        content_layout = QVBoxLayout(content_widget)

        stage_name = QLabel("1. Decomposition")
        content_layout.addWidget(stage_name)

        functional_widget = QHBoxLayout()
        self.depthSpinBox = QSpinBox()
        self.depthSpinBox.setMinimum(1)
        self.depthSpinBox.setMaximum(5)
        functional_widget.addWidget(QLabel("Decomp. level:"))
        functional_widget.addWidget(self.depthSpinBox)

        self.sigmaValueSpinBox = QDoubleSpinBox()
        self.sigmaValueSpinBox.setMinimum(0.5)
        self.sigmaValueSpinBox.setMaximum(2.0)
        self.sigmaValueSpinBox.setSingleStep(0.1)
        functional_widget.addWidget(QLabel("Sigma value"))
        functional_widget.addWidget(self.sigmaValueSpinBox)

        startDecompButton = QPushButton("Start")
        startDecompButton.clicked.connect(self.PerformDecompose)
        functional_widget.addWidget(startDecompButton)
        content_layout.addLayout(functional_widget)

        self.widgetHeap = QVBoxLayout()
        content_layout.addLayout(self.widgetHeap)

        # Set the content widget for the scroll area
        scrollArea.setWidget(content_widget)

        main_layout = QVBoxLayout(self)
        main_layout.addWidget(scrollArea)

        # Set the alignment after creating the layout
        main_layout.setAlignment(Qt.AlignCenter)

    def array_to_pixmap(self, array):
        height, width = array.shape
        bytes_per_line = width
        c_contiguous_array = array.copy(order="C")  # Make a copy in C-contiguous order
        cv2.normalize(c_contiguous_array, c_contiguous_array, 0, 255, cv2.NORM_MINMAX)
        c_contiguous_array = c_contiguous_array.astype(np.uint8)  # Convert to uint8
        qimage = QImage(
            c_contiguous_array.data,
            width,
            height,
            bytes_per_line,
            QImage.Format_Grayscale8,
        )
        pixmap = QPixmap.fromImage(qimage)
        return pixmap

    def CreateCollage(self, waveletDecomposeResult):
        gridLayout = QGridLayout(alignment=Qt.AlignCenter)

        labels = ["BB", "BH", "HB", "HH"]

        for i in range(len(labels)):
            label = QLabel(self)
            pixmap = self.array_to_pixmap(waveletDecomposeResult[:, :, i])
            label.setPixmap(pixmap)
            if i < 2:
                gridLayout.addWidget(label, i // 2, i % 2)
                gridLayout.addWidget(
                    QLabel(labels[i], alignment=Qt.AlignCenter), (i // 2) + 1, i % 2
                )
            else:
                gridLayout.addWidget(label, i // 2 + 1, i % 2)
                gridLayout.addWidget(
                    QLabel(labels[i], alignment=Qt.AlignCenter), (i // 2) + 2, i % 2
                )

        return gridLayout

    def PerformDecompose(self):
        if not self.parent.readyForAction:
            QMessageBox.warning(self, "Not ready", "Something did not set up")
            return

        self.ClearLayout(self.widgetHeap)
        depth = self.depthSpinBox.value()
        imagePath = self.parent.setImageButton.text()
        oldestApproximation = None
        for i in range(depth):
            decomposeResult = None
            if oldestApproximation is None:
                image = cv2.imread(imagePath, cv2.IMREAD_GRAYSCALE)
                decomposeResult = self.parent.WaveletFunction.WaveletDecomposition(
                    image, self.sigmaValueSpinBox.value()
                )
            else:
                decomposeResult = self.parent.WaveletFunction.WaveletDecomposition(
                    oldestApproximation, self.sigmaValueSpinBox.value()
                )
            self.widgetHeap.addWidget(QLabel("Decompose level " + str(i + 1)))
            self.widgetHeap.addLayout(
                self.CreateCollage(decomposeResult)
            )  # Adding result to ui
            oldestApproximation = decomposeResult[
                :, :, 0
            ]  # saving for continuous calculations

            if depth != 1 and i != depth - 1:  # adding delimiter for better view
                # Create a horizontal line
                line = QFrame(self)
                line.setFrameShape(QFrame.HLine)
                line.setFrameShadow(QFrame.Sunken)
                line.setLineWidth(3)
                self.widgetHeap.addWidget(line)
            if i == depth - 1:  # saving result
                self.parent.Decomposed = decomposeResult

    def ClearLayout(self, layout):
        while layout.count():
            item = layout.takeAt(0)
            widget = item.widget()
            if widget:
                widget.setParent(None)
            else:
                sublayout = item.layout()
                if sublayout:
                    self.ClearLayout(sublayout)  # Recursively clear sub-layouts
                else:
                    layout.removeItem(item)
