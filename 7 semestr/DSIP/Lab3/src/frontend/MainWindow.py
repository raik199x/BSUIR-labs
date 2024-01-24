from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap
from PySide6.QtWidgets import (
    QApplication,
    QMainWindow,
    QLabel,
    QPushButton,
    QVBoxLayout,
    QHBoxLayout,
    QWidget,
    QFileDialog,
    QMessageBox,
)
from src.backend.Wavelet import MexicanWavelet
from src.backend.AnalyzeImage import isImage, isSquareImage, isWidthSquare2

from src.assets.globalConfig import assetsFolderName, emptyImageFileName

from src.frontend.DecompositionWidget import Decomposition


class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.WaveletFunction = MexicanWavelet()

        self.mainLayout = QVBoxLayout()
        self.mainLayout.setAlignment(Qt.AlignCenter)

        upperLayout = QHBoxLayout()

        selectedImageAnalyze = QVBoxLayout()
        self.imageFormStatus = QLabel("Image not set")
        self.imageSizeStatus = QLabel("Image not set")
        selectedImageAnalyze.addWidget(self.imageFormStatus)
        selectedImageAnalyze.addWidget(self.imageSizeStatus)
        upperLayout.addLayout(selectedImageAnalyze)

        self.originalImage = QLabel()
        pixmap = QPixmap(assetsFolderName + emptyImageFileName)
        self.originalImage.setPixmap(pixmap)
        upperLayout.addWidget(self.originalImage)

        self.setImageButton = QPushButton("Set image")
        self.setImageButton.clicked.connect(lambda: self.SetImageClicked())
        upperLayout.addWidget(self.setImageButton)
        self.mainLayout.addLayout(upperLayout)

        bottomLayout = QHBoxLayout()
        self.decompositionWidgetAccess = Decomposition(self)
        bottomLayout.addWidget(self.decompositionWidgetAccess)
        self.mainLayout.addLayout(bottomLayout)

        mainWidget = QWidget(self)
        mainWidget.setLayout(self.mainLayout)
        self.setCentralWidget(mainWidget)

        # Variables for program run
        self.readyForAction = False
        self.Decomposed = None

    def SetImageClicked(self):
        chooseFile = QFileDialog()
        fileName, _ = chooseFile.getOpenFileName()

        if not isImage(fileName):
            QMessageBox.warning(self, "File error", "Chosen file is not an image")
            return

        pixmap = QPixmap(fileName)
        pixmap = pixmap.scaled(100, 100)
        self.originalImage.setPixmap(pixmap)

        # updating analyze of image
        self.setImageButton.setText(fileName)
        if not isSquareImage(fileName):
            self.imageFormStatus.setText("Image is not squared")
            self.imageFormStatus.setStyleSheet("color: red;")
            self.readyForAction = False
        else:
            self.imageFormStatus.setText("Image squared")
            self.imageFormStatus.setStyleSheet("color: green;")
            self.readyForAction = True

        if not isWidthSquare2(fileName):
            self.imageSizeStatus.setText("Image size's is not square of 2")
            self.imageSizeStatus.setStyleSheet("color: yellow;")
        else:
            self.imageSizeStatus.setText("Image size's is square of 2")
            self.imageSizeStatus.setStyleSheet("color: green;")


def ShowWindow():
    app = QApplication([])
    window = MainWindow()
    window.show()
    app.exec_()
