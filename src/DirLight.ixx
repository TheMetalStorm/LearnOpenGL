export module dir_light;

import "glm/glm.hpp";

import shader;

export namespace dir_light {

    class DirLight {

    public:
        glm::vec3 dir, ambient, diffuse, specular;

        DirLight(const glm::vec3& dir, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
            : dir(dir), ambient(ambient), diffuse(diffuse), specular(specular) {
        }
        DirLight()
            : dir(-0.2f, -1.0f, -0.3f),
            ambient(0.1f, 0.1f, 0.1f),
            diffuse(1.0f, 1.0f, 1.0f),
            specular(1.0f, 1.0f, 1.0f) {
        }

        void sendToShader(shader::Shader& shader) const {
            shader.setVec3("dirLight.direction", dir);
            shader.setVec3("dirLight.ambient", ambient);
            shader.setVec3("dirLight.diffuse", diffuse);
            shader.setVec3("dirLight.specular", specular);
        }

    };
}