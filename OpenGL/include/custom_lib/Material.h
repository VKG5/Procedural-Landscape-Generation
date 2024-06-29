#pragma once

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>

// This class contains the material properties
class Material {
private:
    GLfloat specularIntensity;
    GLfloat shininess;
    GLfloat metalness;

public:
    // Constructor
    Material();

    // Overloaded Constructor with custom values
    Material(GLfloat specIntensity, GLfloat shine, GLfloat metal);

    // Bind the current material values to the program
    void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation, GLuint metalnessLocation);

    // Getters=========================================================================================================
    const GLfloat getSpecularIntensity() { return specularIntensity; }
    const GLfloat getShininess() { return shininess; }
    const GLfloat getMetalness() { return metalness; }

    // Setters=========================================================================================================
    void setMaterialParamters(GLfloat specularIntensity, GLfloat shininess, GLfloat metal);

    // Destructor
    ~Material();
};
