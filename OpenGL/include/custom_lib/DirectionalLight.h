#pragma once
#include "Light.h"

class DirectionalLight : public Light {
private:
    glm::vec3 direction;
    glm::vec3 dirColor;
    GLfloat ambIntensity;
    GLfloat diffIntensity;

public:
    // Constructor
    DirectionalLight();

    DirectionalLight( GLfloat red, GLfloat green, GLfloat blue,
                      GLfloat ambIntensity, GLfloat diffIntensity,
                      GLfloat xDir, GLfloat yDir, GLfloat zDir );

    // Function override for different types of light
    void useLight(  GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                    GLuint diffuseIntensityLocation, GLuint directionLocation  );

    void setDirLight( GLfloat red, GLfloat green, GLfloat blue,
                      GLfloat amb, GLfloat diff,
                      GLfloat x, GLfloat y, GLfloat z );

    // Destructor
    ~DirectionalLight();
};
