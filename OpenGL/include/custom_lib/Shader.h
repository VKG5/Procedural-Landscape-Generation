#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

// Always include GLFW after GLAD - Core Libraries
#include <glad.h>

#include "Utilities.h"

// To directly pass various lights into the shader
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader {
private:
    int pointLightCount;
    int spotLightCount;

    GLuint  shaderID, uniformProjection, uniformModel, uniformView,
            uniformEyePosition,
            uniformSpecularIntensity, uniformShininess, uniformMetalness;

    // Creating instance of struct - uniformDirectionalLight
    struct {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformDirection;
    } uniformDirectionalLight;

    GLuint uniformPointLightCount;

    struct {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight[MAX_POINT_LIGHTS];

    GLuint uniformSpotLightCount;

    struct {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;

        GLuint uniformDirection;
        GLuint uniformEdge;
    } uniformSpotLight[MAX_SPOT_LIGHTS];

    // For switching between different shading models and UI Elements
    GLuint uniformshadingModel,
           uniformIsShaded, uniformIsWireframe,
           uniformObjectColor, uniformWireframeColor,
           uniformMaterialPreview, uniformSpecularPreview, uniformNormalPreview,
           uniformDiffuseTexture, uniformAmbientOcclusionTexture, uniformSpecularTexture, uniformNormalTexture, uniformNoiseTexture,
           uniformEnvMapping, uniformSkybox, uniformBackgroundColor,
           uniformIsReflection, uniformIsRefraction, uniformIOR, uniformFresnelReflectance, uniformDispersion,
           uniformNormalStrength, uniformSpecularStrength;

    void compileShader(const char* vertexCode, const char* fragmentCode);

    void addShader(GLuint program, const char* shaderCode, GLenum shaderType);

    void compileProgram();

public:
    // Constructor
    Shader();

    void createFromString(const char* vertexCode, const char* fragmentCode);
    void createFromFiles(const char* vertexLocation, const char* fragmentLocation);

    void validate();

    std::string readFile(const char* filePath);

    GLuint getProjectionLocation();
    GLuint getModelLocation();
    GLuint getViewLocation();
    GLuint getAmbientIntensityLocation();
    GLuint getAmbientColourLocation();
    GLuint getDiffuseIntensityLocation();
    GLuint getDiffuseDirectionLocation();
    GLuint getSpecularIntensityLocation();
    GLuint getShininessLocation();
    GLuint getMetalnessLocation();
    GLuint getEyePositionLocation();
    GLuint getShadingModelLocation();

    // Textures
    GLuint getMainTextureLocation();
    GLuint getSpecularTextureLocation();
    GLuint getAmbientOcclusionTextureLocation();
    GLuint getNormalTextureLocation();
    GLuint getNoiseTextureLocation();

    // UI Elements
    // Object Properties
    GLuint getIsShadedLocation();
    GLuint getIsWireframeLocation();
    GLuint getObjectColorLocation();
    GLuint getWireframeColourLocation();
    GLuint getEnvMappingLocation();
    GLuint getSkyboxLocation();
    GLuint getBackgroundColourLocation();

    // Material Preview
    GLuint getMaterialPreviewLocation();
    GLuint getSpecularPreviewLocation();
    GLuint getNormalPreviewLocation();

    // Material properties
    GLuint getIsReflectionLocation();
    GLuint getIsRefractionLocation();
    GLuint getIORLocation();
    GLuint getFresnelReflectance();
    GLuint getDispersionLocation();
    GLuint getNormalStrengthLocation();
    GLuint getSpecularStrengthLocation();

    // Shader ID
    GLuint getShaderIDLocation();

    void setDirectionalLight(DirectionalLight* dLight);
    void setPointLight(PointLight* pLight, unsigned int lightCount);
    void setSpotLight(SpotLight* sLight, unsigned int lightCount);
    void setTexture(GLuint uniformTexLocation, GLuint textureUnit);

    void useShader();
    void cleanShader();

    // Destructor
    ~Shader();
};
