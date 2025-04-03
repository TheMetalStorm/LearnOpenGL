#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SimiEng {
	class Shader
	{
	private:
		const std::string INCLUDE_PREFIX = "#include ";

		unsigned int createFragShader(const char* shaderSource);
		unsigned int createVertShader(const char* shaderSource);
		void readFile(const char* path, std::string& code);

		// Inject "#include "<path>" " content into Shader File
		// Currently not recursive (only resolves #include in input to Shader constructor) and does not check for Circular dependency
		std::string preprocessGLSLFile(std::string originalCode);

	public:
		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath);



		void use();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec4(const std::string& name, float v1, float v2, float v3, float v4) const;
		void setVec3(const std::string& name, float v1, float v2, float v3) const;
		void setVec3(const std::string& name, glm::vec3 vec) const;
		void setMat4(const std::string& name, glm::mat4 matrix) const;
	};
}