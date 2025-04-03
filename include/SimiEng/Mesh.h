#pragma once

#include <string>
#include <vector>
#include <stddef.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimiEng/Shader.h"

namespace SimiEng {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture {

        unsigned int id;
        std::string type;
        std::string path;  // we store the path of the texture to compare with other textures

    };

    class Mesh {

    public:

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader& shader);


    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh();
    };
}