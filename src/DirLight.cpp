#include "SimiEng/DirLight.h"

namespace SimiEng{

    DirLight::DirLight(const glm::vec3& dir, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
        : dir(dir), ambient(ambient), diffuse(diffuse), specular(specular) {
    }
    DirLight::DirLight()
        : dir(-0.2f, -1.0f, -0.3f),
        ambient(0.1f, 0.1f, 0.1f),
        diffuse(1.0f, 1.0f, 1.0f),
        specular(1.0f, 1.0f, 1.0f) {
    }

    void DirLight::sendToShader(Shader& shader) const {
        shader.setVec3("dirLight.direction", dir);
        shader.setVec3("dirLight.ambient", ambient);
        shader.setVec3("dirLight.diffuse", diffuse);
        shader.setVec3("dirLight.specular", specular);
    }

};