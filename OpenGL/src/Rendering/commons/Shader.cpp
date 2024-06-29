#include "Shader.h"

// Constructor
Shader::Shader() {
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
    uniformView = 0;

    pointLightCount = 0;
    spotLightCount = 0;

    uniformDiffuseTexture = 0;
    uniformAmbientOcclusionTexture = 0;
    uniformSpecularTexture = 0;
    uniformNormalTexture = 0;
    uniformNoiseTexture = -1;
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode) {
    compileShader(vertexCode, fragmentCode);
}

void Shader::validate() {
    // Creating this for logging/debugging
    GLint result = 0;
    GLchar eLog[1024] = {0};

    // Checking if valid in current context
    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if(!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout<<"Error validating program: "<<eLog<<"\n";
        return;
    }
}

void Shader::compileProgram() {
    // Creating this for logging/debugging
    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if(!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout<<"Error linking program: "<<eLog<<"\n";
        return;
    }

    // Name in the vertex shader
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformModel = glGetUniformLocation(shaderID, "model");

    // For switching shading models
    uniformshadingModel = glGetUniformLocation(shaderID, "shadingModel");

    // UI Elements
    // Material Preview
    uniformMaterialPreview = glGetUniformLocation(shaderID, "materialPreview");
    uniformSpecularPreview = glGetUniformLocation(shaderID, "specularPreview");
    uniformNormalPreview = glGetUniformLocation(shaderID, "normalPreview");

    // Object Properties
    uniformIsShaded = glGetUniformLocation(shaderID, "isShaded");
    uniformIsWireframe = glGetUniformLocation(shaderID, "isWireframe");
    uniformObjectColor = glGetUniformLocation(shaderID, "objectColor");
    uniformWireframeColor = glGetUniformLocation(shaderID, "wireframeColor");

    // Tramission properties
    uniformIsReflection = glGetUniformLocation(shaderID, "reflection");
    uniformIsRefraction = glGetUniformLocation(shaderID, "refraction");
    uniformIOR = glGetUniformLocation(shaderID, "ior");
    uniformDispersion = glGetUniformLocation(shaderID, "dispersion");
    uniformFresnelReflectance = glGetUniformLocation(shaderID, "reflectance");
    uniformEnvMapping = glGetUniformLocation(shaderID, "envMapping");
    uniformSkybox = glGetUniformLocation(shaderID, "skybox");
    uniformBackgroundColor = glGetUniformLocation(shaderID, "backgroundColor");

    // Getting the locations for the particular "directionalLight" light
    // Ambient Light
    uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");

    // Diffuse Light
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");

    // Textures
    uniformDiffuseTexture = glGetUniformLocation(shaderID, "diffuseMap");
    uniformAmbientOcclusionTexture = glGetUniformLocation(shaderID, "ambientOcclusionMap");
    uniformSpecularTexture = glGetUniformLocation(shaderID, "specularMap");
    uniformNormalTexture = glGetUniformLocation(shaderID, "normalMap");
    uniformNoiseTexture = glGetUniformLocation(shaderID, "noiseTexture");

    // Material Properties
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformMetalness = glGetUniformLocation(shaderID, "material.metalness");
    uniformNormalStrength = glGetUniformLocation(shaderID, "normalStrength");
    uniformSpecularStrength = glGetUniformLocation(shaderID, "specularStrength");

    // Specular Highlights
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

    // Point Light
    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

    for(size_t i = 0; i<MAX_POINT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLight[%zd].base.colour", i);
        uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%zd].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%zd].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%zd].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%zd].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%zd].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%zd].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }

    // Spot Light
    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

    for(size_t i = 0; i<MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "spotLight[%zd].base.base.colour", i);
        uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%zd].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%zd].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%zd].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%zd].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%zd].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%zd].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%zd].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%zd].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {
    // Creating the program to run the shaders on the GPU
    shaderID = glCreateProgram();

    if(!shaderID) {
        std::cout<<"Error creating shader program";
        return;
    }

    addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    compileProgram();
}

