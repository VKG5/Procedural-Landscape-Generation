// STB
#define STB_IMAGE_IMPLEMENTATION

// Including all necessary headers
// ImGUI
#include "GUI.h"

// Basic C++ Libraries for various operations
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>
#include <GLFW/glfw3.h>

// GLM Files - Math Library
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Custom Libraries
#include "Window.h"
#include "Utilities.h"
#include "MathFuncs.h"
#include "Scene.h"

// To force the use of dedicated GPU
// extern "C" {
// __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
// }

// Get the full path of the current source file
const std::filesystem::path currentSourcePath = __FILE__;

// Extract the directory containing the source file
const std::filesystem::path currentSourceDir = currentSourcePath.parent_path();

// Seed for Procedural Content
GLuint seed = 165316;

// Setting a random function to use
std::mt19937 gen(seed);

// Delta Time
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Main Function=======================================================================================================
int main()
{
    // Our main window
    Window mainWindow(1366, 768);
    mainWindow.initialize();

    // Our scene
    Scene mainScene(mainWindow, seed);
    mainScene.setupScene(currentSourceDir);

    // Main Loop - Running till the window is open=====================================================================
    while(!mainWindow.getShouldClose()) {
        // For Delta Time
        GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
        deltaTime = now - lastTime;  // (now - lastTime)*1000 / SDL_GetPerformanceFrequency(); - ms
        lastTime = now;

        // Get + Handle user input events
        glfwPollEvents();

        // Scene update called here
        mainScene.update(deltaTime);

        // Un-Binding the program
        glUseProgram(0);

        // We have 2 scenes, one which we are drawing to and one current
        mainWindow.swapBuffers();
    }

    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}
