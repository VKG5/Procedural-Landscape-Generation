#pragma once

#include "PointLight.h"

class SpotLight : public PointLight {
private:
    glm::vec3 direction;

    GLfloat edge, processedEdge;

    bool isOn = false;

public:
    // Constructor
    SpotLight();

    SpotLight(  GLfloat red, GLfloat green, GLfloat blue,
                GLfloat ambIntensity, GLfloat diffIntensity,
                GLfloat xPos, GLfloat yPos, GLfloat zPos,
                GLfloat xDir, GLfloat yDir, GLfloat zDir,
                GLfloat cons, GLfloat lin, GLfloat exp,
                GLfloat edg );

    // Function override for different types of light
    void useLight(  GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                    GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
                    GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
                    GLuint edgeLocation  );

    void setFlash(glm::vec3 pos, glm::vec3 dir);

    void toggle() { isOn = !isOn; }

    // Destructor
    ~SpotLight();
};
