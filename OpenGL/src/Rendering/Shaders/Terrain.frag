#version 460 core


// Inputs from Geometry Shader=========================================================================================
in vec4 geomCol;
in vec2 geomTexCoord;
in vec3 geomNormal;
in vec3 geomFragPos;


// Output color========================================================================================================
out vec4 outputColor;


// Constants===========================================================================================================
// Should be same as Utilities.h header file
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;


// Light Structs=======================================================================================================
struct Light {
    vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct PointLight {
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight {
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material {
    float specularIntensity;
    float shininess;
    float metalness;
};


// Uniforms from C++===================================================================================================
uniform vec4 objectColor;
uniform vec4 wireframeColor;

// Rendering Mode : Wireframe or Shaded
uniform bool isWireframe;
uniform bool isShaded;


// Instantiating Lights================================================================================================
uniform DirectionalLight directionalLight;
// uniform PointLight pointLight[MAX_POINT_LIGHTS];
// uniform SpotLight spotLight[MAX_SPOT_LIGHTS];


// Instantiating Textures==============================================================================================
// Bound at Texture Unit 0
uniform sampler2D diffuseMap;

// Texture Unit 1
// uniform sampler2D specularMap;

// Texture Unit 2
// uniform sampler2D normalMap;

// Calculations========================================================================================================
vec4 calcLightByDirection(Light light, vec3 direction) {
    // Ambient Light
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    // Diffuse Light
    // Getting the cosine of angle between two vectors
    float diffuseFactor = max(dot(normalize(geomNormal), normalize(direction)), 0.0);
    vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity, 1.0);

    // Specular Light
    vec4 specularColour = vec4(0, 0, 0, 0);

    return (ambientColour + diffuseColour);
}

vec4 calcDirectionalLight() {
    return calcLightByDirection(directionalLight.base, directionalLight.direction);
}

// Main Function=======================================================================================================
void main() {
    vec4 finalColour = calcDirectionalLight();

    outputColor = texture(diffuseMap, geomTexCoord) * finalColour;
}
