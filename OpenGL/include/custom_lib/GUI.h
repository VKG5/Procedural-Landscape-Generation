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
    float directionalLightDirection[3] = {2.0f, -1.0f, -2.0f};
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
    float cameraFarClipping = 100.0f;
    float cameraSpeed = 5.0f;
    float cameraPos[3] = {0.0f, 1.75f, 4.0f};
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

    // Point Lights
    bool isPointLights = false;

    // Spot Lights
    bool isSpotLights = false;

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

    // Material settings
    float specular = 0.5f;
    float shininess = 256.0f;
    float metalness = 0.5f;

    // IK Controls
    bool isIK = false;
    bool isTwoDIK = true;
    bool isAnimate = false;
    int chainLength = 2;
    int maxIterations = 1024;
    float tolerance = 1.0f;
    float epsilon = 0.01f;
    float targetLocation[3] = {0.0f, 3.5f, 0.0f};
    float targetEndLocation[3] = {1.0f, 0.75f, 2.5f};
    float targetSpeed = 0.005f;
    float animationRate = 0.005f;
    int interpolationType = 1;

    // Procedural Content Generation
    bool isPCG = false;
    GLuint seed = 165316;
    float floorOffset[3] = {0.0f, 0.0f, 0.0f};
    float floorScale[3] = {2.5f, 1.0f, 2.5f};
    GLuint gridSize = 20;
    GLuint pointSize = 2;
    GLuint numPoints = 20;
    bool update = false;

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
    bool getSpecularPreview() const { return specularPreview; }
    bool getNormalPreview() const { return normalPreview; }

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

    // Anaglyph
    bool getIsAnaglyph() const { return isAnaglyph; }
    bool getIsToedInRendering() const { return isToedIn; }
    bool getIsAsymmetricFrustumRendering() const { return isAsymmetricFrustum; }
    bool getIsAnaglyphChannelsFlippedToed() const { return isFlipAnaglyphChannelsToed; }
    bool getIsAnaglyphChannelsFlippedFrustum() const  { return isFlipAnaglyphChannelsFrustum; }
    float getInterOcularDistance() const { return interOcularDistance; }
    float getCovergenceDistance() const { return convergeDistance; }

    // Directional Light Parameters
    const float* getDirectionalLightColor() const { return directionalLightColor; }
    const float* getDirectionalLightDirection() const { return directionalLightDirection; }
    float getDirectionalLightAmbient() const { return directionalLightAmbient; }
    float getDirectionalLightDiffuse() const { return directionalLightDiffuse; }

    // Point Light Parameters
    bool getIsPointLights() const { return isPointLights; }

    // Spot Light Parameters
    bool getIsSpotLights() const { return isSpotLights; }

    // Skybox Parameters
    bool getIsSkyBox() const { return isSkyBox; }
    bool getDrawSkyBox() const { return drawSkybox; }

    // Environment Mapping Parameters - Material Properties
    bool getIsEnvMapping() const { return isEnvMapping; }
    bool getIsReflection() const { return isReflection; }
    bool getIsRefraction() const { return isRefraction; }
    float getIOR() const { return ior; }
    float getFresnelReflectance() const { return f0; }
    float getDispersion() const { return dispersion; }
    float getNormalStrength() const { return normalStrength; }
    float getSpecularStrength() const { return specularSrength; }

    // Materials
    float getSpecular() const { return specular; }
    float getShininess() const  { return shininess; }
    float getMetalness() const { return metalness; }

    // PCG
    bool getIsPCG() const { return isPCG; }
    GLuint getSeed() const { return seed; }
    const float* getFloorOffset() const { return floorOffset; }
    const float* getFloorScale() const { return floorScale; }
    GLuint getGridSize() const { return gridSize; }
    GLuint getPointSize() const { return pointSize; }
    GLuint getNumPoints() const { return numPoints; }
    bool getUpdate() const { return update; }


    // Project specific components=====================================================================================
    // IK
    bool getIsIK() const { return isIK; }
    bool getIsTwoDIK() const { return isTwoDIK; }
    bool getIsAnimate() const { return isAnimate; }
    int getMaxIterations() const { return maxIterations; }
    float getTolerance() const { return tolerance; }
    float getEpsilon() const { return epsilon; }
    int getChainLength() const { return chainLength; }
    const float* getTargetLocation() const { return targetLocation; }
    const float* getTargetEndLocation() const { return targetEndLocation; }
    float getTargetSpeed() const { return targetSpeed; }
    float getAnimationRate() const { return animationRate; }
    int getInterpolationType() const { return interpolationType; }


    // Setters=========================================================================================================
    // Camera
    void setCameraIsPerspective(bool flag);
    void setCameraIsOrthographic(bool flag);
    void setCameraFOV(float fieldOfView);
    void setCameraScale(float scale);
    void setCameraClipping(float near, float far);

    // Anaglyph
    void setIsAnaglyph(bool flag);
    void setIsToedInRendering(bool flag);
    void setIsAsymmetricFrustumRendering(bool flag);
    void setIsAnaglyphChannelsFlippedToed(bool flag);
    void setIsAnaglyphChannelsFlippedFrustum(bool flag);

    // IK
    void setTargetLocation(float x, float y, float z);
    void setCameraPosition(float x, float y, float z);
    void setIsAnimate(bool flag);
    void setIsIK(bool flag);

    // PCG - Setter to reset the button press value
    void setUpdate(bool updateValue);

    // Destructor
    ~GUI();
};
