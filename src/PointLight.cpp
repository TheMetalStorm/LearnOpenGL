#include "SimiEng/PointLight.h"

namespace SimiEng {
    PointLight::PointLight()
        : position(glm::vec3(0.0f, 0.0f, 0.0f)),
        constant(1.0f),
        linear(0.09f),
        quadratic(0.032f),
        ambient(glm::vec3(0.2f, 0.2f, 0.2f)),
        diffuse(glm::vec3(0.5f, 0.5f, 0.5f)),
        specular(glm::vec3(1.0f, 1.0f, 1.0f)) {
    }

    PointLight::PointLight(glm::vec3 pos, float cons, float lin, float quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
        : position(pos), constant(cons), linear(lin), quadratic(quad), ambient(amb), diffuse(diff), specular(spec) {
    }

    void PointLight::sendToShader(Shader& shader, int index) const {
        shader.setVec3(std::string("pointLight[").append(std::to_string(index)).append("].position"), position);
        shader.setFloat(std::string("pointLight[").append(std::to_string(index)).append("].constant"), constant);
        shader.setFloat(std::string("pointLight[").append(std::to_string(index)).append("].linear"), linear);
        shader.setFloat(std::string("pointLight[").append(std::to_string(index)).append("].quadratic"), quadratic);
        shader.setVec3(std::string("pointLight[").append(std::to_string(index)).append("].ambient"), ambient);
        shader.setVec3(std::string("pointLight[").append(std::to_string(index)).append("].diffuse"), diffuse);
        shader.setVec3(std::string("pointLight[").append(std::to_string(index)).append("].specular"), specular);

    }
};