from PySide6.QtWidgets import QDialog, QSpinBox, QCheckBox, QDialogButtonBox, QVBoxLayout, QHBoxLayout, QLabel, QRadioButton, QButtonGroup


class FilterSettingsWindow(QDialog):
    def __init__(self, g_min, g_max, f_min, f_max, filter_min, filter_max, EnablePreparation, EnableFiltering, FilterMinH, FilterMaxH, Graphic, parent=None):
        super().__init__(parent=parent)
        self.setWindowTitle("Filter Settings")

        # Create instance variables for the widgets
        self.GminSpinBox = QSpinBox(self)
        self.GmaxSpinBox = QSpinBox(self)
        self.FminSpinBox = QSpinBox(self)
        self.FmaxSpinBox = QSpinBox(self)
        self.FilterMinCheckBox = QCheckBox(self)
        self.FilterMinH1CheckBox = QRadioButton(self)
        self.FilterMinH2CheckBox = QRadioButton(self)
        self.FilterMinH3CheckBox = QRadioButton(self)
        self.FilterMaxCheckBox = QCheckBox(self)
        self.FilterMaxH1CheckBox = QRadioButton(self)
        self.FilterMaxH2CheckBox = QRadioButton(self)
        self.FilterMaxH3CheckBox = QRadioButton(self)
        self.EnablePreparation = QCheckBox(self)
        self.EnableFiltering = QCheckBox(self)
        self.GraphicE = QRadioButton(self)
        self.GraphicD = QRadioButton(self)

        # Create button groups for the radio buttons
        self.FilterMinGroup = QButtonGroup(self)
        self.FilterMinGroup.addButton(self.FilterMinH1CheckBox)
        self.FilterMinGroup.addButton(self.FilterMinH2CheckBox)
        self.FilterMinGroup.addButton(self.FilterMinH3CheckBox)
        self.FilterMinGroup.setExclusive(True)

        self.FilterMaxGroup = QButtonGroup(self)
        self.FilterMaxGroup.addButton(self.FilterMaxH1CheckBox)
        self.FilterMaxGroup.addButton(self.FilterMaxH2CheckBox)
        self.FilterMaxGroup.addButton(self.FilterMaxH3CheckBox)
        self.FilterMaxGroup.setExclusive(True)

        self.GraphicGroup = QButtonGroup(self)
        self.GraphicGroup.addButton(self.GraphicE)
        self.GraphicGroup.addButton(self.GraphicD)
        self.GraphicGroup.setExclusive(True)

        # Set the values of the radio buttons
        if FilterMinH == 1:
            self.FilterMinH1CheckBox.setChecked(True)
        elif FilterMinH == 2:
            self.FilterMinH2CheckBox.setChecked(True)
        elif FilterMinH == 3:
            self.FilterMinH3CheckBox.setChecked(True)

        if FilterMaxH == 1:
            self.FilterMaxH1CheckBox.setChecked(True)
        elif FilterMaxH == 2:
            self.FilterMaxH2CheckBox.setChecked(True)
        elif FilterMaxH == 3:
            self.FilterMaxH3CheckBox.setChecked(True)

        if Graphic == "E":
            self.GraphicE.setChecked(True)
        elif Graphic == "D":
            self.GraphicD.setChecked(True)

        # Set the values of the widgets
        self.GminSpinBox.setMinimum(0)
        self.GminSpinBox.setMaximum(255)
        self.GminSpinBox.setValue(g_min)

        self.GmaxSpinBox.setMinimum(0)
        self.GmaxSpinBox.setMaximum(255)
        self.GmaxSpinBox.setValue(g_max)

        self.FminSpinBox.setMinimum(0)
        self.FminSpinBox.setMaximum(255)
        self.FminSpinBox.setValue(f_min)

        self.FmaxSpinBox.setMinimum(0)
        self.FmaxSpinBox.setMaximum(255)
        self.FmaxSpinBox.setValue(f_max)

        self.FilterMinCheckBox.setChecked(filter_min)
        self.FilterMaxCheckBox.setChecked(filter_max)

        self.EnablePreparation.setChecked(EnablePreparation)
        self.EnableFiltering.setChecked(EnableFiltering)

        # Create the layout and add the widgets
        MainLayout = QVBoxLayout()

        ElementProcessingLayout = QHBoxLayout()
        ElementProcessing = QLabel(self)
        ElementProcessing.setText("Element Processing Enabled")
        ElementProcessingLayout.addWidget(ElementProcessing)
        ElementProcessingLayout.addWidget(self.EnablePreparation)
        MainLayout.addLayout(ElementProcessingLayout)

        # G setting layout
        GLayout = QHBoxLayout()

        GminLabel = QLabel(self)
        GminLabel.setText("Gmin")
        GLayout.addWidget(GminLabel)
        GLayout.addWidget(self.GminSpinBox)

        GmaxLabel = QLabel(self)
        GmaxLabel.setText("Gmax")
        GLayout.addWidget(GmaxLabel)
        GLayout.addWidget(self.GmaxSpinBox)

        MainLayout.addLayout(GLayout)

        # F setting layout
        FLayout = QHBoxLayout()

        FminLabel = QLabel(self)
        FminLabel.setText("Fmin")
        FLayout.addWidget(FminLabel)
        FLayout.addWidget(self.FminSpinBox)

        FmaxLabel = QLabel(self)
        FmaxLabel.setText("Fmax")
        FLayout.addWidget(FmaxLabel)
        FLayout.addWidget(self.FmaxSpinBox)

        MainLayout.addLayout(FLayout)

        # Type of graphic
        GraphicLayout = QHBoxLayout()
        
        GraphicLabelD = QLabel(self)
        GraphicLabelD.setText("Graphic D")
        GraphicLayout.addWidget(GraphicLabelD)
        GraphicLayout.addWidget(self.GraphicD)

        GraphicLabelE = QLabel(self)
        GraphicLabelE.setText("Graphic E")
        GraphicLayout.addWidget(GraphicLabelE)
        GraphicLayout.addWidget(self.GraphicE)

        MainLayout.addLayout(GraphicLayout)
        # Adding spacer to lower place of elements
        MainLayout.addWidget(QLabel(" "))

        # Filters
        FilterEnableLayout = QHBoxLayout()
        FiltersLabel = QLabel(self)
        FiltersLabel.setText("Filtering enabled")
        FilterEnableLayout.addWidget(FiltersLabel)
        FilterEnableLayout.addWidget(self.EnableFiltering)
        MainLayout.addLayout(FilterEnableLayout)

        # Filters layout
        FiltersLayout = QVBoxLayout()

        # Filter Min
        FilterMinLayout = QHBoxLayout()
        FilterMinLabel = QLabel(self)
        FilterMinLabel.setText("Min")
        FilterMinLayout.addWidget(FilterMinLabel)
        FilterMinLayout.addWidget(self.FilterMinCheckBox)
        # Adding separator
        FilterMinLayout.addWidget(QLabel(" "))

        FilterMinH1Label = QLabel(self)
        FilterMinH1Label.setText("H1")
        FilterMinLayout.addWidget(FilterMinH1Label)
        FilterMinLayout.addWidget(self.FilterMinH1CheckBox)

        FilterMinH2Label = QLabel(self)
        FilterMinH2Label.setText("H2")
        FilterMinLayout.addWidget(FilterMinH2Label)
        FilterMinLayout.addWidget(self.FilterMinH2CheckBox)

        FilterMinH3Label = QLabel(self)
        FilterMinH3Label.setText("H3")
        FilterMinLayout.addWidget(FilterMinH3Label)
        FilterMinLayout.addWidget(self.FilterMinH3CheckBox)

        FiltersLayout.addLayout(FilterMinLayout)
        # Filter Max
        FilterMaxLayout = QHBoxLayout()
        FilterMaxLabel = QLabel(self)
        FilterMaxLabel.setText("Max")
        FilterMaxLayout.addWidget(FilterMaxLabel)
        FilterMaxLayout.addWidget(self.FilterMaxCheckBox)
        # Adding separator
        FilterMaxLayout.addWidget(QLabel(" "))

        FilterMaxH1Label = QLabel(self)
        FilterMaxH1Label.setText("H1")
        FilterMaxLayout.addWidget(FilterMaxH1Label)
        FilterMaxLayout.addWidget(self.FilterMaxH1CheckBox)

        FilterMaxH2Label = QLabel(self)
        FilterMaxH2Label.setText("H2")
        FilterMaxLayout.addWidget(FilterMaxH2Label)
        FilterMaxLayout.addWidget(self.FilterMaxH2CheckBox)

        FilterMaxH3Label = QLabel(self)
        FilterMaxH3Label.setText("H3")
        FilterMaxLayout.addWidget(FilterMaxH3Label)
        FilterMaxLayout.addWidget(self.FilterMaxH3CheckBox)

        FiltersLayout.addLayout(FilterMaxLayout)

        MainLayout.addLayout(FiltersLayout)

        # Add the buttons
        button_box = QDialogButtonBox(
            QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        button_box.accepted.connect(self.accept)
        button_box.rejected.connect(self.reject)
        MainLayout.addWidget(button_box)

        self.setLayout(MainLayout)

    def accept(self):
        # Retrieve the values of the widgets and save them
        g_min = self.GminSpinBox.value()
        g_max = self.GmaxSpinBox.value()
        f_min = self.FminSpinBox.value()
        f_max = self.FmaxSpinBox.value()
        filter_min = self.FilterMinCheckBox.isChecked()
        filter_max = self.FilterMaxCheckBox.isChecked()
        EnablePreparation = self.EnablePreparation.isChecked()
        EnableFiltering = self.EnableFiltering.isChecked()

        # Filters H1, H2, H3
        filter_min_h = 0
        if self.FilterMinH1CheckBox.isChecked():
            filter_min_h = 1
        elif self.FilterMinH2CheckBox.isChecked():
            filter_min_h = 2
        elif self.FilterMinH3CheckBox.isChecked():
            filter_min_h = 3

        filter_max_h = 1
        if self.FilterMaxH2CheckBox.isChecked():
            filter_max_h = 2
        elif self.FilterMaxH3CheckBox.isChecked():
            filter_max_h = 3

        Graphic = "D"
        if self.GraphicE.isChecked():
            Graphic = "E"

        # Send values to the parent class
        self.parent().set_filter_settings(g_min, g_max, f_min, f_max,
                                          filter_min, filter_max,
                                          EnablePreparation, EnableFiltering,
                                          filter_min_h, filter_max_h, Graphic)

        # Call the base class accept method to close the dialog
        super().accept()

    def on_cancel_pressed(self):
        self.close()
