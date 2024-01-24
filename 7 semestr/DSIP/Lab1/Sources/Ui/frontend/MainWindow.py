from PySide6.QtCore import Qt
from PySide6.QtWidgets import QApplication, QMainWindow, QLabel, QHBoxLayout, QVBoxLayout, QPushButton, QWidget

from Sources.Ui.frontend.FilterSettingsWindow import FilterSettingsWindow

from Sources.Ui.backend.Slots import SetImage, MakeHistogram, ApplyButton


class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setWindowTitle("Lab1")
        self.resize(1200, 500)

        self.ImageWidth = 400
        self.ImageHeight = 400
        self.LeftImagePath = "None"
        self.RightImagePath = "None"

        # Settings for Image processing
        self.EnablePreparation = False
        self.EnableFiltering = False
        self.Gmin = 0
        self.Gmax = 255
        self.Fmin = 0
        self.Fmax = 255
        self.FilterMin = False
        self.FilterMax = False
        self.FilterMinH = 1
        self.FilterMaxH = 1
        self.Graphic = "E"

        # Main layout
        MainLayout = QHBoxLayout()
        self.setLayout(MainLayout)

        # Left Side of window
        LeftSide = QVBoxLayout()
        LeftSide.setAlignment(Qt.AlignCenter)

        # Create label and set to to the center of the window
        InputLabel = QLabel(self)
        InputLabel.setText("Input image")
        InputLabel.setAlignment(Qt.AlignCenter)
        LeftSide.addWidget(InputLabel)

        #  Creating Label
        self.InputImage = QLabel(self)
        # Scaling label to 400x400
        self.InputImage.setFixedSize(self.ImageWidth, self.ImageHeight)
        self.InputImage.setText("No image")
        # Set "No image" in the center of the label
        self.InputImage.setAlignment(Qt.AlignCenter)
        # adding stroke to the label
        self.InputImage.setStyleSheet("border: 1px solid black;")
        LeftSide.addWidget(self.InputImage)

        # Creating buttons (Set image, Histogram, Filter)
        SetImageButton = QPushButton("Set image")
        SetImageButton.clicked.connect(self.on_setImage_pressed)

        HistogramButton = QPushButton("Histogram")
        HistogramButton.clicked.connect(
            lambda: self.on_histogram_pressed(self.LeftImagePath))

        ImageProcessingSettings = QPushButton("Image Processing Settings")
        ImageProcessingSettings.clicked.connect(
            self.on_imageProcessingSettings_pressed)

        ButtonsLayout = QHBoxLayout()
        ButtonsLayout.addWidget(SetImageButton)
        ButtonsLayout.addWidget(HistogramButton)
        ButtonsLayout.addWidget(ImageProcessingSettings)
        LeftSide.addLayout(ButtonsLayout)

        MainLayout.addLayout(LeftSide)
        # Middle side (Label(arrow) + Button apply)
        MiddleSide = QVBoxLayout()

        # Adding spacer to lower place of elements
        MiddleSide.addStretch(1)

        # Creating Label
        ArrowLabel = QLabel(self)
        ArrowLabel.setText("->")
        ArrowLabel.setAlignment(Qt.AlignCenter)
        MiddleSide.addWidget(ArrowLabel)

        # Creating button
        ApplyButton = QPushButton("Apply")
        ApplyButton.clicked.connect(self.on_apply_pressed)
        MiddleSide.addWidget(ApplyButton)
        # Resize apply button so it looks better
        ApplyButton.setFixedSize(100, 50)

        # Adding Separator
        MiddleSide.addStretch(1)

        MainLayout.addLayout(MiddleSide)
        # Right Side of window
        RightSide = QVBoxLayout()
        RightSide.setAlignment(Qt.AlignCenter)

        ResultLabel = QLabel(self)
        ResultLabel.setText("Result image")
        ResultLabel.setAlignment(Qt.AlignCenter)
        RightSide.addWidget(ResultLabel)

        self.ResultImage = QLabel(self)
        self.ResultImage.setFixedSize(self.ImageWidth, self.ImageHeight)
        self.ResultImage.setText("No image")
        # Set "No image" in the center of the label
        self.ResultImage.setAlignment(Qt.AlignCenter)
        # adding stroke to the label
        self.ResultImage.setStyleSheet("border: 1px solid black;")
        RightSide.addWidget(self.ResultImage)

        # Button (Histogram)
        HistogramButton = QPushButton("Histogram")
        HistogramButton.clicked.connect(
            lambda: self.on_histogram_pressed(self.RightImagePath))
        RightSide.addWidget(HistogramButton)

        MainLayout.addLayout(RightSide)
        # Set the main window layout to the MainLayout object
        central_widget = QWidget()
        central_widget.setLayout(MainLayout)
        self.setCentralWidget(central_widget)
        self.adjustSize()  # Update layout and resize dialog

    def on_setImage_pressed(self):
        SetImage(self)

    def on_histogram_pressed(self, path):
        MakeHistogram(self, path)

    def on_imageProcessingSettings_pressed(self):
        filter_settings = FilterSettingsWindow(self.Gmin, self.Gmax,
                                               self.Fmin, self.Fmax,
                                               self.FilterMin, self.FilterMax,
                                               self.EnablePreparation, self.EnableFiltering,
                                               self.FilterMinH, self.FilterMaxH, self.Graphic,
                                               parent=self)
        filter_settings.exec_()

    def set_filter_settings(self, g_min, g_max, f_min, f_max, filter_min, filter_max, EnablePreparation, EnableFiltering, FilterMinH, FilterMaxH, Graphic):
        self.Gmin = g_min
        self.Gmax = g_max
        self.Fmin = f_min
        self.Fmax = f_max
        self.FilterMin = filter_min
        self.FilterMax = filter_max
        self.EnablePreparation = EnablePreparation
        self.EnableFiltering = EnableFiltering
        self.FilterMinH = FilterMinH
        self.FilterMaxH = FilterMaxH
        self.Graphic = Graphic

    def on_apply_pressed(self):
        ApplyButton(self)


def ShowWindow():
    app = QApplication([])
    window = MainWindow()
    window.show()
    app.exec_()
