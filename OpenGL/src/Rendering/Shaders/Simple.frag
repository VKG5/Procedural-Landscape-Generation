#version 460 core

in vec4 col;
in vec2 texCoord;
in vec3 Normal;
in vec3 fragPos;
in mat3 TBNMatrix;

out vec4 color;

// Corrected Normal
vec3 normalTBN;
vec3 viewDir;
float specularMapFactor;

// Counter variables
uniform int pointLightCount;
uniform int spotLightCount;
uniform int shadingModel;

// Object Color
uniform vec4 objectColor;
uniform vec4 wireframeColor;

// Material Preview Mode
uniform bool materialPreview;
uniform bool specularPreview;
uniform bool normalPreview;

// Wireframe or Shaded
uniform bool isWireframe;
uniform bool isShaded;

// Lights
// uniform DirectionalLight directionalLight;
// uniform PointLight pointLight[MAX_POINT_LIGHTS];
// uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

// Textures
// Bound at Texture Unit 0
uniform sampler2D diffuseMap;
uniform sampler2D ambientOcclusionMap;

// Texture Unit 1
uniform sampler2D specularMap;

// Texture Unit 2
uniform sampler2D normalMap;

// Bound at Texture Unit 1
// uniform sampler2D noiseTexture;

// Materials
// uniform Material material;

// Eye Position
uniform vec3 eyePosition;

// Skybox
uniform samplerCube environmentMap;
uniform bool envMapping;
uniform bool skybox;
uniform vec3 backgroundColor;

// Transmission
uniform bool reflection;
uniform bool refraction;
uniform float ior;
uniform float reflectance;
uniform float dispersion;
uniform float normalStrength;
uniform float specularStrength;

// Anaglyph rendering
uniform float depthEffectStrength = 0.005;

vec4 renderAnaglyph() {
    // Basic depth-based offset for red channel (left eye)
    float depth = texture(diffuseMap, texCoord).z;
    vec2 redOffset = vec2(depthEffectStrength * depth, 0.0);
    float red = texture(diffuseMap, texCoord + redOffset).r;

    // Cyan channels (right eye), no offset in this simple example
    float green = texture(diffuseMap, texCoord - redOffset).g;
    float blue = texture(diffuseMap, texCoord - redOffset).b;

    return vec4(red, green, blue, 1.0);
}

void main() {
    // If we just want to view the textures
    if(materialPreview) {
        color = texture(diffuseMap, texCoord);
    }

    // If we are in wireframe mode
    else if(isWireframe) {
        color = wireframeColor;
    }

    else {
        color = renderAnaglyph();
    }
}
