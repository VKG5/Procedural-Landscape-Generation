#include "Model.h"

// Get the full path of the current source file
const std::filesystem::path currentSourcePath = __FILE__;

// Extract the directory containing the source file
const std::filesystem::path currentSourceDir = currentSourcePath.parent_path();

Model::Model() {
    localPosition = glm::vec3(0.0f);
    localRotation = glm::vec3(0.0f, 1.0f, 0.0f);
    localScale = glm::vec3(1.0f);

    initialTransform = glm::mat4(1.0f);
    accumulateTransform = glm::mat4(1.0f);

    matUniformSpecularIntensity = 0;
    matUniformShininess = 0;
    matUniformMetalness = 0;

    // Attach default material to each object
    this->material = Material();
}

void Model::renderModel() {
    // Iterating over the mesh to render
    for(size_t i = 0; i < meshList.size(); i++) {
        unsigned int materialIndex = meshToTex[i];

        // Debugging
        // printf("Material Index : %i, Texture List Size : %i", materialIndex, textureList.size());

        // Iterate over all textures in textureList
        for (size_t index = 0; index < textureList.size(); index++) {
            // Checking if the texure exists
            if (textureList[index]) {
                // printf("Render Model : %i\n", index);
                textureList[index]->useTexture(index);
            }
        }

        // Rendering materials if present
        // for (auto& material : materials) {
        //     material.useMaterial();
        // }
        // Binding the values to uniform
        this->material.useMaterial(matUniformSpecularIntensity, matUniformShininess, matUniformMetalness);

        meshList[i]->renderMesh();
    }
}

void Model::renderModel(const GLuint& uniformModel) {
    // Binding the uniform model
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(accumulateTransform));

    // Iterating over the mesh to render
    for(size_t i = 0; i < meshList.size(); i++) {
        unsigned int materialIndex = meshToTex[i];

        // Debugging
        // printf("Material Index : %i, Texture List Size : %i", materialIndex, textureList.size());

        // Iterate over all textures in textureList
        for (size_t index = 0; index < textureList.size(); index++) {
            // Checking if the texure exists
            if (textureList[index]) {
                // printf("Render Model : %i\n", index);
                textureList[index]->useTexture(index);
            }
        }

        meshList[i]->renderMesh();
    }

    for(Model* child : children) {
        child->renderModel(uniformModel);
    }
}

