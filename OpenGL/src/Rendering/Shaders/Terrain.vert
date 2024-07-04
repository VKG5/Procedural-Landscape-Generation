#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 tangent;

out vec4 col;
out vec2 texCoord;

// We can use flat out vec3 Normal to give a fake flat look
// This is a provoking normal, meaning not interpolating between any values
out vec3 Normal;

// Outputting the TBN matrix
out mat3 TBNMatrix;

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

    // **Referenced from : https://learnopengl.com/Advanced-Lighting/Normal-Mapping**
    // Calculating TBN matrix for normal maps
    // Gram-Schmidt process - We are calculating the bitangents
    // directly in the vertex instead of reading them from the mesh
    vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(norm, 0.0)));

    // Re-Orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);

    // Retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);

    TBNMatrix = transpose(mat3(T, B, N));

    // Swizzling in GLSL
    fragPos = (model * vec4(pos, 1.0)).xyz;
}
