#pragma once

#include <glm/glm.hpp>

#include "SimiEng/SimiEng.h"
#include "SimiEng/Shader.h"

namespace SimiEng{
class SIMIENG_API DirLight {

public:
    glm::vec3 dir, ambient, diffuse, specular;

    DirLight(const glm::vec3& dir, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
    DirLight();
    ~DirLight() = default;

    void sendToShader(Shader& shader) const;

};
}