# Importing required libraries
import sys
import os
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QLabel, QLineEdit
from PyQt5.QtGui import QPixmap, QPalette, QColor
from PyQt5.QtCore import QProcess, Qt


# <---------------------- Global Variables ----------------------->
# Get location of the current file
directory = os.path.dirname(os.path.realpath(__file__))

# Replace backslashes with forward slashes
dir = directory.replace('\\', '/')

# Concatenate the path to the executable
executablePath = dir + '/OpenGL/build/src/Rendering/Debug/Executable.exe'
imagePath = dir + '/stable-diffusion-webui/outputs/'

# <---------------------- Main Window ----------------------->
class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('Yumi - Landscape Generator')
        self.setGeometry(100, 100, 400, 300)
        
        # Layout
        layout = QVBoxLayout()
        
        # Generate Image Button
        self.btnGenerateImage = QPushButton('Generate Image')
        self.btnGenerateImage.clicked.connect(self.generateImage)
        layout.addWidget(self.btnGenerateImage)
        
        # Prompt Text Field
        self.promptTextField = QLineEdit('Enter prompt:')
        layout.addWidget(self.promptTextField)

        # Launch Application Button
        self.btnLaunchApp = QPushButton('Launch Application')
        self.btnLaunchApp.clicked.connect(self.launchApplication)
        layout.addWidget(self.btnLaunchApp)

        # Quit Button
        self.btnQuit = QPushButton('Quit')
        self.btnQuit.clicked.connect(self.quitApp)
        layout.addWidget(self.btnQuit)

        # Image Preview
        self.imageLabel = QLabel('Image Preview')
        self.imageLabel.setMaximumSize(2048, 2048)
        layout.addWidget(self.imageLabel)
        
        # Path Text Field
        self.pathTextField = QLineEdit('Generated Image Path:')
        self.pathTextField.setReadOnly(True)
        layout.addWidget(self.pathTextField)

        self.setLayout(layout)

        # Pointer to the C++ process
        self.process = None

    def generateImage(self):
        # Placeholder for image generation logic
        # Here, just display a placeholder image
        self.imageLabel.setPixmap(QPixmap(imagePath + '/img2img-images/2024-06-25/00000-1723017853.png'))
        self.imageLabel.setAlignment(Qt.AlignCenter)

        self.pathTextField.setText(imagePath + 'img2img-images/2024-06-25/00000-1723017853.png')
    
    def quitApp(self):
        # Quit the launched application if it is running
        if self.process and self.process.state() == QProcess.Running:
            ## Debugging
            print(f"Quitting application with PID : {self.process.processId()}")
            
            self.process.kill()
            self.process.waitForFinished()
            self.process = None
        
        else:
            print("No application is running.")
            
        # Quit the GUI application
        QApplication.quit()
    
    def launchApplication(self):
        # If process is already running, do not start another instance
        if self.process and self.process.state() == QProcess.Running:
            print("C++ Application is already running.")
            return

        ## Debugging
        print(f"Starting C++ application from : \"{executablePath}\"")
        
        self.process = QProcess()
        self.process.start(executablePath)

        if self.process.waitForStarted():
            print(f"C++ Application started with PID : {self.process.processId()}")
        
        else:
            print("Failed to start C++ application.")


# <---------------------- Main Application ----------------------->
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


# <---------------------- Entry Point ----------------------->
if __name__ == '__main__':
    main()