#pragma once

#include <glm/common.hpp>

#include "SimiEng/Shader.h"
#include "SimiEng/SimiEng.h"


namespace SimiEng {
    class SIMIENG_API PointLight {
    public:
        glm::vec3 position;

        float constant;
        float linear;
        float quadratic;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        PointLight();
        PointLight(glm::vec3 pos, float cons, float lin, float quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
        ~PointLight() = default;

        void sendToShader(Shader& shader, int index) const;
    };
}