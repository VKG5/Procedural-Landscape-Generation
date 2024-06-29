#include "Utilities.h"

void calcAverageNormals( unsigned int* indices, unsigned int indexCount,
                         GLfloat* vertices, unsigned int vertexCount,
                         unsigned int vLength, unsigned int normalOffset ) {
    // Iterating over the indices
    for(size_t i = 0; i < indexCount; i += 3) {
        unsigned int index1 = indices[i] * vLength;
        unsigned int index2 = indices[i+1] * vLength;
        unsigned int index3 = indices[i+2] * vLength;

        // Creating edges
        glm::vec3 v1(   vertices[index2] - vertices[index1],
                        vertices[index2 + 1] - vertices[index1 + 1],
                        vertices[index2 + 2] - vertices[index1 + 2] );

        glm::vec3 v2(   vertices[index3] - vertices[index1],
                        vertices[index3 + 1] - vertices[index1 + 1],
                        vertices[index3 + 2] - vertices[index1 + 2] );

        glm::vec3 normal = glm::cross(v1, v2);

        normal = glm::normalize(normal);

        // Skipping the values by required places
        index1 += normalOffset; index2 += normalOffset; index3 += normalOffset;

        vertices[index1] += normal.x; vertices[index1+1] += normal.y; vertices[index1+2] += normal.z;
        vertices[index2] += normal.x; vertices[index2+1] += normal.y; vertices[index2+2] += normal.z;
        vertices[index3] += normal.x; vertices[index3+1] += normal.y; vertices[index3+2] += normal.z;
    }

    // Normalizing the normal vector
    for(size_t i=0; i < vertexCount/vLength; i++) {
        unsigned int NOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[NOffset], vertices[NOffset + 1], vertices[NOffset + 2]);
        vec = glm::normalize(vec);
        vertices[NOffset] = vec.x; vertices[NOffset + 1] = vec.y; vertices[NOffset + 2] = vec.z;
    }
}

// *Comment out for older code!*
void setShadingModeName(const GUI& GUI, const int& shadingModel, std::string& shadingMode) {
    if(GUI.getMaterialPreview()) {
        shadingMode = "Material Preview";
    }

    else if(GUI.getIsWireframe()) {
        shadingMode = "Wireframe";
    }

    else if(shadingModel == 0) {
        shadingMode = "Phong Illumination";
    }

    else if(shadingModel == 1) {
        shadingMode = "Blinn-Phong Illumination";
    }

    else if(shadingModel == 2) {
        shadingMode = "Gooch Shading";
    }

    else if(shadingModel == 3) {
        shadingMode = "Minnaert Shading";
    }

    else {
        shadingMode = "None";
    }
}

// Helpers=============================================================================================================
void printVec3GUI(const glm::vec3& vector, const int& index) {
    ImGui::Text("Vector {%i} components: (%.2f, %.2f, %.2f)\n", index, vector.x, vector.y, vector.z);
}

void printMat4GUI(const glm::mat4& mat, const int& index) {
    ImGui::Text("Index : %i Matrix:\n", index);
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            // Print element and a trailing space only after last column
            ImGui::Text("%.2f%s", mat[col][row], (col == 3) ? "" : " ");
            ImGui::SameLine();
        }
        ImGui::Text("");
    }
}

std::string removeBackslash(const char* str) {
    // Convert const char* to std::string
    std::string strPath = str;

    // Replace backslashes with forward slashes
    std::replace(strPath.begin(), strPath.end(), '\\', '/');

    return strPath;
}


std::string returnPath(const std::filesystem::path currentSourceDir, const std::string fileName) {
    std::string textureDir = (currentSourceDir / fileName).string();
    std::string texturePath = removeBackslash(textureDir.c_str());

    return texturePath;
}
