#include <iostream>
#include "Bones.h"

// Constructor
Bones::Bones() {
    localTransforms = glm::mat4(1.0f);
    VAO = 0;
    VBO = 0;
    IBO = 0;
    indexCount = 0;
}

void Bones::createBone(GLfloat * vertices, unsigned int * indices, unsigned int numOfVertices, unsigned int numOfIndices) {
    // Getting the number of indices
    indexCount = numOfIndices;

    // Creating and gettting the vertex ID of a VAO
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        // Creating the Index Buffer Object
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

            // Size of value we are passing in
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

        // Creating the buffer object inside VAO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            // STATIC DRAW - Not chaning the values in the array
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

            // Location = 0, the layout (location=0) id in the vertex shader
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 3, 0);
            glEnableVertexAttribArray(0);

        // Un-Binding Buffer Array
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Un-Binding Vertex Array
    glBindVertexArray(0);

    // Un-Binding IBO/EBO after VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Bones::renderBones() {
    // Fail conditions
    if(!VAO) {
        std::cout<<"VAO NOT DEFINED PROPERLY!";
        return;
    }

    if(!VBO) {
        std::cout<<"VBO NOT DEFINED PROPERLY!";
        return;
    }

    if(!IBO) {
        std::cout<<"IBO NOT DEFINED PROPERLY!";
        return;
    }

    // Binding the Vertex Array for Drawing
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        // Drawing the Elements (Since we are using Indexed arrays)
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Bones::cleanBones() {
    // Preventing overflow, garbage collection
    if(IBO) {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }

    if(VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    // Preventing overflow, garbage collection
    if(VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
}

// Destructor
Bones::~Bones() {
    cleanBones();
}
