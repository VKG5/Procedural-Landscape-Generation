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
uniform bool isNormal;

// Materials
uniform Material material;

// Eye Position
uniform vec3 eyePosition;


// Instantiating Lights================================================================================================
uniform DirectionalLight directionalLight;


// Instantiating Textures==============================================================================================
// Bound at Texture Unit 0
uniform sampler2D diffuseMap;

// Texture Unit 1
// uniform sampler2D specularMap;

// Texture Unit 2
// uniform sampler2D normalMap;


// Global Variables
vec3 normal;


// Calculations========================================================================================================
vec4 calcLightByDirection(Light light, vec3 direction) {
    // Ambient Light
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    // Diffuse Light
    // Getting the cosine of angle between two vectors
    float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0);
    vec4 diffuseColour = vec4(light.colour, 1.0)  * light.diffuseIntensity * diffuseFactor;

    // Specular Light
    vec4 specularColour = vec4(0, 0, 0, 0);

    if(diffuseFactor > 0.0) {
        float specularFactor = 0.0;

        // Calculating Halfway Direction for Phong-Blinn Model
        vec3 fragToLight = normalize(direction);
        vec3 fragToEye = normalize(eyePosition - geomFragPos);
        vec3 halfwayDir = normalize(fragToLight + fragToEye);

        specularFactor = max(dot(halfwayDir, normalize(normal)), 0.0);

        // Clamping and specular highlights
        if (specularFactor > 0.0) {
            specularFactor = pow(specularFactor, material.shininess);

            // if(isShaded) {
            //     specularColour = vec4((light.colour * material.specularIntensity * specularFactor), 1.0);
            // }

            // else {
            specularColour = vec4((light.colour * material.specularIntensity * specularFactor), 1.0);
            // }
        }
    }

    return (ambientColour + diffuseColour + specularColour);
}

vec4 calcDirectionalLight() {
    return calcLightByDirection(directionalLight.base, directionalLight.direction);
}

// Main Function=======================================================================================================
void main() {
    float height = texture(diffuseMap, geomTexCoord).r;
    float heightRight = texture(diffuseMap, geomTexCoord + vec2(0.001, 0.0)).r;
    float heightUp = texture(diffuseMap, geomTexCoord + vec2(0.0, 0.001)).r;

    vec3 va = normalize(vec3(1.0, 0.0, (heightRight - height) * 100.0));
    vec3 vb = normalize(vec3(0.0, 1.0, (heightUp - height) * 100.0));

    // vec3 normal = cross(va, vb);
    normal = cross(va, vb);

    if(isWireframe) {
        outputColor = wireframeColor;
    }

    else if(isShaded) {
        outputColor = texture(diffuseMap, geomTexCoord);
    }

    else if(isNormal) {
        // Output normal as color
        outputColor = vec4(normal * 0.5 + 0.5, 1.0);
    }

    else {
        vec4 finalColour = calcDirectionalLight();
        outputColor = objectColor * finalColour;
    }
}
