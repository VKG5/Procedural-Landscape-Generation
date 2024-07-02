#pragma once

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>

#include <iostream>

#include "Utilities.h"

class Texture {
private:
    GLuint textureID;
    int width, height, bitDepth;

    const char* filePath;

    unsigned char* texData;

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

    // Debugger
    void printTextureInfo() const;

    void freeTextureData() const;

    // Getters=========================================================================================================
    int getHeight() const { return height; }
    int getWidth() const { return width; }
    int getBitDepth() const { return bitDepth; }
    const unsigned char* getTexData() const { return texData; }

    // Destructor
    ~Texture();
};
