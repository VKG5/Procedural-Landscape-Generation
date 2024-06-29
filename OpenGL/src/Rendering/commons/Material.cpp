#include "Material.h"

Material::Material() {
    specularIntensity = 0.0f;
    shininess = 0.5f;
    metalness = 0.0f;
}

Material::Material(GLfloat specIntensity, GLfloat shine, GLfloat metal) {
    specularIntensity = specIntensity;
    shininess = shine;
    metalness = metal;
}

void Material::useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation, GLuint metalnessLocation) {
    // Specular Highlights
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
    glUniform1f(metalnessLocation, metalness);
}

void Material::setMaterialParamters(GLfloat specular, GLfloat shine, GLfloat metal) {
    specularIntensity = specular;
    shininess = shine;
    metalness = metal;
}

Material::~Material() {
}
