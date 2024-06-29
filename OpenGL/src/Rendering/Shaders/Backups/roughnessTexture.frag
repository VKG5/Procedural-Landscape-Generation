#version 460 core

in vec4 col;
in vec2 texCoord;
in vec3 Normal;
in vec3 fragPos;

out vec4 color;

// Should be same as Utilities.h header file
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

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
    float roughness;
};

// Counter variables
uniform int pointLightCount;
uniform int spotLightCount;
uniform int shadingModel;

// Object Color
uniform vec4 objectColor;
uniform vec4 wireframeColor;
uniform bool materialPreview;

// Wireframe or Shaded
uniform bool isWireframe;
uniform bool isShaded;

// Lights
uniform DirectionalLight directionalLight;
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

// Textures
// Bound at Texture Unit 0
uniform sampler2D theTexture;

// Bound at Texture Unit 1
// uniform sampler2D noiseTexture;

// Materials
uniform Material material;

// Eye Position
uniform vec3 eyePosition;

// Skybox
uniform samplerCube environmentMap;
bool envMapping = true;

vec3 calcEnvironmentMapping() {
    if (material.roughness > 0.0) {
        // Perturbing the reflection based on generated noise texture
        vec3 randVec = normalize(vec3(texture(noiseTexture, fragPos.xy * 0.1).r * 2.0 - 1.0,
                                      texture(noiseTexture, fragPos.yz * 0.1).r * 2.0 - 1.0,
                                      texture(noiseTexture, fragPos.xz * 0.1).r * 2.0 - 1.0));

        reflectedDir = normalize(reflect(fragPos - eyePosition, normalize(Normal)) + material.roughness * randVec);
    }

    else {
        reflectedDir = reflect(fragPos - eyePosition, normalize(Normal));
    }

    return texture(environmentMap, reflectedDir).rgb;
}

vec4 calcLightByDirection(Light light, vec3 direction) {
    // Ambient Light
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    // Diffuse Light
    // Getting the cosine of angle between two vectors
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0);
    vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0);

    // Specular Light
    vec4 specularColour = vec4(0, 0, 0, 0);

    // If no diffuse, then no specular
    if(diffuseFactor > 0.0) {
        float specularFactor = 0.0;

        // Phong-Blinn Illumination
        if(shadingModel == 1) {
            // Calculating Halfway Direction for Phong-Blinn Model
            vec3 fragToLight = normalize(direction);
            vec3 fragToEye = normalize(fragPos - eyePosition);
            vec3 halfwayDir = normalize(fragToLight + fragToEye);

            specularFactor = max(dot(halfwayDir, normalize(Normal)), 0.0);
        }

        // Phong Illumination
        else if (shadingModel == 0) {
            vec3 fragToEye = normalize(eyePosition - fragPos);
            vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

            specularFactor = dot(fragToEye, reflectedVertex);
        }

        else {
            specularFactor = 0.0;
        }

        if (specularFactor > 0.0) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0);
        }
    }

    vec3 envColor = calcEnvironmentMapping();

    if(envMapping) {
        // Combine diffuse and specular with environment reflection
        // return (ambientColour + diffuseColour + specularColour);
        return  (ambientColour + diffuseColour) + specularColour * (1.0 - material.roughness) +
                vec4(envColor * (1.0 - material.roughness) * material.specularIntensity, 1.0 );
    }

    else {
        return (ambientColour + diffuseColour + specularColour);
    }
}

