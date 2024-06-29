#include "Movement.h"

Movement::Movement() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90;
    pitch = 0;

    front = glm::vec3(0.0f, 0.0f, 1.0f);

    moveSpeed = 5.0f;
    turnSpeed = 1.0f;

    update();
}

void Movement::keyControl(bool* keys, GLfloat deltaTime) {
    GLfloat velocity = moveSpeed * deltaTime;

    // Move Forward
    if(keys[GLFW_KEY_W]) {
        position += front * velocity;
    }

    // Move Backward
    if(keys[GLFW_KEY_S]) {
        position -= front * velocity;
    }

    // Move Right
    if(keys[GLFW_KEY_D]) {
        position += right * velocity;
    }

    // Move Left
    if(keys[GLFW_KEY_A]) {
        position -= right * velocity;
    }

    // Move Up
    if(keys[GLFW_KEY_Q]) {
        position += worldUp * velocity;
    }

    // Move Down
    if(keys[GLFW_KEY_E]) {
        position -= worldUp * velocity;
    }
}

void Movement::mouseControl(GLfloat xChange, GLfloat yChange) {
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    // Limiting the Pitch
    if(pitch > 89.0f) {
        pitch = 89.0f;
    }

    if(pitch < -89.0f) {
        pitch = -89.0f;
    }

    update();
}

// Updating the Up, Front, and Right
void Movement::update() {
    // Calculating the front of the camera
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(front);

    right = glm::normalize(glm::cross(normalize(front), worldUp));
    up = glm::normalize(glm::cross(normalize(front), front));
}

// Changing Move Speed
void Movement::updateMoveSpeed(GLfloat newMove, GLfloat newTurn) {
    moveSpeed = newMove;
    turnSpeed = newTurn;
}

// Getters
glm::vec3 Movement::getMovementPosition() {
    return position;
}

// Function to map values from [-1.0, 1.0] to [-360.0, 360.0]
glm::vec3 Movement::mapRange(const glm::vec3& input) {
    return glm::vec3(
        glm::clamp((input.x + 1.0f) * 360.0f, 0.0f, 360.0f),
        glm::clamp((input.y + 1.0f) * 360.0f, 0.0f, 360.0f),
        glm::clamp((input.z + 1.0f) * 360.0f, 0.0f, 360.0f)
    );
}

glm::vec3 Movement::getMovementDirection() {
    // return glm::normalize(front);
    return mapRange(front);
}

Movement::~Movement() {
}
