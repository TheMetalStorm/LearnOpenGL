#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "SimiEng/Mesh.h"

namespace SimiEng {
	class Model {

	public:
		Model(const char* path);

		void Draw(SimiEng::Shader& shader);

	private:
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<Texture> textures_loaded;

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
			std::string typeName);
		unsigned int TextureFromFile(char const* path, std::string directory);
	};
}