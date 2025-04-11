#pragma once

#include <glm/glm.hpp>

#include "SimiEng/Shader.h"
#include "SimiEng/SimiEng.h"

namespace SimiEng {
	class SIMIENG_API SpotLight {

	public:

		glm::vec3 position;
		glm::vec3 direction;
		float cutOff;
		float outerCutOff; // set this to Non-Zero for soft edges 
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;

		SpotLight(glm::vec3 pos, glm::vec3 dir, float cutOff, float outerCutOff, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float constant, float linear, float quadratic);
		SpotLight();
		~SpotLight() = default;

		void sendToShader(Shader& shader) const;
	};
};