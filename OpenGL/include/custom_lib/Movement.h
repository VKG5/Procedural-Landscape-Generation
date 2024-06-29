#pragma once

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>
#include <glfw/glfw3.h>

// GLM Files - Math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Movement {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    // Global up
    glm::vec3 worldUp;

    GLfloat roll;
    GLfloat pitch;
    GLfloat yaw;

    GLfloat moveSpeed;
    GLfloat turnSpeed;

    void update();

public:
    // Constructor
    Movement();

    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);

    // Getter for specular highlight data
    glm::vec3 getMovementPosition();
    glm::vec3 getMovementDirection();

    void updateMoveSpeed(GLfloat newMove, GLfloat newTurn);

    glm::vec3 mapRange(const glm::vec3& input);

    // Destructor
    ~Movement();
};
