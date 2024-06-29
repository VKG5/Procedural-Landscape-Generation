#include "Camera.h"

Camera::Camera() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90;
    pitch = 0;

    front = glm::vec3(0.0f, 0.0f, 1.0f);

    moveSpeed = 5.0f;
    turnSpeed = 1.0f;

    // Projection
    FOV = 45.0f;
    scale = 7.135f;
    nearClipping = 0.1f;
    farClipping = 100.f;

    update();
}

Camera::Camera( glm::vec3 initialPosition, glm::vec3 initialUp, GLfloat initialYaw,
                GLfloat initialPitch, GLfloat initialTurnSpeed, GLfloat initialMoveSpeed ) {
    position = initialPosition;
    worldUp = initialUp;
    yaw = initialYaw;
    pitch = initialPitch;

    front = glm::vec3(0.0f, 0.0f, 1.0f);

    moveSpeed = initialMoveSpeed;
    turnSpeed = initialTurnSpeed;

    // Projection
    FOV = 45.0f;
    scale = 7.135f;
    nearClipping = 0.1f;
    farClipping = 100.f;

    update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {
    GLfloat multiplier = 1.0f;

    // Sprint/Increase speed condition
    if(keys[GLFW_KEY_LEFT_SHIFT]) {
        multiplier = 5.0f;
    }

    GLfloat velocity = moveSpeed * deltaTime * multiplier;

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

    // Resetting to origin with default camera position
    if(keys[GLFW_KEY_0]) {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        pitch = 0.0f;
        yaw = -90.0f;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
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
void Camera::update() {
    // Calculating the front of the camera
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

// Getters=============================================================================================================
glm::vec3 Camera::getCameraPosition() {
    return position;
}

glm::vec3 Camera::getCameraDirection() {
    return glm::normalize(front);
}

glm::vec3 Camera::getCameraDirectionMagnitude() {
    // Function to map values from [-1.0, 1.0] to [-360.0, 360.0]
    return glm::vec3( glm::clamp((front.x + 1.0f) * 360.0f, 0.0f, 360.0f),
                      glm::clamp((front.y + 1.0f) * 360.0f, 0.0f, 360.0f),
                      glm::clamp((front.z + 1.0f) * 360.0f, 0.0f, 360.0f) );
}

glm::mat4 Camera::calculateViewMatrix() {
    // Position, where we are look at from
    // Thing we are looking at
    // Where up is in the world
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::calculateViewMatrix(bool isLeftEye, const float& IOD, const float& convergenceDistance) {
    // Calculate eye offset: half of IOD to the left or right
    float eyeOffset = (isLeftEye ? -1 : 1) * IOD / 2.0f;

    // Calculate the offset position
    glm::vec3 right = glm::normalize(glm::cross(front, up));
    glm::vec3 eyePosition = position + (right * eyeOffset);

    // Calculate the convergence point (focal point)
    glm::vec3 convergencePoint = position + front * convergenceDistance;

    // Calculate the toe-in rotation by adjusting the front vector
    // Note: This is a simplified approximation. For precise control, consider more complex transformations
    float toeInAngle = atan(eyeOffset / convergenceDistance);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), toeInAngle, up);
    glm::vec3 rotatedFront = glm::vec3(rotation * glm::vec4(front, 0.0f));

    // Return the lookAt matrix using the adjusted position and front vector
    return glm::lookAt(eyePosition, eyePosition + rotatedFront, up);
}

glm::mat4 Camera::calculatePerspectiveProjectionMatrix(const GLint& width, const GLint& height) {
    return glm::perspective(glm::radians(FOV), GLfloat(width)/GLfloat(height), nearClipping, farClipping);
}

glm::mat4  Camera::calculateAsymmetricFrustum(bool isLeftEye, const float& IOD, const float& convergenceDistance, const GLint& width, const GLint& height) {
    // Calculate eye offset: half of IOD to the left or right
    float eyeOffset = (isLeftEye ? -1 : 1) * IOD / 2.0f;

    float halfFOV = FOV / 2.0f;
    float top = tan(halfFOV) * nearClipping;
    float bottom = -top;
    float right = (GLfloat(width)/GLfloat(height)) * top;
    float left = -right;

    // Apply the eye position offset to create an asymmetric frustum
    left += eyeOffset * nearClipping / convergenceDistance;
    right += eyeOffset * nearClipping / convergenceDistance;

    // Using GLM to create the frustum matrix
    return glm::frustum(left, right, bottom, top, nearClipping, farClipping);
}

glm::mat4 Camera::calculateOrthographicProjectionMatrix() {
    return glm::ortho(-scale, scale, -scale, scale, nearClipping, farClipping);
}

glm::vec3 Camera::getRayDirection(GLfloat mouseX, GLfloat mouseY, int screenWidth, int screenHeight) {
    // Converting mouse coordinates to NDC (Normalized Device Coordinates)
    glm::vec4 rayClip = glm::vec4((2.0f * mouseX) / screenWidth - 1.0f, 1.0f - (2.0f * mouseY) / screenHeight, -1.0f, 1.0f);

    // Transform ray from clip to eye space
    glm::vec4 rayEye = glm::inverse(this->calculatePerspectiveProjectionMatrix(screenWidth, screenHeight)) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // Transform ray from eye to world space
    glm::vec3 rayWorld = glm::vec3(glm::inverse(this->calculateViewMatrix()) * rayEye);
    rayWorld = glm::normalize(rayWorld);

    // Gives the direction vector of the ray in world space
    return rayWorld;
}

glm::vec3 Camera::getRayHitCoords(GLfloat mouseX, GLfloat mouseY, int screenWidth, int screenHeight) {
// Read the depth buffer value at the mouse click position
    float depth;
    glReadPixels(mouseX, screenHeight - mouseY - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    // Convert the mouse coordinates to OpenGL viewport coordinates (origin bottom left)
    glm::vec3 screenPos(mouseX, screenHeight - mouseY - 1, depth);

    // Define the viewport as per OpenGL's viewport
    glm::vec4 viewport = glm::vec4(0, 0, screenWidth, screenHeight);

    // Unproject the screen position back into world coordinates
    glm::vec3 worldPos = glm::unProject(screenPos, this->calculateViewMatrix(), this->calculatePerspectiveProjectionMatrix(screenWidth, screenHeight), viewport);

    // Return the calculated world position
    return worldPos;
}

// Setters=============================================================================================================
void Camera::setPosition(glm::vec3 currPos) {
    position = currPos;
}

// Changing Move Speed
void Camera::updateMoveSpeed(GLfloat newSpeed) {
    moveSpeed = newSpeed;
}

void Camera::setCameraParameters(float fieldOfView, float orthoScale, float near, float far) {
    FOV = fieldOfView;
    scale = orthoScale;
    nearClipping = near;
    farClipping = far;
}

void Camera::setCameraTarget(const glm::vec3& target) {
    front = target;
}

Camera::~Camera() {
}
