#pragma once

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>
#include <glfw/glfw3.h>

// GLM Files - Math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    // Global up
    glm::vec3 worldUp;

    // Window coordinates
    GLdouble mouseX;
    GLdouble mouseY;

    GLfloat roll;
    GLfloat pitch;
    GLfloat yaw;

    GLfloat moveSpeed;
    GLfloat turnSpeed;

    // Projection
    GLfloat FOV;
    GLfloat scale;
    GLfloat nearClipping;
    GLfloat farClipping;

    void update();

public:
    // Constructor
    Camera();

    // Yaw - Front, Pitch - Right
    Camera( glm::vec3 initialPosition, glm::vec3 initialUp, GLfloat initialYaw,
            GLfloat initialPitch, GLfloat initialTurnSpeed, GLfloat initialMoveSpeed );

    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);

    // Getters=========================================================================================================
    // Getter for specular highlight data
    glm::vec3 getCameraPosition();
    glm::vec3 getCameraDirection();
    glm::vec3 getCameraDirectionMagnitude();

    // View matrices
    glm::mat4 calculateViewMatrix();

    // Overloads for Anaglyph rendering
    // For calculating the view matrix for Toed-in method
    glm::mat4 calculateViewMatrix(bool isLeftEye, const float& IOD, const float& convergenceDistance);

    // For calculating the projection matrix for Asymmetric Frustum Method
    glm::mat4 calculateAsymmetricFrustum(bool isLeftEye, const float& IOD, const float& convergenceDistance, const GLint& width, const GLint& height);

    // Projection Matrices
    glm::mat4 calculatePerspectiveProjectionMatrix(const GLint& width, const GLint& height);
    glm::mat4 calculateOrthographicProjectionMatrix();

    // Ray Casting
    // This will be used for object selection and other functionalities
    glm::vec3 getRayDirection(GLfloat mouseX, GLfloat mouseY, int screenWidth, int screenHeight);
    glm::vec3 getRayHitCoords(GLfloat mouseX, GLfloat mouseY, int screenWidth, int screenHeight);

    // Setters=========================================================================================================
    void updateMoveSpeed(GLfloat newSpeed);

    void setPosition(glm::vec3 currPos);

    void setCameraParameters(float fieldOfView, float orthoScale, float near, float far);

    void setCameraTarget(const glm::vec3& target);

    // Destructor
    ~Camera();
};
