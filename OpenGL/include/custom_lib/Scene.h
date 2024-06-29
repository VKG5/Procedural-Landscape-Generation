#pragma once

// Including all necessary headers
// ImGUI
#include "GUI.h"

// Basic C++ Libraries for various operations
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
// For smart and shared pointers - To better manage memory leaks
#include <memory>

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>
#include <GLFW/glfw3.h>

// GLM Files - Math Library
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Custom Libraries
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Utilities.h"
#include "MathFuncs.h"
#include "Material.h"

// Custom Models
#include "Model.h"

// Skybox
#include "Skybox.h"

// Procedural Content Generation
#include "randomDistribute.h"

/*
This class encapsulates all the elements in the viewport or scene. That includes the GUI layout, objects in the scene, Skyboxes,
materials, cameras and anything that should be specific to the scene. Please modify as per required, making sure that the main
functionality is not altered. It is suggested to inherit this class and add further content in the child class!
*/
class Scene {
private:
    // Defining all necessary variables
    // PCG variables
    GLuint seed;
    int gridSize = 20, pointSize = 2, numPoints = 20;
    std::vector<std::pair<int, int>> randomPoints;
    std::vector<glm::vec3> randomScales;
    std::vector<int> randomHeights;

    // Setting the variables
    GLuint uniformProjection, uniformModel, uniformView, uniformEyePosition;
    GLuint uniformSpecularIntensity, uniformShininess, uniformMetalness;
    GLuint uniformshadingModel;
    GLuint uniformIsShaded, uniformIsWireframe, uniformObjectColor, uniformWireframeColor;
    GLuint uniformMaterialPreview, uniformSpecularPreview, uniformNormalPreview;
    GLuint uniformDiffuseTexture, uniformAmbientOcclusionTexture, uniformSpecularTexture, uniformNormalTexture;
    GLuint uniformNoiseTexture;
    GLuint uniformEnvMapping, uniformSkybox, uniformBackgroundColor;
    GLuint uniformIsReflection, uniformIsRefraction, uniformIOR, uniformFresnelReflectance, uniformDispersion;
    GLuint uniformNormalStrength, uniformSpecularStrength;

    // Our main window
    Window& mainWindow;
    GUI mainGUI;
    std::string shadingMode = "Phong Illumination";

    // Creating a vector of the meshes and shaders
    std::vector<Mesh*> meshList;
    std::vector<Shader*> shaderList;

    // Camera
    Camera camera;

    // Switching between GLFW and ImGUI
    bool cursorDisabled = true;

    // Textures
    // TODO : Vectors don't work with these textures? Weird - SOLUTION : USE POINTERS FOR TEXTURE VECTORS
    Texture whiteTexture;
    Texture brickTexture;

    // Noise Texture for random maps
    // Texture noiseTexture;

    // Lights - 1 Directional, Multiple Point, Multiple Spot
    DirectionalLight mainLight;
    PointLight pointLights[MAX_POINT_LIGHTS];
    SpotLight spotLights[MAX_SPOT_LIGHTS];

    // Point Lights
    unsigned int pointLightCount = 0;

    // Spot Lights
    unsigned int spotLightCount = 0;

    // Skybox
    std::unique_ptr<Skybox> mainSkybox;
    std::vector<std::unique_ptr<Skybox>> skyboxList;

    // Materials
    Material shinyMat;
    Material roughMat;
    Material extraRoughMat;
    Material extraShinyMat;

    // We have 4 shading models that can be iterated through
    // 1. Phong Illumination
    // 2. Blinn-Phong Illumination
    // 3. Gooch Shading
    // 4. Minnaert Shading
    int shadingModel = 0;

    // We have 3 rendering modes at the moment
    // 1. Normal
    // 2. Toed-in Asymmetric
    // 3. Asymmetric Frustum
    int renderingMode = 0;

    // Models
    Model* building0;
    Model* building1;
    Model* monkey;
    Model* cube;

    // Camera Rotation
    float radius = 10.0f;
    float cameraAngle = 0.0f;

    // Local deltatime
    float deltaTime = 0.0f;

    // Initial Projection Matrix
    glm::mat4 projection;

public:
    // Constructor
    Scene(Window& window, GLuint seed = 69420);

    // General Elements================================================================================================
    // Add a simple plane/floor
    void createPlane(const float floorSize = 5.0f, const float floorUV = 2.5f);

    // Creating and adding shaders to the scene
    void createShaders(const std::filesystem::path& currentSourceDir);

    // Add lights and skyboxes to your scene!
    void createLights();

    // Add textures to be used
    void createTextures();

    // Add materials to be used
    // TODO : REFACTOR INTO MODEL CLASS
    void createMaterials();

    // Load objects into the scene using Model class
    void loadObjects();

    // *Function to call the previous 4 functions for ease of use!
    // This sets up the scene - Loads Lights, Textures, Materials and Objects
    void setupScene(const std::filesystem::path& currentSourceDir);

    // Uniforms========================================================================================================
    // Get uniforms from a particular shader in the Shader List
    void getUniformsFromShader(Shader* shader);

    // Set the uniforms for a particular shader in the Shader List
    void setUniformsForShader(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, Shader* shader);

    // Render Passes===================================================================================================
    // These include the elements that will be render in the scene, can define multiple ones
    // Render a default PCG City/Whatever definition you have for the function
    void renderPCGElements();

    // Render a custom scene with custom objects, lighting and different shading modes
    void renderScene();

    // Anaglyph Rendering
    void calculateAnaglyph(glm::mat4& projectionMatrix, glm::mat4& viewMatrix);

    // General Elements of a render pass===============================================================================
    // This includes setting up the Skyboxes and various components initially
    void generalElements(glm::mat4& projectionMatrix, glm::mat4& viewMatrix);

    // Actual render pass
    void renderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

    // Function to update everything in each main loop cycle
    void update(float time);

    // Destructor
    ~Scene();
};
