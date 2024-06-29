#pragma once

#include <vector>

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Bones {
    // These account for rotation, position and scale
    glm::mat4 localTransforms;
    std::vector<Bones> children;

    GLuint VAO, VBO, IBO;
    GLsizei indexCount;

public:
    // Constructor
    Bones();

    // Setup the initial bone
    void createBone(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);

    // Render the bone
    void renderBones();

    // Clearing memory and references from Graphics Card
    void cleanBones();

    // Destructor
    ~Bones();
};
