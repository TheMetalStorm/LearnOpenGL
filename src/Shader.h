#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
private:
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

public:
	unsigned int ID;

	
	

	Shader(const char* vertexPath, const char* fragmentPath) {
		int success;
		char infoLog[512];
		//get shader src from file
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile, fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ, either " << vertexPath << " or " << fragmentPath << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

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

	void use()
	{
		glUseProgram(ID);
	}

	void setBool(const std::string &name, bool value) const
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