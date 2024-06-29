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
    float metalness;
};

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
uniform DirectionalLight directionalLight;
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

// Textures
// Bound at Texture Unit 0
uniform sampler2D diffuseMap;

// Bound at Texture Unit 1
uniform sampler2D ambientOcclusionMap;

// Bound at Texture Unit 2
uniform sampler2D specularMap;

// Bound at Texture Unit 3
uniform sampler2D normalMap;

// Bound at Texture Unit 1
// uniform sampler2D noiseTexture;

// Materials
uniform Material material;

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

vec3 calcReflection() {
    // Environment Reflection Mapping
    vec3 reflectedDir = reflect(-normalize(fragPos - eyePosition), normalize(Normal));
    return texture(environmentMap, -reflectedDir).rgb;
}

vec3 calcRefraction() {
    // Environment Refraction Mapping
    float ratioR = 1.00 / ior + dispersion;
    float ratioG = 1.00 / ior;
    float ratioB = 1.00 / ior - dispersion;

    // Simulating dispersion by adjusting the refraction direction for each channel
    vec3 refractedDirR = refract(-normalize(fragPos - eyePosition), normalize(Normal), ratioR);
    vec3 refractedDirG = refract(-normalize(fragPos - eyePosition), normalize(Normal), ratioG);
    vec3 refractedDirB = refract(-normalize(fragPos - eyePosition), normalize(Normal), ratioB);

    // Combining the various channels
    float colourR = texture(environmentMap, -refractedDirR).r;
    float colourG = texture(environmentMap, -refractedDirG).g;
    float colourB = texture(environmentMap, -refractedDirB).b;

    return vec3(colourR, colourG, colourB);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
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

        // Clamping and specular highlights
        if (specularFactor > 0.0) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4((light.colour * material.specularIntensity * texture(specularMap, texCoord).r * specularFactor), 1.0);
        }
    }

    if(envMapping) {
        if(skybox) {
            // Setting up empty environment color
            vec3 envColor = vec3(0.0, 0.0, 0.0);

            // Reflection
            if(reflection && !refraction) {
                envColor = calcReflection();
            }

            // Refraction
            else if (!reflection && refraction) {
                envColor = calcRefraction();
            }

            // Fresnel
            else if (reflection && refraction) {
                vec3 incident = -normalize(fragPos - eyePosition);
                vec3 normal = -normalize(Normal);

                float cosTheta = dot(incident, normal);
                float sinTheta2 = 1.0 - cosTheta * cosTheta;

                if(sinTheta2 < 0.0) {
                    envColor = objectColor.rgb;
                }

                else {
                    vec3 F0 = vec3(reflectance);
                    F0 = mix(F0, objectColor.rgb, material.metalness);

                    // Calculate fresnel reflection using Schlick's approximation
                    vec3 fresenelFactor = fresnelSchlick(cosTheta, F0);
                    envColor = mix( calcRefraction(), calcReflection(), fresenelFactor );
                }
            }

            // Combine diffuse and specular with environment reflection
            // return (ambientColour + diffuseColour + specularColour);
            // Also using the specular map if shaded
            if(isShaded) {
                return  ( ambientColour + diffuseColour) +
                          specularColour * (1.0 - material.metalness) +
                          vec4(envColor * (1.0 - material.metalness) * material.specularIntensity * texture(specularMap, texCoord).r, 1.0 );
            }

            else {
                return  ( ambientColour + diffuseColour) +
                          specularColour * (1.0 - material.metalness) +
                          vec4(envColor * (1.0 - material.metalness) * material.specularIntensity, 1.0 );
            }
        }

        else {
            // Combine diffuse and specular with background color
            if(isShaded) {
                return  ( ambientColour + diffuseColour) +
                          specularColour * (1.0 - material.metalness) +
                          vec4(backgroundColor * (1.0 - material.metalness) * material.specularIntensity * texture(specularMap, texCoord).r, 1.0 );
            }

            else {
                return  ( ambientColour + diffuseColour) +
                          specularColour * (1.0 - material.metalness) +
                          vec4(backgroundColor * (1.0 - material.metalness) * material.specularIntensity, 1.0 );
            }
        }
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
        if(isShaded) {
            if(specularPreview) {
                color = texture(specularMap, texCoord);
            }

            else if(normalPreview) {
                color = texture(normalMap, texCoord);
            }

            else {
                color = texture(diffuseMap, texCoord);
            }
        }

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

        color = texture(diffuseMap, texCoord) * finalColour;
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