void Model::loadNode(aiNode *node, const aiScene *scene) {
    // Iterating over meshes
    for(size_t i = 0; i < node->mNumMeshes; i++) {
        // Actual Mesh is stored in scene
        // Node contains the data - node->mMeshes[i]
        loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    // Iterating over children of meshes
    for(size_t i = 0; i < node->mNumChildren; i++) {
        loadNode(node->mChildren[i], scene);
    }
}

void Model::loadMesh(aiMesh * mesh, const aiScene * scene) {
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for(size_t i = 0; i < mesh->mNumVertices; i++) {
        // Recreating the array we made in main.cpp for Vertices, UVs and Normals
        vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

        // Checking if mesh has texture
        if(mesh->mTextureCoords[0]) {
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        }

        else{
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }

        // Adding Normals
        // Adding the reversed values because of the shader code
        vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });

        // Adding Tangents
        // Checking if the tagnets are availabe
        if(mesh->mTangents) {
            // Debugging
            // std::cout<<mesh->mTangents[i].x<<" "<<mesh->mTangents[i].y<<" "<<mesh->mTangents[i].z<<"\n";

            vertices.insert(vertices.end(), { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z });
        }

        else {
            vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f });
        }
    }

    // Adding indices
    // Going through each face as it has 3 vertices (indices)
    for(size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for(size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* newMesh = new Mesh();
    newMesh->createMesh( &vertices[0], &indices[0], vertices.size(), indices.size() );
    meshList.push_back(newMesh);

    // Storing index of all materials
    meshToTex.push_back(mesh->mMaterialIndex);
}

// Function to load the maps, since the functionality for loading each map is similar
void Model::loadMap(aiMaterial* material, aiTextureType textureType, int textureIndex) {
    aiString path;

    // If there is a diffuse map
    if(material->GetTexture(textureType, 0, &path) == AI_SUCCESS) {
        int idx = std::string(path.data).rfind("\\");
        std::string fileName = std::string(path.data).substr(idx + 1);

        std::string texRealtivePath = (currentSourceDir / "Textures/").string();
        std::string texRelativeFormatted = removeBackslash(texRealtivePath.c_str());
        std::string texPath = texRelativeFormatted + fileName;

        // Debugging
        // printf("Loading Texture from: %s\n", texPath.c_str());

        textureList[textureIndex] = new Texture(texPath.c_str());

        // Debugging
        // printf("Loading Texture: %s\n", fileName.c_str());

        if(!textureList[textureIndex]->loadTexture()) {
            printf("Failed to load texture at: %s\n", texPath.c_str());

            // SAFE_DELETE
            delete textureList[textureIndex];
            textureList[textureIndex] = nullptr;

            // Loading an empty Normal Map instead of the default white texture
            if(textureType == aiTextureType_HEIGHT) {
                // Debugging
                // printf("Adding default Normal Map\n");

                std::string texRealtivePath = (currentSourceDir / "Textures/Default/emptyNormal.png").string();
                std::string texRelativeFormatted = removeBackslash(texRealtivePath.c_str());

                textureList[textureIndex] = new Texture(texRelativeFormatted.c_str());
                textureList[textureIndex]->loadTexture();
            }
        }
    }
}

void Model::loadMaterials(const aiScene * scene) {
    // Initializing the vector
    textureList.resize(scene->mNumMaterials);

    // Debugging
    // printf("Number of Materials : %i\n", scene->mNumMaterials);

    // Checking if already loaded
    bool diffuse = true, specular = true, normal = true, ambientOcclusion = true;

    /*
    The code is not using AI_SCENE_FLAGS_INCOMPLETE, hence it will always have 1 material by default
    To bypass this, iterating over the materials by checking if numMaterials > 1 and using those as texture.
    */
    if(scene->mNumMaterials > 1) {
        // TODO : Remove the magic number 1
        aiMaterial* material = scene->mMaterials[1];

        // Counting the various textures in the material
        int textureCount = material->GetTextureCount(aiTextureType_DIFFUSE) +
                           material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) +
                           material->GetTextureCount(aiTextureType_SPECULAR) +
                           material->GetTextureCount(aiTextureType_HEIGHT);

        // Debugging
        // printf("Texture Count : %i\n", textureCount);

        // Resizing list to fit all the textures
        textureList.resize(textureCount);

        // Iterating over the various textures
        for(size_t i = 0; i < textureCount; i++) {
            textureList[i] = nullptr;

            // Debugging
            // printf("Texture Index : %zi\n", i);

            // Checking for null maps
            // DIFFUSE MAP - Texture Unit 0
            if(material->GetTextureCount(aiTextureType_DIFFUSE) && diffuse) {
                loadMap(material, aiTextureType_DIFFUSE, i);
                diffuse = false;
            }

            // SPECULAR MAP - Texture Unit 1
            else if(material->GetTextureCount(aiTextureType_SPECULAR) && specular) {
                loadMap(material, aiTextureType_SPECULAR, i);
                specular = false;
            }

            // NORMAL MAP - Texture Unit 2
            // aiTextureType_NORMAL doesn't load normal maps, aiTextureType_HEIGHT does - Wavefront OBJ format
            else if(material->GetTextureCount(aiTextureType_HEIGHT) && normal) {
                loadMap(material, aiTextureType_HEIGHT, i);
                normal = false;
            }

            // AMBIENT OCCLUSION - Texture Unit 3
            else if(material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) && ambientOcclusion) {
                loadMap(material, aiTextureType_AMBIENT_OCCLUSION, i);
                ambientOcclusion = false;
            }

            // If there was no texture, plugin default texture
            if(!textureList[i]) {
                // Debugging
                // printf("Adding default White Texture\n");

                std::string texRealtivePath = (currentSourceDir / "Textures/Default/white.jpg").string();
                std::string texRelativeFormatted = removeBackslash(texRealtivePath.c_str());

                textureList[i] = new Texture(texRelativeFormatted.c_str());
                textureList[i]->loadTexture();
            }
        }
    }

    // Default material/No material
    else if(scene->mNumMaterials == 1) {
        aiMaterial* material = scene->mMaterials[0];

        textureList[0] = nullptr;

        // Debugging
        // printf("Missing materials!\nAdding default texture\n");

        std::string texRealtivePath = (currentSourceDir / "Textures/Default/white.jpg").string();
        std::string texRelativeFormatted = removeBackslash(texRealtivePath.c_str());

        textureList[0] = new Texture(texRelativeFormatted.c_str());
        textureList[0]->loadTexture();
    }

    else {
        printf("Error loading materials!\n");
    }

    // Debugging
    // printf("Texture List Size : %i\n", textureList.size());
}

