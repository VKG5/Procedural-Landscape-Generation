#ifndef MATHLFUNCSIB
#define MATHLFUNCSIB

// Library with Custom Math functions based off of GLM
// GLM Files - Math Library
#include <glm/glm.hpp>

// Print a glm::vec3 in Terminal
void printVec3(const glm::vec3& vector, const int& index);

// Print a glm::mat4 in Terminal
void printMat4(const glm::mat4& mat, const int& index);

// Invert an axis in a glm::vec3 (0, 1, 2)
glm::vec3 invertAxis(const glm::vec3& originalVec, const int& axis);

// Return the length/magnitude of a vector
float calcMagnitude(const glm::vec3& vecA);

// Calculate the vector from A to B
glm::vec3 calculateVector(const glm::vec3& pointA, const glm::vec3& pointB);

// Calculate angle betweem two vector relative to an origin
float calculateAngleBetween(const glm::vec3& vecA, const glm::vec3& vecB, const glm::vec3& origin);

// Calculate angle betweem two vector relative to each other
float calculateAngleBetween(const glm::vec3& vecA, const glm::vec3& vecB);

// Calculate axis of rotation relative to an origin
glm::vec3 calculateRotationAxis(const glm::vec3& vecA, const glm::vec3& vecB, const glm::vec3& origin);

// Calculate axis of rotation
glm::vec3 calculateRotationAxis(const glm::vec3& vecA, const glm::vec3& vecB);

// Return absolute difference between 2 vectors : A - B
float calcDifferenceMagnitude(const glm::vec3& vecA, const glm::vec3& vecB);

// Moving from Point A to Point B using Linear Interpolation
float lerp(float start, float end, float t);

// Ease in - Ease out function for good fall-off
float easeInOut(float t);

// Function that plays the animation
glm::vec3 moveFromAToB(int type, const glm::vec3& A, const glm::vec3& B, float t);

// Function to check if two float3 arrays are equal
bool areFloatArraysEqual(const float a[3], const float b[3], float epsilon = 0.001);

#endif
