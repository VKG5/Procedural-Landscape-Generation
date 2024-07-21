# Importing required libraries
import sys
import os
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, \
                            QLabel, QLineEdit, QComboBox, QCheckBox, QHBoxLayout, \
                            QAbstractSlider

from PyQt5.QtGui import QPixmap, QPalette, QColor
from PyQt5.QtCore import QProcess, Qt

import apiLink as api

# <---------------------- Global Variables ----------------------->
# Get location of the current file
directory = os.path.dirname(os.path.realpath(__file__))

# Replace backslashes with forward slashes
dir = directory.replace('\\', '/')

# Concatenate the path to the executable
apiBatchPath = dir + '/stable-diffusion-webui/webui-user.bat'
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
        
        # Elements for controlling the SD API
        # Choosing the type of generation we are akming for
        self.apiTypeComboBox = QComboBox()

        # Elements for the combo box
        # Usually include txt2img, img2img, etc.
        self.apiTypeComboBox.addItem("txt2img")
        self.apiTypeComboBox.addItem("img2img")

        layout.addWidget(self.apiTypeComboBox)

        # Prompt Text Field
        # Adding a new row element for putting fields in the same row
        layout_h = QHBoxLayout()
        self.promptLabel = QLabel('Prompt:')
        self.promptTextField = QLineEdit()
        self.promptTextField.setPlaceholderText('Enter a prompt here...')

        layout_h.addWidget(self.promptLabel)
        layout_h.addWidget(self.promptTextField)

        # Adding the prompt row to the main layout
        layout.addLayout(layout_h)

        # Adding a new row element for putting fields in the same row
        layout_h = QHBoxLayout()
        self.dimensionsLabel = QLabel('Dimensions:')
        self.dimensionsTextField = QLineEdit()
        self.dimensionsTextField.setPlaceholderText('1024')

        layout_h.addWidget(self.dimensionsLabel)
        layout_h.addWidget(self.dimensionsTextField)

        # Adding the dimesnions row to the main layout
        layout.addLayout(layout_h)

        # Adding a new row element for putting fields in the same row
        layout_h = QHBoxLayout()
        self.stepsLabel = QLabel('Steps:')
        self.stepsTextField = QLineEdit()
        self.stepsTextField.setPlaceholderText('99')

        layout_h.addWidget(self.stepsLabel)
        layout_h.addWidget(self.stepsTextField)

        # Adding the dimesnions row to the main layout
        layout.addLayout(layout_h)
        
        # Adding a new row element for putting fields in the same row
        layout_h = QHBoxLayout()
        self.upscaleCheckBox = QCheckBox('Upscale')
        self.upscaleFactorLabel = QLabel('Upscale Factor:')
        self.upscaleFactorTextField = QLineEdit()
        self.upscaleFactorTextField.setPlaceholderText('2')
        
        layout_h.addWidget(self.upscaleCheckBox)
        layout_h.addWidget(self.upscaleFactorLabel)
        layout_h.addWidget(self.upscaleFactorTextField)
        
        # Adding the dimesnions row to the main layout
        layout.addLayout(layout_h)

        # Adding a new row element for putting fields in the same row
        layout_h = QHBoxLayout()
        self.seedLabel = QLabel('Seed:')
        self.seedTextField = QLineEdit()
        self.seedTextField.setPlaceholderText('-1')

        layout_h.addWidget(self.seedLabel)
        layout_h.addWidget(self.seedTextField)

        # Adding the dimesnions row to the main layout
        layout.addLayout(layout_h)

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
        self.imageLabel = QLabel('Image Preview')
        self.imagePreviewField = QLabel()
        self.imagePreviewField.setMaximumSize(2048, 2048)

        layout.addWidget(self.imageLabel)
        layout.addWidget(self.imagePreviewField)

        # Path Text Field
        # Adding a new row element for putting fields in the same row
        layout_h = QHBoxLayout()
        self.pathLabel = QLabel('Generated Image Path:')
        self.pathTextField = QLineEdit()
        self.pathTextField.setPlaceholderText('Path to the generated image...')
        self.pathTextField.setReadOnly(True)
        
        layout_h.addWidget(self.pathLabel)
        layout_h.addWidget(self.pathTextField)

        # Adding the dimesnions row to the main layout
        layout.addLayout(layout_h)

        # Adding the dimesnions row to the main layout
        self.setLayout(layout)

        # Pointer to the C++ process
        self.process = None

        # Pointer to SD API
        self.apiProcess = None

    # Function to start the Stable Diffusion API in the background
    #*For now, the API is started using a batch file manually, the code below is not working
    def startSDAPI(self):
        # If process is already running, do not start another instance
        if self.apiProcess and self.apiProcess.state() == QProcess.Running:
            ## Debugging
            print("SD API is already running.")
            return
        
        ## Debugging
        print(f"Starting SD API from : \"{apiBatchPath}\"")

        self.apiProcess = QProcess()
        self.apiProcess.start('cmd', ['/c', apiBatchPath])

        if self.apiProcess.waitForStarted():
            print(f"SD API started with PID : {self.apiProcess.processId()}")

        else:
            print("Failed to start SD API.")

    # Function to call the SD API and generate images
    def generateImage(self):
        # Starting the API of not already started
        # self.startSDAPI()

        # <---------------------- Call the API ----------------------->
        # api.runStableDiffusionAPIText2Img(type = 'txt2txt', prompt = self.promptTextField.text(), 
        #                                   dimesnions = 1024, steps = 99, 
        #                                   isUpscale = False, upscaleFactor = 2, 
        #                                   seed = -1)

        # Displaying the generated image
        self.imagePreviewField.setPixmap(QPixmap(imagePath + '/img2img-images/2024-06-25/00000-1723017853.png'))
        self.imagePreviewField.setAlignment(Qt.AlignCenter)

        self.pathTextField.setText(imagePath + 'img2img-images/2024-06-25/00000-1723017853.png')
    
    # Function to quit the APIs, applications and processes if running
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

        # Quit the launched API if it is running
        if self.apiProcess and self.apiProcess.state() == QProcess.Running:
            ## Debugging
            print(f"Quitting API with PID : {self.apiProcess.processId()}")
            
            self.apiProcess.kill()
            self.apiProcess.waitForFinished()
            self.apiProcess = None
        
        else:
            print("No API is running.")
            
        # Quit the GUI application
        QApplication.quit()
    
    # Function to launch the C++ application for rendering terrains in real-time using heightmaps
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