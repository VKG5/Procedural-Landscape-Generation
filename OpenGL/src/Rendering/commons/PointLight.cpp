#include "PointLight.h"

PointLight::PointLight() : Light() {
    // Position of the light
    position = glm::vec3(0.0f, 1.0f, 0.0f);

    // Attenuation Values
    // L / (ax^2 + bx + c)
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}

PointLight::PointLight( GLfloat red, GLfloat green, GLfloat blue,
                        GLfloat ambIntensity, GLfloat diffIntensity,
                        GLfloat xPos, GLfloat yPos, GLfloat zPos,
                        GLfloat cons, GLfloat lin, GLfloat exp ) : Light(red, green, blue, ambIntensity, diffIntensity) {
    // Position of the light
    position = glm::vec3(xPos, yPos, zPos);

    // Attenuation Values
    constant = cons;
    linear = lin;
    exponent = exp;
}

void PointLight::useLight(  GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                            GLuint diffuseIntensityLocation, GLuint positionLocation,
                            GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation  ) {
    // Ambient Light
    glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);

    // Diffuse Light
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

    // Point Light
    glUniform3f(positionLocation, position.x, position.y, position.z);

    // Attenuation Factor
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight() {
}
