#version 330 core

#include "Resources/Shader/DirLight.include"
#include "Resources/Shader/Material.include"

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform bool showTexture;

uniform DirLight light;
uniform Material material;

void main()
{

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));  

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 result = (diffuse + ambient + specular) * objectColor;
    FragColor = vec4(result, 1.0);
    
}