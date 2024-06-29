#ifndef COMMONVALS
#define COMMONVALS

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>

// GLM Files - Math Library
#include <glm/glm.hpp>

// Image Handling
#include <stb_image.h>

// Custom Libraries for passing parameters
#include "GUI.h"

// General libraries
#include <algorithm>

// Filesystem library for paths
#include <filesystem>

// Common Values
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;
const int MAX_SHADING_MODELS = 4;
const int DEFAULT_SKYBOXES = 6;

// Averaging Normals for Phong Shading
void calcAverageNormals(unsigned int* indices, unsigned int indexCount, GLfloat* vertices, unsigned int vertexCount, unsigned int vLength, unsigned int normalOffset);

// *Comment out for older code!*
// Setting correct Shading Mode name in the UI
void setShadingModeName(const GUI& GUI, const int& shadingModel, std::string& shadingMode);

// General Helpers=====================================================================================================
// Print a glm::vec3in GUI
void printVec3GUI(const glm::vec3& vector, const int& index);

// Print a glm::mat4 in GUI
void printMat4GUI(const glm::mat4& mat, const int& index);

// Removing backslashes from the path
std::string removeBackslash(const char* str);

// Compound path
std::string returnPath(const std::filesystem::path currentSourceDir, const std::string fileName);

#endif
