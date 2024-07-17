#pragma once

// ImGui libraries
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad.h>
#include <GLFW/glfw3.h>

#include <string>

class GUI {
private:
    ImGuiIO io;

    // Element variables
    // Wireframe
    bool wireframe = false;
    ImVec4 wireframeColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Texture/Object Color
    // Default color is Blender's default color
    bool shaded = true;
    ImVec4 objectColor = ImVec4(151.0f / 255.0f, 153.0f / 255.0f, 153.0f / 255.0f, 1.0f);

    // Background Color
    ImVec4 bgColor = ImVec4(61.0f / 255.0f, 61.0f / 255.0f, 61.0f / 255.0f, 1.0f);

    // Material Preview Mode
    bool materialPreview = false;
    bool specularPreview = false;
    bool normalPreview = false;

    // Light settings
    // Directional Light
    float directionalLightColor[3] = {1.0f, 1.0f, 1.0f};
    float directionalLightDirection[3] = {2.0f, 1.0f, -2.0f};
    float directionalLightAmbient = 0.350f;
    float directionalLightDiffuse = 0.500f;

    // Skybox
    int skyboxIndex = 1;

    // Camera
    bool cameraIsPerspective = true;
    bool cameraIsOrthographic = false;
    float cameraFOV = 45.0f;
    float cameraOrthoScale = 7.135f;
    float cameraNearClipping = 0.1f;
    float cameraFarClipping = 1500.0f;
    float cameraSpeed = 15.0f;
    float cameraPos[3] = {465.0f, 218.2f, 465.0f};
    bool isCameraRotate = false;
    float cameraRotateRadius = 10.0f;
    float cameraRotateSpeed = 1.0f;

    // Anaglyph
    bool isAnaglyph = false;
    bool isToedIn = false;
    bool isFlipAnaglyphChannelsToed = true;
    bool isFlipAnaglyphChannelsFrustum = false;
    bool isAsymmetricFrustum = false;
    float interOcularDistance = 0.065f;
    float convergeDistance = 2.0f;

    // Skybox
    bool isSkyBox = false;

    // Environment Mapping - Material Properties
    bool isEnvMapping = false;
    bool isReflection = true;
    bool isRefraction = false;
    bool drawSkybox = true;
    float ior = 1.52f;
    float f0 = 0.03f;
    float dispersion = 0.03f;
    float normalStrength = 0.0f;
    float specularSrength = 0.0f;

    // Material Settings
    float specular = 0.5f;
    float shininess = 256.0f;
    float metalness = 0.5f;

    // Terrain Settings
    char heightmapPath[256] = "D:/Programs/Python/Thesis/OpenGL/src/Rendering/Textures/Heightmaps/Mountains.png";
    bool refreshTerrain = false;
    float landscapeHeight = 1.0f;

public:
    // Constructor
    GUI();

    void InitializeImGuiIO();
    void initialize(GLFWwindow* mainWindow);
    void newFrame();
    void elements(const std::string& shadingMode);
    void render(const std::string& shadingMode);
    void warningMessage(const std::string& message);
    void errorMessage(const std::string& message);
    void shutdown();

    ImGuiIO getIO() const { return io; }


    // Getters for UI elements=========================================================================================
    // General UI======================================================================================================
    // Wireframe
    bool getIsWireframe() const { return wireframe; }
    ImVec4 getWireframeColor() const { return wireframeColor; }

    // Textures/Object Color
    bool getIsShaded() const { return shaded; }
    ImVec4 getObjectColor() const { return objectColor; }

    // Background Color
    ImVec4 getBackgroundColor() const { return bgColor; }

    // Material Preview Mode
    bool getMaterialPreview() const { return materialPreview; }

    // Terrain Properties
    const char* getHeightmapPath() const { return heightmapPath; }
    bool getIsTerrainRefreshRequired() const { return refreshTerrain; }
    float getLandscapeHeight() const { return landscapeHeight; }

    // Camera
    int getSkyboxIndex() const { return skyboxIndex; }
    bool getCameraIsPerspective() const { return cameraIsPerspective; }
    bool getCameraIsOrthographic() const { return cameraIsOrthographic; }
    float getCameraFOV() const { return cameraFOV; }
    float getCameraScale() const { return cameraOrthoScale; }
    float getCameraNearClipping() const { return cameraNearClipping; }
    float getCameraFarClipping() const { return cameraFarClipping; }
    float getCameraSpeed() const { return cameraSpeed; }
    const float* getCameraPosition() const { return cameraPos; }
    bool getIsCameraRotating() const { return isCameraRotate; }
    float getCameraRotationRadius() const { return cameraRotateRadius; }
    float getCameraRotationSpeed() const { return cameraRotateSpeed; }

    // Directional Light Parameters
    const float* getDirectionalLightColor() const { return directionalLightColor; }
    const float* getDirectionalLightDirection() const { return directionalLightDirection; }
    float getDirectionalLightAmbient() const { return directionalLightAmbient; }
    float getDirectionalLightDiffuse() const { return directionalLightDiffuse; }

    // Skybox Parameters
    bool getIsSkyBox() const { return isSkyBox; }
    bool getDrawSkyBox() const { return drawSkybox; }

    // Materials
    float getSpecular() const { return specular; }
    float getShininess() const  { return shininess; }
    float getMetalness() const { return metalness; }


    // Project specific components=====================================================================================
    // Setters=========================================================================================================
    // Camera
    void setCameraIsPerspective(bool flag);
    void setCameraIsOrthographic(bool flag);
    void setCameraFOV(float fieldOfView);
    void setCameraScale(float scale);
    void setCameraClipping(float near, float far);
    void setCameraPosition(float x, float y, float z);

    // Terrain
    void setIsTerrainRefreshRequired(bool isRequired);

    // Destructor
    ~GUI();
};
