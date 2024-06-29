#include "Picking.h"

Picking::Picking() {
}

bool Picking::testRayOBBIntersection(glm::vec3 ray_origin,
                                     glm::vec3 ray_direction,
                                     glm::vec3 aabb_min,
                                     glm::vec3 aabb_max,
                                     glm::mat4 ModelMatrix,
                                     float & intersection_distance) {
    // *Reference : https://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
    /*
    This is a simple implementation of a ray picking based on Oriented Bounding Boxes method.
    In this we are simple drawing cubic/rectangular boxes around each object and checking intersection between those boxes.
    This gives a solution to picking objectes with reasonable accuracy while maintaining performance.
    */
	float tMin = 0.0f;
	float tMax = 100000.0f;

    return true;
}

Picking::~Picking() {
}
