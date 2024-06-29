#pragma once
#include "Light.h"

class PointLight : public Light {
protected:
    glm::vec3 position;

    // ax^2 + bx + c
    // a - exponent
    // b - linear
    // c - constant
    // Attenuation Control
    GLfloat constant, linear, exponent;
public:
    // Constructor
    PointLight();

    PointLight(  GLfloat red, GLfloat green, GLfloat blue,
                 GLfloat ambIntensity, GLfloat diffIntensity,
                 GLfloat xPos, GLfloat yPos, GLfloat zPos,
                 GLfloat cons, GLfloat lin, GLfloat exp );

    // Function override for different types of light
    void useLight(  GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                    GLuint diffuseIntensityLocation, GLuint positionLocation,
                    GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation  );

    // Destructor
    ~PointLight();
};
