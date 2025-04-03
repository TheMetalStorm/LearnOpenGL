#version 330 core
out vec4 FragColor;

#include "Resources/Shader/Light.include"

uniform Light light;
void main()
{
	FragColor =  vec4(light.ambient, 1.0) + vec4(light.diffuse, 1.0) + vec4(light.specular, 1.0);
}