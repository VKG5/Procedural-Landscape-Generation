#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 col;
out vec2 texCoord;

// We can use flat out vec3 Normal to give a fake flat look
// This is a provoking normal, meaning not interpolating between any values
out vec3 Normal;

// For specular
out vec3 fragPos;

// MVP - Model, View, Projection Structure
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    col = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

    texCoord = tex;

    // We have to ignore the last column and row, hence mat3 conversion
    // Transpose and Inverse to preserve non-uniform scaling
    // Model is used to preserve scale and rotation
    // Since normal is just a direction
    Normal = mat3(transpose(inverse(model))) * norm;

    // Swizzling in GLSL
    fragPos = (model * vec4(pos, 1.0)).xyz;
}
