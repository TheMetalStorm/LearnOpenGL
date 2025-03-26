export module spot_light;

import "glm/glm.hpp";

import shader;

export namespace spot_light{

class SpotLight {

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

	SpotLight(glm::vec3 pos, glm::vec3 dir, float cutOff, float outerCutOff, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float constant, float linear, float quadratic)
		: position(pos), direction(dir), cutOff(cutOff), outerCutOff(outerCutOff), ambient(amb), diffuse(diff), specular(spec), constant(constant), linear(linear), quadratic(quadratic) {}

	SpotLight()
		: position(0.0f, 10.0f, 0.0f), 
		direction(0.0f, -1.0f, 0.0f), 
		cutOff(glm::cos(glm::radians(12.5f))),   
		outerCutOff(glm::cos(glm::radians(15.0f))),
		ambient(0.1f, 0.1f, 0.1f),  
		diffuse(1.0f, 1.0f, 1.0f),   
		specular(1.0f, 1.0f, 1.0f),  
		constant(1.0f),
		linear(0.09f),
		quadratic(0.032f) {
	}

	void sendToShader(shader::Shader& shader) const {
	
		shader.setVec3("spotLight.position", position);
		shader.setVec3("spotLight.direction", direction);
		shader.setFloat("spotLight.cutOff", cutOff);
		shader.setFloat("spotLight.outerCutOff", outerCutOff);
		shader.setVec3("spotLight.ambient", ambient);
		shader.setVec3("spotLight.diffuse", diffuse);
		shader.setVec3("spotLight.specular",  specular);
		shader.setFloat("spotLight.constant", constant);
		shader.setFloat("spotLight.linear", linear);
		shader.setFloat("spotLight.quadratic", quadratic);
	}

};
}