export module shader;

import <string>;
import <iostream>;
import <sstream>;
import <fstream>;

import "glad/glad.h" ;

import "glm/glm.hpp";
import "glm/gtc/matrix_transform.hpp";
import "glm/gtc/type_ptr.hpp";

export namespace shader {
	class Shader
	{
	private:

		const std::string INCLUDE_PREFIX = "#include ";
		unsigned int createFragShader(const char* shaderSource) {
			int success;
			char infoLog[512];
			unsigned int  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &shaderSource, NULL);
			glCompileShader(fragmentShader);
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			};
			return fragmentShader;
		}

		unsigned int createVertShader(const char* shaderSource) {

			int success;
			char infoLog[512];
			unsigned int  vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &shaderSource, NULL);
			glCompileShader(vertexShader);
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			};

			return vertexShader;
		}

		// Inject "#include "<path>" " content into Shader File
		// Currently not recursive (only resolves #include in input to Shader constructor) and does not check for Circular dependency
		std::string preprocessGLSLFile(std::string originalCode) {

			std::istringstream f(originalCode);
			std::ostringstream processedCode;
			std::string line;
			while (std::getline(f, line)) {
				size_t substring_length = line.find(INCLUDE_PREFIX);

				if (substring_length == std::string::npos)
				{
					processedCode << line << std::endl;
					continue;
				}

				char charToRemove = '\"';
				std::string path = line.substr(substring_length + INCLUDE_PREFIX.length());
				path.erase(std::remove(path.begin(), path.end(), charToRemove), path.end());
				std::string includeCode;
				readFile(path.c_str(), includeCode);
				if (includeCode.length() != 0) {
					processedCode << includeCode << std::endl;
				}
			}
			return processedCode.str();
		}

	public:
		unsigned int ID;




		Shader(const char* vertexPath, const char* fragmentPath) {
			int success;
			char infoLog[512];
			//get shader src from file
			std::string vertexCode, fragmentCode;

			readFile(vertexPath, vertexCode);
			readFile(fragmentPath, fragmentCode);


			std::string actualVertexCode = preprocessGLSLFile(vertexCode);
			std::string actualFragmentCode = preprocessGLSLFile(fragmentCode);


			const char* vShaderCode = actualVertexCode.c_str();
			const char* fShaderCode = actualFragmentCode.c_str();

			unsigned int vertexShader = createVertShader(vShaderCode);
			unsigned int fragmentShader = createFragShader(fShaderCode);

			ID = glCreateProgram();
			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);
			glLinkProgram(ID);

			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}


			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);


		}


		void readFile(const char* path, std::string& code)
		{
			std::ifstream file;
			file.exceptions(std::ifstream::failbit | std::ifstream::badbit);


			try {
				file.open(path);
				std::stringstream stream;
				// read file's buffer contents into streams
				stream << file.rdbuf();
				// close file handlers
				file.close();
				// convert stream into string
				code = stream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::FILE_NOT_SUCCESFULLY_READ \nPath: " << path << std::endl;
			}
		}

		void use()
		{
			glUseProgram(ID);
		}

		void setBool(const std::string& name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}

		void setInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}
		void setFloat(const std::string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}
		void setVec4(const std::string& name, float v1, float v2, float v3, float v4) const
		{
			glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
		}

		void setVec3(const std::string& name, float v1, float v2, float v3) const
		{
			glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
		}
		void setVec3(const std::string& name, glm::vec3 vec) const
		{
			glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
		}

		void setMat4(const std::string& name, glm::mat4 matrix) const
		{
			glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
		}
	};
}