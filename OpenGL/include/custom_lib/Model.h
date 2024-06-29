#pragma once

// General libraries
#include <iostream>
#include <vector>
#include <string>

// GLM Files - Math Library
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

// ASSIMP File Importer
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Custom libraries
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Utilities.h"

class Model {
private:
    std::vector<Mesh*> meshList;
    std::vector<Texture*> textureList;
    std::vector<unsigned int> meshToTex;

    // Local Transforms for the model
    // You can change them as needed
    glm::vec3 localPosition;
    glm::vec3 localRotation;
    glm::vec3 localScale;

    // Children pointer
    std::vector<Model*> children;

    // Parent pointer
    Model* parent;

    // Transform Matrix without transformations
    glm::mat4 initialTransform;

    // Transform Matrix with transformations
    glm::mat4 accumulateTransform;

    // Material for the object
    // TODO : Add support for multiple materials
    // std::vector<Material> materials;
    Material material;
    GLuint matUniformSpecularIntensity, matUniformShininess, matUniformMetalness;

    // To load children data
    void loadNode(aiNode *node, const aiScene *scene);
    void loadMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterials(const aiScene *scene);
    void loadMap(aiMaterial* material, aiTextureType textureType, int textureIndex);

public:
    // Constructor
    Model();

    void loadModel(const std::string& filePath);

    // Render a single model using normal method
    void renderModel();

    // Render hierarchical model
    void renderModel(const GLuint& uniformModel);

    // Clear data of the selected model
    void clearModel();

    // Getters=========================================================================================================
    glm::mat4 getInitialTransformMatrix() { return initialTransform; }
    glm::mat4 getAccumulateTransformMatrix() { return accumulateTransform; }
    glm::vec3 getPosition() { return localPosition; }
    Model* getParent() { return parent; }
    const std::vector<Model*>& getChildren() const { return children; }

    // Recursive function to calculate chain length
    GLuint calculateChainLength();

    // Function to clear the list of children
    void clearChildren();

    // Setters=========================================================================================================
    // Setting Local Transforms for the model, if you call the default constructor, it will initialize it at origin
    void setLocalTransforms(glm::vec3 translate = glm::vec3(0.0f), glm::vec3 rotate = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f));

    // Call this only ONCE at the beginning of the code IF you are CHANGING any local transforms
    void setInitialTransformMatrix();

    // Add a child to the list of children
    void attachChild(Model* childModel);

    // Add a SINGLE parent to the child
    void attachParent(Model* parentModel);

    // Call this after every draw call to reset the accumulate matrix to initial matrix
    void resetTransform();

    // Updating the final transform
    void updateTransform(glm::mat4& parentTransform = glm::mat4(1.0f));

    // Updating position in a recurive fashion, without updating transforms
    void updateHierarchicalLocation(glm::mat4& transform = glm::mat4(1.0f));

    // Setters for local transforms of the model - You can call them to permanently change the transformation of the model
    void setPosition(glm::vec3& pos = glm::vec3(0.0f));
    void setRotation(glm::vec3& rot);
    void setScale(glm::vec3& scale);

    // TRS
    void updateTranslation(glm::vec3& offset);
    void updateRotation(GLfloat angle, glm::vec3& axis, bool rads);
    void updateScale(glm::vec3& scale);

    // Materials
    void setMaterialUniforms(GLuint uniformSpecularIntensity, GLuint uniformShininess, GLuint uniformMetalness);
    void updateMaterialProperties(GLfloat specular, GLfloat shine, GLfloat metal);

    // Destructor
    ~Model();
};
