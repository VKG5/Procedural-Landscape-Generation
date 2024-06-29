#pragma once

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>

class Mesh {
private:
    // IBO is optional but causes issues in some graphics cards
    GLuint VAO, VBO, IBO;

    // Indexcount, since we will be passing unkown number of indices
    GLsizei indexCount;

public:
    // Constructor
    Mesh();

    // Setup the initial mesh
    void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);

    // Render the mesh
    void renderMesh();

    // Clear Mesh from the Graphics Card
    void cleanMesh();

    // Destructor
    ~Mesh();
};