vec4 calcDirectionalLight() {
    return calcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 calcPointLightsBase(PointLight pLight) {
    // Calculating Direction of our Point Lights
    // Getting direction from light to fragment
    vec3 direction = fragPos - pLight.position;
    float distance = length(direction);

    direction = normalize(direction);

    vec4 colour = calcLightByDirection(pLight.base, direction);

    float attenuation = pLight.exponent * distance * distance +
                        pLight.linear * distance +
                        pLight.constant;

    return (colour / attenuation);
}

vec4 calcSpotLightsBase(SpotLight sLight) {
    // Getting Direction
    vec3 rayDirection = normalize(fragPos - sLight.base.position);
	float slFactor = dot(rayDirection, normalize(sLight.direction));
    vec4 colour = vec4(0, 0, 0, 0);

    // If we are withing range
    if(slFactor > sLight.edge) {
        colour = calcPointLightsBase(sLight.base);
        if(colour != vec4(0, 0, 0, 0)) {
            colour *= (1.0 - (1.0 - slFactor) * (1.0/(1.0 - sLight.edge)));
        }
    }

    return colour;
}

vec4 calcPointLights() {
    vec4 totalColour = vec4(0, 0, 0, 0);

    for(int i = 0; i < pointLightCount; i++) {
        totalColour += calcPointLightsBase(pointLight[i]);
    }

    return totalColour;
}

vec4 calcSpotLights() {
    vec4 totalColour = vec4(0, 0, 0, 0);

    for(int i = 0; i < spotLightCount; i++) {
        totalColour += calcSpotLightsBase(spotLight[i]);
    }

    return totalColour;
}

vec4 calcGoochComplex(Light light, vec3 direction) {
    // Ambient Light
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    // Gooch Shading Colors
    vec3 warmColor = vec3(1.0, 0.6, 0.0);  // Warm color for highlights
    vec3 coolColor = vec3(0.0, 0.4, 1.0);  // Cool color for shadows

    // Diffuse Light (Gooch Shading)
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);

    // Soften the edge with smoothstep
    float edgeThreshold = 0.25;
    float smoothEdge = smoothstep(edgeThreshold, edgeThreshold + 0.369, diffuseFactor);

    // Interpolate between warm and cool colors based on the smooth edge
    vec3 diffuseColor = mix(coolColor, warmColor, smoothEdge);

    vec4 diffuseColour = vec4(diffuseColor * light.colour * light.diffuseIntensity, 1.0f);

    // Specular Light (Gooch Shading)
    vec4 specularColour = vec4(0, 0, 0, 0);

    // If no diffuse, then no specular
    if (diffuseFactor > 0.0f) {
        vec3 fragToEye = normalize(eyePosition - fragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);

        if (specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, material.shininess);
            // Gooch shading for specular highlights (you can customize warm and cool colors)
            vec3 specularColor = mix(coolColor, warmColor, specularFactor);
            specularColour = vec4(specularColor * light.colour * material.specularIntensity, 1.0f);
        }
    }

    return (ambientColour + diffuseColour + specularColour);
}

vec4 calcMinnaert(Light light, vec3 direction) {
    // Ambient Light
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    // Diffuse Light
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0);
    vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0);

    // Minnaert Shading
    vec3 fragToLight = normalize(direction);

    float minnaertExponent = 0.1;
    float cosTheta = max(dot(normalize(Normal), fragToLight), 0.0);
    float minnaertTerm = (cosTheta * cosTheta) / (cosTheta + minnaertExponent);

    vec4 specularColour = vec4(light.colour * material.specularIntensity * minnaertTerm, 1.0);

    return (ambientColour + diffuseColour + specularColour);
}

void main() {
    // If we just want to view the textures
    if(materialPreview) {
        if(isShaded)
            color = texture(theTexture, texCoord);

        else
            color = objectColor;
    }

    // If we are not in the wireframe mode
    // And the texture are enabled
    else if(!isWireframe && isShaded) {
        vec4 finalColour = vec4(1.0, 1.0, 1.0, 1.0);

        if(shadingModel < 2) {
            finalColour = calcDirectionalLight();
            finalColour += calcPointLights();
            finalColour += calcSpotLights();
        }

        else if(shadingModel == 2) {
            finalColour = calcGoochComplex(directionalLight.base, directionalLight.direction);
        }

        else {
            finalColour = calcMinnaert(directionalLight.base, directionalLight.direction);
        }

        color = texture(theTexture, texCoord) * finalColour;
    }

    // If we are not in the wireframe mode
    // And textures are disabled
    else if(!isWireframe && !isShaded) {
        vec4 finalColour = vec4(1.0, 1.0, 1.0, 1.0);

        if(shadingModel < 2) {
            finalColour = calcDirectionalLight();
            finalColour += calcPointLights();
            finalColour += calcSpotLights();
        }

        else if(shadingModel == 2) {
            finalColour = calcGoochComplex(directionalLight.base, directionalLight.direction);
        }

        else {
            finalColour = calcMinnaert(directionalLight.base, directionalLight.direction);
        }

        color = objectColor * finalColour;
    }

    // If we are in wireframe mode
    else if(isWireframe) {
        color = wireframeColor;
    }

    else {
        color = objectColor;
    }
}
