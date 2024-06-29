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
};

uniform int pointLightCount;
uniform int spotLightCount;

// Lights
uniform DirectionalLight directionalLight;
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

// Textures
uniform sampler2D theTexture;

// Materials
uniform Material material;

// Eye Position
uniform vec3 eyePosition;

vec4 calcLightByDirection(Light light, vec3 direction) {
    // Ambient Light
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    // Diffuse Light
    // Getting the cosine of angle between two vectors
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
    vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);

    // Specular Light
    vec4 specularColour = vec4(0, 0, 0, 0);

    // If no diffuse, then no specular
    if(diffuseFactor > 0.0f) {
        vec3 fragToEye = normalize(eyePosition - fragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);

        if(specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColour + diffuseColour + specularColour);
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
        // Hacky way for spotlights
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
    float edgeThreshold = 0.2;
    float smoothEdge = smoothstep(edgeThreshold, edgeThreshold + 0.5, diffuseFactor);

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

void main() {
    // vec4 finalColour = calcDirectionalLight();
    // finalColour += calcPointLights();
    // finalColour += calcSpotLights();

    vec4 finalColour = calcGoochComplex(directionalLight.base, directionalLight.direction);

    color = texture(theTexture, texCoord) * finalColour;
}
