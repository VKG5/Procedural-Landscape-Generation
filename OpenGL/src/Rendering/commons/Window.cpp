#include <iostream>
#include "Window.h"

Window::Window() {
    width = 800;
    height = 600;
    xChange = 0.0f;
    yChange = 0.0f;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
}

Window::Window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;
    xChange = 0.0f;
    yChange = 0.0f;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
}

int Window::initialize() {
    // Making a basic window in GLFW
    if(!glfwInit()) {
        std::cout<<"GLFW Failed to initialize";
        glfwTerminate();
        exit( EXIT_FAILURE );
    }

    // Setting up GLFW Window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE
    );

    // Core profile = No backwards compatiblity
    glfwWindowHint (
        GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE
    );

    // Set the GLFW window hint for resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    mainWindow = glfwCreateWindow(width, height, "~Yumi~", NULL, NULL);

    if(!mainWindow) {
        std::cout<<"GLFW Window creation failed!";
        glfwTerminate();
        return -1;
    }

    // Get buffer size info
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight );

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Handle Key + Mouse Inputs
    createCallbacks();

    // Limiting mouse movement to window
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize the OpenGL context using Glad or GLEW
    // Initialize Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize Glad" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return -1;
    }

    // Enabling Depth Buffer
    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    // x,y - Top left corner position
    // bufferWidth and bufferHeight - Give us the height in between the window
    glViewport( 0, 0, bufferWidth, bufferHeight );

    // Passing in a pointer to the whole class itself instead of a function
    glfwSetWindowUserPointer(mainWindow, this);

    return 0;
}

void Window::createCallbacks() {
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
    glfwSetFramebufferSizeCallback(mainWindow, handleResizeWindow);
}

GLfloat Window::getXChange() {
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange() {
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

void Window::changeState(bool enable) {
    if(enable) {
        // Limiting mouse movement to window
        glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    else {
        // Un-restricted movement
        glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    // Getting access to our current window
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    // Close Window/Escape Control
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Setting the key pointers to true/false
    if(key >= 0 && key < 1024) {
        if(action == GLFW_PRESS) {
            theWindow->keys[key] = true;

            // Debugging
            //printf("Pressed %d\n", key);
        }

        else if(action == GLFW_RELEASE) {
            theWindow->keys[key] = false;

            // Debugging
            //printf("Released %d\n", key);
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
    // Getting access to our current window
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    // For initial movement of mouse
    if(theWindow->mouseFirstMoved) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;

    // Avoid inverted up-down
    theWindow->yChange = theWindow->lastY - yPos;

    // Storing for next iteration
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    // Debugging
    //printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

void Window::handleResizeWindow(GLFWwindow* window, int width, int height) {
    // Debugging
    // printf("Resizing Window");

    // Resizing the frame buffers
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    theWindow->bufferWidth = width;
    theWindow->bufferHeight = height;

    // Set the viewport size to match the resized window
    glViewport(0, 0, width, height);
}


Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