void Shader::addShader(GLuint program, const char* shaderCode, GLenum shaderType) {
    // Creating an empty shader
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    // Creating this for logging/debugging
    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if(!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cout<<"Error compiling the "<<shaderType<<" shader: "<<eLog<<"\n";
        return;
    }

    glAttachShader(program, theShader);
}

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation) {
    std::string vertexString = readFile(vertexLocation);
    std::string fragmentString = readFile(fragmentLocation);
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char* filePath) {
    std::string content;

    // Source of the files
    std::fstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        printf("Failed to read '%s'! File doesn't exist.\n", filePath);
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

// Getter for Uniform Projection
GLuint Shader::getProjectionLocation() {
    return uniformProjection;
}

// Getter for Uniform Model
GLuint Shader::getModelLocation() {
    return uniformModel;
}

// Getter for Uniform View
GLuint Shader::getViewLocation() {
    return uniformView;
}

// Getter for Ambient Colour
GLuint Shader::getAmbientColourLocation() {
    return uniformDirectionalLight.uniformColour;
}

// Getter for Ambient Intensity
GLuint Shader::getAmbientIntensityLocation() {
    return uniformDirectionalLight.uniformAmbientIntensity;
}

// Getter for Light Direction
GLuint Shader::getDiffuseDirectionLocation() {
    return uniformDirectionalLight.uniformDirection;
}

// Getter for Diffuse Intensity
GLuint Shader::getDiffuseIntensityLocation() {
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

// Getter for Specular Intensity
GLuint Shader::getSpecularIntensityLocation() {
    return uniformSpecularIntensity;
}

// Getter for Shininess
GLuint Shader::getShininessLocation() {
    return uniformShininess;
}

// Getter for Roughness
GLuint Shader::getMetalnessLocation() {
    return uniformMetalness;
}

// Getter for Eye Position
GLuint Shader::getEyePositionLocation() {
    return uniformEyePosition;
}

// Getting Use Blinn
GLuint Shader::getShadingModelLocation() {
    return uniformshadingModel;
}

// UI Element Uniform Locations========================================================================================
GLuint Shader::getIsShadedLocation() {
    return uniformIsShaded;
}

GLuint Shader::getIsWireframeLocation() {
    return uniformIsWireframe;
}

GLuint Shader::getObjectColorLocation() {
    return uniformObjectColor;
}

GLuint Shader::getWireframeColourLocation() {
    return uniformWireframeColor;
}

// Material Preview
GLuint Shader::getMaterialPreviewLocation() {
    return uniformMaterialPreview;
}

GLuint Shader::getSpecularPreviewLocation() {
    return uniformSpecularPreview;
}

GLuint Shader::getNormalPreviewLocation() {
    return uniformNormalPreview;
}

// Textures
GLuint Shader::getMainTextureLocation() {
    return uniformDiffuseTexture;
}

GLuint Shader::getSpecularTextureLocation() {
    return uniformSpecularTexture;
}

GLuint Shader::getAmbientOcclusionTextureLocation() {
    return uniformAmbientOcclusionTexture;
}

GLuint Shader::getNormalTextureLocation() {
    return uniformNormalTexture;
}

GLuint Shader::getNoiseTextureLocation() {
    return uniformNoiseTexture;
}

// Environment Mapping
GLuint Shader::getEnvMappingLocation() {
    return uniformEnvMapping;
}

GLuint Shader::getSkyboxLocation() {
    return uniformSkybox;
}

GLuint Shader::getBackgroundColourLocation() {
    return uniformBackgroundColor;
}

// Material Paramters
GLuint Shader::getIsReflectionLocation() {
    return uniformIsReflection;
}

GLuint Shader::getIsRefractionLocation() {
    return uniformIsRefraction;
}

GLuint Shader::getIORLocation() {
    return uniformIOR;
}

GLuint Shader::getFresnelReflectance() {
    return uniformFresnelReflectance;
}

GLuint Shader::getDispersionLocation() {
    return uniformDispersion;
}

GLuint Shader::getNormalStrengthLocation() {
    return uniformNormalStrength;
}

GLuint Shader::getSpecularStrengthLocation() {
    return uniformSpecularStrength;
}

// Getting the shader ID===============================================================================================
GLuint Shader::getShaderIDLocation() {
    return shaderID;
}

void Shader::setTexture(GLuint uniformTexLocation, GLuint textureUnit) {
    glUniform1i(uniformTexLocation, textureUnit);
}

void Shader::setDirectionalLight(DirectionalLight * dLight) {
    dLight->useLight( uniformDirectionalLight.uniformAmbientIntensity,
                      uniformDirectionalLight.uniformColour,
                      uniformDirectionalLight.uniformDiffuseIntensity,
                      uniformDirectionalLight.uniformDirection );
}

void Shader::setPointLight(PointLight * pLight, unsigned int lightCount) {
    // Limiting number of point lights due to shader structure
    if(lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);

    for(size_t i = 0; i < lightCount; i++) {
        pLight[i].useLight( uniformPointLight[i].uniformAmbientIntensity,
                            uniformPointLight[i].uniformColour,
                            uniformPointLight[i].uniformDiffuseIntensity,
                            uniformPointLight[i].uniformPosition,
                            uniformPointLight[i].uniformConstant,
                            uniformPointLight[i].uniformLinear,
                            uniformPointLight[i].uniformExponent );
    }
}

void Shader::setSpotLight(SpotLight * sLight, unsigned int lightCount) {
    // Limiting number of point lights due to shader structure
    if(lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, lightCount);

    for(size_t i = 0; i < lightCount; i++) {
        sLight[i].useLight( uniformSpotLight[i].uniformAmbientIntensity,
                            uniformSpotLight[i].uniformColour,
                            uniformSpotLight[i].uniformDiffuseIntensity,
                            uniformSpotLight[i].uniformPosition,
                            uniformSpotLight[i].uniformDirection,
                            uniformSpotLight[i].uniformConstant,
                            uniformSpotLight[i].uniformLinear,
                            uniformSpotLight[i].uniformExponent,
                            uniformSpotLight[i].uniformEdge );
    }
}

void Shader::useShader() {
    if(shaderID) {
        glUseProgram(shaderID);
    }

    else {
        std::cout<<"Error in attaching shaderID "<<shaderID<<" to program";
    }
}

void Shader::cleanShader() {
    if(shaderID) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

// Destructor
Shader::~Shader() {
    cleanShader();
}
