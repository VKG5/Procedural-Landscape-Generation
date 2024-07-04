#version 460 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

// Input attributes
in vec4 col[];
in vec2 texCoord[];
in vec3 Normal[];
in mat3 TBNMatrix[];
in vec3 fragPos[];

// Copying the input attributes to outputs as is
out vec4 geomCol;
out vec2 geomTexCoord;
out vec3 geomNormal;
out mat3 geomTBNMatrix;
out vec3 geomFragPos;

// To remove perspective correct interpolation, meaning the depth of the pixel is NOT taken into account (Linear Interpolation)
noperspective out vec3 edgeDistance;

// MVP - Model, View, Projection Structure
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Checking the distance from the pixel to an edge of a triangle, hence perspective correct project might break that
    // Therefore, using Linear Interpolation
    vec4 p;

    mat4 mvp = projection * view * model;

    // Converting from NDC -> Screen Space Transformation
    /*
    x_ndc * windowWidth/2 + center_x = x_screenSpace
    y_ndc * windowHeight/2 + center_y = y_screenSpace

    | Clip Space -> NDC -> Screen Space |
    Clip Space position is available in gl_in array
    */
    p = gl_in[0].gl_Position;
    vec2 p0 = vec2(mvp * (p / p.w));

    p = gl_in[1].gl_Position;
    vec2 p1 = vec2(mvp * (p / p.w));

    p = gl_in[2].gl_Position;
    vec2 p2 = vec2(mvp * (p / p.w));

    // Calculating the wireframes based off of Pythagoras Theorem
    // Length of the three edges in Screen Space
    float a = length(p1 - p2);
    float b = length(p2 - p0);
    float c = length(p1 - p0);

    // a^2 = b^2 + c^2
    float alpha = acos((b * b + c * c - a * a) / (2.0 * b * c));
    float beta  = acos((a * a + c * c - b * b) / (2.0 * a * c));

    // Using Law of Cosines
    float ha = abs(c * sin(beta));
    float hb = abs(c * sin(alpha));
    float hc = abs(b * sin(alpha));

    /*
    in vec4 col[];
    in vec2 texCoord[];
    in vec3 Normal[];
    in mat3 TBNMatrix[];
    in vec3 fragPos[];
    */

    gl_Position = gl_in[0].gl_Position;
    gl_ClipDistance = gl_in[0].gl_ClipDistance;
    geomTexCoord = texCoord[0];
    geomNormal = Normal[0];
    geomFragPos = fragPos[0];
    geomTBNMatrix = TBNMatrix[0];
    geomCol = col[0];
    edgeDistance = vec3(ha, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    gl_ClipDistance = gl_in[1].gl_ClipDistance;
    geomTexCoord = texCoord[1];
    geomNormal = Normal[1];
    geomFragPos = fragPos[1];
    geomTBNMatrix = TBNMatrix[1];
    geomCol = col[1];
    edgeDistance = vec3(0.0, hb, 0.0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    gl_ClipDistance = gl_in[2].gl_ClipDistance;
    geomTexCoord = texCoord[2];
    geomNormal = Normal[2];
    geomFragPos = fragPos[2];
    geomTBNMatrix = TBNMatrix[2];
    geomCol = col[2];
    edgeDistance = vec3(0.0, 0.0, hc);
    EmitVertex();

    EndPrimitive();
}
