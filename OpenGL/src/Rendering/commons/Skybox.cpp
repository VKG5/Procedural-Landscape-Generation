#include "Skybox.h"

// Get the full path of the current source file
const std::filesystem::path currentSourcePath = __FILE__;

// Extract the directory containing the source file
const std::filesystem::path currentSourceDir = currentSourcePath.parent_path().parent_path();

// Vertex Shader
// Uniform - Global to shader, not associated with a particular vertex
// Bind data to uniform to get location
std::string skyVertexShaderPath = returnPath(currentSourceDir, "Shaders/skybox.vert");
static const char* skyVertexShader = skyVertexShaderPath.c_str();

// Fragment Shader
std::string skyFragmentShaderPath = returnPath(currentSourceDir, "Shaders/skybox.frag");
static const char* skyFragmentShader = skyFragmentShaderPath.c_str();

Skybox::Skybox() {
}

Skybox::Skybox(std::vector<std::string> faceLocations) {
    // Shader setup
    skyShader = new Shader();
    skyShader->createFromFiles(skyVertexShader, skyFragmentShader);

    uniformProjection = skyShader->getProjectionLocation();
    uniformView = skyShader->getViewLocation();

    // Texture setup
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, bitDepth;

    for(size_t i = 0; i < 6; i++) {
        unsigned char *texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
        if(!texData) {
            printf("Failed to load: %s\n", faceLocations[i].c_str());
            return;
        }

        // Making texture with RGBA
        if(bitDepth == 4) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        }

        // Making texture with RGB
        else if(bitDepth == 3) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
        }

        else {
            printf("The texture has %i channels, unable to load!", bitDepth);
        }

        // Removing images from memory
        stbi_image_free(texData);
    }

    // For zooming out - Minify
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // For zooming in - Magnify
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Setting parameter values
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // For z coordinate
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Mesh Setup
    unsigned int skyboxIndices[] = {
        // Front
        0, 1, 2,
        2, 1, 3,

        // Right
        2, 3, 5,
        5, 3, 7,

        // Back
        5, 7, 4,
        4, 7, 6,

        // Left
        4, 6, 0,
        0, 6, 1,

        // Top
        4, 0, 5,
        5, 0, 2,

        // Bottom
        1, 6, 3,
        3, 6, 7
    };

    // Placing the images based off of camera values instead of UV
    float skyboxVertices[] = {
        // Back corners
        -1.0f, 1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,     0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,

        // Front corners
        -1.0f, 1.0f, 1.0f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,      0.0f, 0.0f,     0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
    };

    skyMesh = new Mesh();
    skyMesh->createMesh(skyboxVertices, skyboxIndices, 88, 36);
}

void Skybox::loadDefaultSkyboxes() {
    std::vector<std::string> skyboxFaces;

    // Clearing existing Skyboxes from list=
    defaultSkyboxes.clear();

    // Pushing the textures in a particular order
    // Right, Left
    // Up, Down
    // Front, Back
    // Clearing old data
    skyboxFaces.clear();
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Lake/right.jpg"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Lake/left.jpg"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Lake/top.jpg"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Lake/bottom.jpg"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Lake/front.jpg"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Lake/back.jpg"));

    // Parameters - Skybox faces, RGB/RGBA
    std::unique_ptr<Skybox> skyboxPtr = std::make_unique<Skybox>(skyboxFaces);
    // Now skyboxPtr is empty again
    defaultSkyboxes.push_back(std::move(skyboxPtr));

    // Clearing old data
    skyboxFaces.clear();
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Christmas/right.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Christmas/left.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Christmas/top.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Christmas/bottom.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Christmas/front.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Christmas/back.png"));

    // Parameters - Skybox faces, RGB/RGBA
    skyboxPtr = std::make_unique<Skybox>(skyboxFaces);
    defaultSkyboxes.push_back(std::move(skyboxPtr));

    // Clearing old data
    skyboxFaces.clear();
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Fireplace/right.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Fireplace/left.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Fireplace/top.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Fireplace/bottom.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Fireplace/front.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Fireplace/back.png"));

    // Parameters - Skybox faces, RGB/RGBA
    skyboxPtr = std::make_unique<Skybox>(skyboxFaces);
    defaultSkyboxes.push_back(std::move(skyboxPtr));

    // Clearing old data
    skyboxFaces.clear();
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Clouds/right.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Clouds/left.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Clouds/top.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Clouds/bottom.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Clouds/front.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Clouds/back.png"));

    // Parameters - Skybox faces, RGB/RGBA
    skyboxPtr = std::make_unique<Skybox>(skyboxFaces);
    defaultSkyboxes.push_back(std::move(skyboxPtr));

    // Clearing old data
    skyboxFaces.clear();
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Shanghai/right.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Shanghai/left.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Shanghai/top.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Shanghai/bottom.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Shanghai/front.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Shanghai/back.png"));

    // Parameters - Skybox faces, RGB/RGBA
    skyboxPtr = std::make_unique<Skybox>(skyboxFaces);
    defaultSkyboxes.push_back(std::move(skyboxPtr));

    // Clearing old data
    skyboxFaces.clear();
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Sky_Fire/right.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Sky_Fire/left.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Sky_Fire/top.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Sky_Fire/bottom.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Sky_Fire/front.png"));
    skyboxFaces.push_back(returnPath(currentSourceDir, "Textures/Skybox/Sky_Fire/back.png"));

    // Parameters - Skybox faces, RGB/RGBA
    skyboxPtr = std::make_unique<Skybox>(skyboxFaces);
    defaultSkyboxes.push_back(std::move(skyboxPtr));
}

void Skybox::drawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    // Removing the Transform values from mat4 by converting into mat3
    // Keeping only Translation and Rotation
    viewMatrix = glm::mat4(glm::mat3(viewMatrix));

    // Turning off GL Depth Mask so that the skybox doesn't overlap anything objects
    glDepthMask(GL_FALSE);

    // Main Skybox code
    skyShader->useShader();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    skyShader->validate();

    skyMesh->renderMesh();

    // Turning it back on, so that we can draw our objects in the scene properly (Without artifacts)
    // To prevent weird overlapping of vertices/edges/faces
    glDepthMask(GL_TRUE);
}

Skybox::~Skybox() {
}
