#pragma once

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>

#include "Utilities.h"

class Texture {
private:
    GLuint textureID;
    int width, height, bitDepth;

    const char* filePath;

    // Function to generate noise
    unsigned char* generateNoise();

    // Debugger
    void printTextureInfo();

public:
    // Constructor
    Texture();

    Texture(const char* fileLoc);

    bool loadTexture();
    bool loadTexture(int choice);

    // For supporting multiple textures in one shader
    void useTexture();
    void useTexture(int textureUnit);

    void cleanTexture();

    // Noise
    bool generateRandomTexture(GLuint w, GLuint h, GLuint d);

    // Destructor
    ~Texture();
};
