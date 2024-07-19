import sys
from PyQt5.QtWidgets import QApplication, QWidget

def main():
    # Create an instance of QApplication
    app = QApplication(sys.argv)

    # Create a window
    window = QWidget()
    window.setWindowTitle('My GUI')
    window.setGeometry(100, 100, 300, 200)  # Set the window size and position

    # Show the window
    window.show()

    # Start the event loop
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()