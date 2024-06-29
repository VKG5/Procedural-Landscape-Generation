#include "SpotLight.h"

SpotLight::SpotLight() : PointLight() {
    direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
    edge = 0.0f;
    processedEdge = glm::cos(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
                GLfloat ambIntensity, GLfloat diffIntensity,
                GLfloat xPos, GLfloat yPos, GLfloat zPos,
                GLfloat xDir, GLfloat yDir, GLfloat zDir,
                GLfloat cons, GLfloat lin, GLfloat exp,
                GLfloat edg ) : PointLight(red, green, blue, ambIntensity, diffIntensity, xPos, yPos, zPos, cons, lin, exp) {
    direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
    edge = edg;

    processedEdge = glm::cos(glm::radians(edge));
}

void SpotLight::useLight(   GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                            GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
                            GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
                            GLuint edgeLocation ) {
    // Ambient Light
    glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);

    if(isOn) {
        glUniform1f(ambientIntensityLocation, ambientIntensity);
        // Diffuse Light
        glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    }

    else {
        glUniform1f(ambientIntensityLocation, 0.0f);
        // Diffuse Light
        glUniform1f(diffuseIntensityLocation, 0.0f);
    }

    // Point Light
    glUniform3f(positionLocation, position.x, position.y, position.z);

    // Attenuation Factor
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);

    // SpotLight Factors
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir) {
    position = pos;
    direction = dir;
}

SpotLight::~SpotLight() {
}
