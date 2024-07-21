import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QSlider, QLabel
from PyQt5.QtCore import Qt

class SliderExample(QWidget):
    def __init__(self):
        super().__init__()
        
        self.initUI()
        
    def initUI(self):
        # Create a QVBoxLayout instance
        layout = QVBoxLayout()
        
        # Create a QSlider instance
        self.slider = QSlider(Qt.Horizontal, self)
        self.slider.setMinimum(0)
        self.slider.setMaximum(100)
        self.slider.setValue(50)
        self.slider.setTickPosition(QSlider.TicksBelow)
        self.slider.setTickInterval(10)
        self.slider.valueChanged.connect(self.updateLabel)
        
        # Create a QLabel instance
        self.label = QLabel('50', self)
        
        # Add the slider and label to the layout
        layout.addWidget(self.slider)
        layout.addWidget(self.label)
        
        # Set the layout to the main window
        self.setLayout(layout)
        
        # Set the window title
        self.setWindowTitle('QSlider Example')
        self.show()
        
    def updateLabel(self, value):
        self.label.setText(str(value))

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = SliderExample()
    sys.exit(app.exec_())
