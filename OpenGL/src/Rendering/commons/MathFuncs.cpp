#include "MathFuncs.h"

void printVec3(const glm::vec3& vector, const int& index) {
    printf("Vector components: (%.2f, %.2f, %.2f)\n", vector.x, vector.y, vector.z);
}

void printMat4(const glm::mat4& mat, const int& index) {
    printf("Matrix:\n");
    for (int row = 0; row < 4; row++) {
        // Indent for better readability
        printf("   ");
        for (int col = 0; col < 4; col++) {
            // Print element and a trailing space only after last column
            printf("%.2f%s", mat[col][row], (col == 3) ? "" : " ");
        }
        printf("\n");
    }
}
glm::vec3 invertAxis(const glm::vec3& originalVec, const int& axis) {
    if(axis == 0) {
        return glm::vec3(-originalVec.x, originalVec.y, originalVec.z);
    }

    if(axis == 1) {
        return glm::vec3(originalVec.x, -originalVec.y, originalVec.z);
    }

    if(axis == 2) {
        return glm::vec3(originalVec.x, originalVec.y, -originalVec.z);
    }

    // If none, return the original vector
    return originalVec;
}

float calcMagnitude(const glm::vec3& vecA) {
    return glm::length(vecA);
}

glm::vec3 calculateVector(const glm::vec3 & pointA, const glm::vec3 & pointB) {
    return glm::vec3(pointB - pointA);
}

float calculateAngleBetween(const glm::vec3& vecA, const glm::vec3& vecB, const glm::vec3& origin) {
    return glm::acos(glm::dot(glm::normalize(vecA - origin), glm::normalize(vecB - origin)));
}

float calculateAngleBetween(const glm::vec3& vecA, const glm::vec3& vecB) {
    return glm::acos(glm::dot(glm::normalize(vecA), glm::normalize(vecB)));
}

glm::vec3 calculateRotationAxis(const glm::vec3& vecA, const glm::vec3& vecB, const glm::vec3& origin) {
    return glm::cross(glm::normalize(vecA - origin), glm::normalize(vecB - origin));
}

glm::vec3 calculateRotationAxis(const glm::vec3& vecA, const glm::vec3& vecB) {
    return glm::cross(glm::normalize(vecA), glm::normalize(vecB));
}

float calcDifferenceMagnitude(const glm::vec3& vecA, const glm::vec3& vecB) {
    return glm::length(glm::abs(vecA - vecB));
}

float lerp(float start, float end, float t) {
    return start + (end - start) * t;
}

glm::vec3 calculateBezierPoint(float t, const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3) {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    glm::vec3 point = uuu * P0; // Start point influenced by u cube
    point += 3 * uu * t * P1; // Control point 1 influence
    point += 3 * u * tt * P2; // Control point 2 influence
    point += ttt * P3; // End point influence

    return point;
}

float easeInOut(float t) {
    return t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
}

glm::vec3 moveFromAToB(int type, const glm::vec3& A, const glm::vec3& B, float t) {
    float easedT = easeInOut(t);

    // Linear Interpolation
    if(type == 1) {
        return glm::vec3(
            lerp(A.x, B.x, easedT),
            lerp(A.y, B.y, easedT),
            lerp(A.z, B.z, easedT)
        );
    }

    // Bezier Interpolation
    else if(type == 2) {
        // Defining control points for the cubic Bezier curve.
        // Generating control points based on the direction from A to B.
        // Adjust control points as needed for your desired curve.
        glm::vec3 P1 = A + (B - A) * 0.33f; // Control point 1
        glm::vec3 P2 = A + (B - A) * 0.66f; // Control point 2

        return calculateBezierPoint(t, A, P1, P2, B);
    }

    else {
        printf("Select a valid type of interpolation!");
        return A;
    }
}

bool areFloatArraysEqual(const float a[3], const float b[3], float epsilon) {
    for (int i = 0; i < 3; ++i) {
        if (std::fabs(a[i] - b[i]) > epsilon) {
            return false;
        }
    }
    return true;
}
