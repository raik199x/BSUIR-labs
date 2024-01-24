from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap
from PySide6.QtWidgets import QApplication, QMainWindow, QLabel, QHBoxLayout, QVBoxLayout, QPushButton, QLineEdit, QComboBox, QWidget, QSpinBox
from Sources.globalVariables import AmountOfClasses, FolderWithClassImages, DataFolder, ThrashedFolder
from Sources.backend.trashImages import TrashImage, DeleteTrash
from Sources.backend.fileOperations import GetFiles
from Sources.backend.NeuronNetwork import NeuralNetwork

from os import path

class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        # Preparing image frames with their percentages
        self.ListOfImages = list()
        self.ListOfPercentages = list()
        self.ListOfImagesPaths = list()
        ClassesImagesLayout = QHBoxLayout()

        for i in range(AmountOfClasses):
            Combo = QVBoxLayout()
            label = QLabel()

            FileName = "Class" + str(i+1) + ".png"
            pixmap = QPixmap(DataFolder + FolderWithClassImages + FileName)
            self.ListOfImagesPaths.append(DataFolder + FolderWithClassImages + FileName)
            pixmap = pixmap.scaled(100, 100, Qt.KeepAspectRatio)

            label.setPixmap(pixmap)
            self.ListOfImages.append(label)

            lineEdit = QLineEdit()
            lineEdit.setReadOnly(True)
            self.ListOfPercentages.append(lineEdit)

            # adding to general layout
            Combo.addWidget(label, alignment=Qt.AlignCenter)
            Combo.addWidget(lineEdit)
            ClassesImagesLayout.addLayout(Combo)

        self.InputImage = QLabel("Set image", alignment=Qt.AlignCenter)
        self.InputImage.setFixedSize(100, 100)

        # Thrashed box
        self.AmountOfImages = QSpinBox()
        self.AmountOfImages.setMinimum(1)
        self.AmountOfImages.setMaximum(5)

        self.TrashPercent = QSpinBox()
        self.TrashPercent.setMinimum(0)
        self.TrashPercent.setMaximum(90)
        self.TrashPercent.setSingleStep(10)

        self.ButtonGenerateThrash = QPushButton("Generate trash")
        self.ButtonGenerateThrash.clicked.connect(
            lambda: TrashImage(DataFolder+FolderWithClassImages, self.AmountOfImages.value(), self.TrashPercent.value()))

        self.ButtonDeleteTrash = QPushButton("Delete thrash")
        self.ButtonDeleteTrash.clicked.connect(DeleteTrash)

        ThrashedTop = QHBoxLayout()
        ThrashedTop.addWidget(self.ButtonGenerateThrash)
        ThrashedTop.addWidget(self.ButtonDeleteTrash)

        ThrashedBottom = QHBoxLayout()
        ThrashedBottom.addWidget(QLabel("Thrash images (for each)"))
        ThrashedBottom.addWidget(self.AmountOfImages)
        ThrashedBottom.addWidget(QLabel("Chance of thrashing"))
        ThrashedBottom.addWidget(self.TrashPercent)
        ThrashedBottom.addWidget(QLabel("%"))

        ThrashedMenu = QVBoxLayout()
        ThrashedMenu.addLayout(ThrashedTop)
        ThrashedMenu.addLayout(ThrashedBottom)

        self.UpdateImageFolder = QPushButton("Update folder")
        self.UpdateImageFolder.clicked.connect(self.UpdateComboBox)
        self.CurrentInputImage = QComboBox()
        self.CurrentInputImage.currentIndexChanged.connect(
            self.on_ComboBox_changed)

        # Layouts
        MainLayout = QVBoxLayout()
        ComboBox_with_SetFolderImage = QVBoxLayout()
        ComboBox_with_SetFolderImage.addWidget(
            self.CurrentInputImage)
        ComboBox_with_SetFolderImage.addWidget(self.UpdateImageFolder)

        upperLayout = QHBoxLayout()
        upperLayout.addLayout(ThrashedMenu)
        upperLayout.addWidget(self.InputImage)
        upperLayout.addLayout(ComboBox_with_SetFolderImage)

        MainLayout.addLayout(upperLayout)
        MainLayout.addLayout(ClassesImagesLayout)

        MainWidget = QWidget(self)
        MainWidget.setLayout(MainLayout)
        self.setCentralWidget(MainWidget)

        # Creating Neural Network
        self.NW = NeuralNetwork()
        self.NW.LearnNeuralNetwork(self.ListOfImagesPaths)

    def UpdateComboBox(self):
        self.CurrentInputImage.clear()
        Files = GetFiles(DataFolder + ThrashedFolder)
        if Files == None:
            return

        self.CurrentInputImage.addItems(Files)

    def on_ComboBox_changed(self):
        FileName = self.CurrentInputImage.currentText()
        if len(FileName) == 0:
            self.InputImage.clear()
            self.InputImage.setText("Set image")
            return
        # Check if file still exists:
        if not path.exists(DataFolder + ThrashedFolder + FileName):
            self.UpdateComboBox()
            return
        pixmap = QPixmap(DataFolder + ThrashedFolder + FileName)
        pixmap = pixmap.scaled(100, 100, Qt.KeepAspectRatio)
        self.InputImage.setPixmap(pixmap)

        # Also sending info to a neural network
        InputNeurons = self.NW.GetInputNeurons(DataFolder + ThrashedFolder + FileName)
        OutputLayer = self.NW.GetNeuralResult(InputNeurons)
        for num,i in enumerate(OutputLayer):
            self.ListOfPercentages[num].setText(str(OutputLayer[num] * 100))


def ShowWindow():
    app = QApplication([])
    window = MainWindow()
    window.setMinimumSize(850,250)
    window.show()
    app.exec_()
