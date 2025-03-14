#version 330 core
out vec4 FragColor;

in vec2 TexCoord;


#include "Shader/ModelMaterial.include"

uniform Material material;

void main()
{    
    FragColor = texture(material.texture_roughness1, TexCoord);
}