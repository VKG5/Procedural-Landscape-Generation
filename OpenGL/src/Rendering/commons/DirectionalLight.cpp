#include "DirectionalLight.h"

// Calling Super Contructor
DirectionalLight::DirectionalLight() : Light() {
    // Diffuse Light
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    dirColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

DirectionalLight::DirectionalLight( GLfloat red, GLfloat green, GLfloat blue,
                                    GLfloat ambIntensity, GLfloat diffIntensity,
                                    GLfloat xDir, GLfloat yDir, GLfloat zDir ) : Light(red, green, blue, ambIntensity, diffIntensity) {
    // Diffuse Light
    direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::useLight( GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                                 GLuint diffuseIntensityLocation, GLuint directionLocation  ) {
    // Ambient Light
    glUniform3f(ambientColourLocation, dirColor.x, dirColor.y, dirColor.z);
    glUniform1f(ambientIntensityLocation, ambIntensity);

    // Diffuse Light
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffIntensity);
}

void DirectionalLight::setDirLight( GLfloat red, GLfloat green, GLfloat blue,
                                    GLfloat amb, GLfloat diff,
                                    GLfloat x, GLfloat y, GLfloat z ) {
    // Setting the color
    dirColor = glm::vec3(red, green, blue);

    // Setting the intensities
    ambIntensity = amb;
    diffIntensity = diff;

    // Setting the direction
    direction = glm::vec3(x, y, z);
}

DirectionalLight::~DirectionalLight() {
}