void Model::loadModel(const std::string& filePath) {
    Assimp::Importer importer;

    // aiProcess_Triangulate - Triangulate quads or mesh
    // aiProcess_FlipUVs - Flip UVs along Y axis (Because of the way our lighting is setup)
    // aiProcess_GenSmoothNormals - We are not handling flat shading
    // aiProcess_JoinIdenticalVertices - If overlapping vertices, will combine them
    const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace );

    // If there is no model
    if(!scene) {
        printf("Failed to load model (%s) : %s\n", filePath.c_str(), importer.GetErrorString());
        return;
    }

    loadNode(scene->mRootNode, scene);

    loadMaterials(scene);
}

void Model::clearModel() {
    for(size_t i = 0; i < meshList.size(); i++) {
        if(meshList[i]) {
            delete meshList[i];
            meshList[i] = nullptr;
        }
    }

    for(size_t i = 0; i < textureList.size(); i++) {
        if(textureList[i]) {
            delete textureList[i];
            textureList[i] = nullptr;
        }
    }
}

// Getters=============================================================================================================
GLuint Model::calculateChainLength() {
    // Include the current node in the chain length
    GLuint length = 1;

    // Recursively calculate the chain length for each child
    for (Model* child : children) {
        length += child->calculateChainLength();
    }

    return length;
}

void Model::clearChildren() {
    // Safe delete
    for(Model* child : children) {
        delete child;
    }

    children.clear();
}

// Setters=============================================================================================================
void Model::setLocalTransforms(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) {
    localPosition = translate;
    localRotation = rotate;
    localScale = scale;
}

void Model::setInitialTransformMatrix() {
    initialTransform = glm::mat4(1.0f);

    // Convert Euler angles from degrees to radians
    glm::vec3 rotationRadians = glm::radians(localRotation);

    // Create a quaternion from Euler angles
    glm::quat rotationQuat = glm::quat(rotationRadians);

    // TRS using quaternions
    initialTransform = glm::translate(initialTransform, localPosition);
    initialTransform *= glm::toMat4(rotationQuat);
    initialTransform = glm::scale(initialTransform, localScale);

    accumulateTransform = initialTransform;

    for(Model* child : children) {
        child->setInitialTransformMatrix();
    }
}

void Model::attachChild(Model * childModel) {
    if(childModel) {
        childModel->setInitialTransformMatrix();
        children.push_back(childModel);

        return;
    }

    printf("Failed to add child!\n");
}

void Model::attachParent(Model* parentModel) {
    if(parentModel) {
        parent = parentModel;

        return;
    }

    printf("Failed to attach to parent!\n");
}

void Model::updateTranslation(glm::vec3& offset) {
    accumulateTransform = glm::translate(accumulateTransform, offset);
}

void Model::setPosition(glm::vec3& pos) {
    localPosition = pos;
}

void Model::setRotation(glm::vec3& rot) {
    localRotation = rot;
}

void Model::setScale(glm::vec3& scale) {
    localScale = scale;
}

void Model::updateMaterialProperties(GLfloat specular, GLfloat shine, GLfloat metal) {
    material.setMaterialParamters(specular, shine, metal);
}

void Model::setMaterialUniforms(GLuint uniformSpecularIntensity, GLuint uniformShininess, GLuint uniformMetalness) {
    matUniformSpecularIntensity = uniformSpecularIntensity;
    matUniformShininess = uniformShininess;
    matUniformMetalness = uniformMetalness;
}

void Model::updateRotation(GLfloat angle, glm::vec3& axis, bool rads) {
    // Convert angle to radians if it's not already
    GLfloat angleInRadians = rads ? angle : glm::radians(angle);

    // Create a quaternion from the axis and angle
    glm::quat rotationQuat = glm::angleAxis(angleInRadians, glm::normalize(axis));

    // Convert quaternion to a rotation matrix
    glm::mat4 rotationMat = glm::toMat4(rotationQuat);

    // Apply the rotation matrix to the initialTransform
    accumulateTransform = accumulateTransform * rotationMat;
}

void Model::updateScale(glm::vec3& scale) {
    accumulateTransform = glm::scale(accumulateTransform, scale);
}

void Model::updateHierarchicalLocation(glm::mat4& transform) {
    glm::mat4 localTransform = transform * accumulateTransform;

    localPosition = glm::vec3(localTransform[3]);

    for(Model* child : children) {
        child->updateHierarchicalLocation(localTransform);
    }
}

void Model::updateTransform(glm::mat4& parentTransform) {
    accumulateTransform = parentTransform * accumulateTransform;

    for(Model* child : children) {
        child->updateTransform(accumulateTransform);
    }
}

void Model::resetTransform() {
    accumulateTransform = initialTransform;

    for(Model* child : children) {
        child->resetTransform();
    }
}

Model::~Model() {
}
