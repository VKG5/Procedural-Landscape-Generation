import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QLabel
from PyQt5.QtGui import QPixmap, QPalette, QColor
from PyQt5.QtCore import QProcess, Qt

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('My GUI with Buttons')
        self.setGeometry(100, 100, 400, 300)
        
        # Layout
        layout = QVBoxLayout()
        
        # Generate Image Button
        self.btnGenerateImage = QPushButton('Generate Image')
        self.btnGenerateImage.clicked.connect(self.generateImage)
        layout.addWidget(self.btnGenerateImage)
        
        # Launch Application Button
        self.btnLaunchApp = QPushButton('Launch Application')
        self.btnLaunchApp.clicked.connect(self.launchApplication)
        layout.addWidget(self.btnLaunchApp)

        # Quit Button
        self.btnQuit = QPushButton('Quit')
        self.btnQuit.clicked.connect(self.quitApp)
        layout.addWidget(self.btnQuit)

        # Image Preview
        self.imageLabel = QLabel()
        self.imageLabel.setFixedSize(200, 200)
        layout.addWidget(self.imageLabel)
        
        self.setLayout(layout)
    
    def generateImage(self):
        # Placeholder for image generation logic
        # Here, just display a placeholder image
        self.imageLabel.setPixmap(QPixmap('path/to/generated/image.png'))
    
    def quitApp(self):
        QApplication.quit()
    
    def launchApplication(self):
        # Placeholder for launching an application
        # Example: Launching Notepad on Windows
        QProcess.startDetached("D:/Programs/Python/Thesis/OpenGL/build/src/Rendering/Debug/Executable.exe")

def main():
    app = QApplication(sys.argv)
    app.setStyle("Fusion")

    # Making a custom palette for dark mode
    dark_palette = QPalette()
    dark_palette.setColor(QPalette.Window, QColor(53, 53, 53))
    dark_palette.setColor(QPalette.WindowText, Qt.white)
    dark_palette.setColor(QPalette.Base, QColor(25, 25, 25))
    dark_palette.setColor(QPalette.AlternateBase, QColor(53, 53, 53))
    dark_palette.setColor(QPalette.ToolTipBase, Qt.white)
    dark_palette.setColor(QPalette.ToolTipText, Qt.white)
    dark_palette.setColor(QPalette.Text, Qt.white)
    dark_palette.setColor(QPalette.Button, QColor(53, 53, 53))
    dark_palette.setColor(QPalette.ButtonText, Qt.white)
    dark_palette.setColor(QPalette.BrightText, Qt.red)
    dark_palette.setColor(QPalette.Link, QColor(42, 130, 218))
    dark_palette.setColor(QPalette.Highlight, QColor(42, 130, 218))
    dark_palette.setColor(QPalette.HighlightedText, Qt.black)

    app.setPalette(dark_palette)
    app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }")

    window = MainWindow()
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()