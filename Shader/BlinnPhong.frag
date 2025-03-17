#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;


#include "Shader/ModelMaterial.include"
#include "Shader/PointLight.include"
#include "Shader/DirLight.include"
#include "Shader/SpotLight.include"

uniform Material material;
uniform DirLight dirLight;
uniform vec3 viewPos;
#define NR_POINT_LIGHTS 4  
uniform PointLight[NR_POINT_LIGHTS] pointLight;
uniform SpotLight spotLight;

#include "Shader/BlinnPhongCalculations.include"

void main()
{    

    vec3 finalColor = vec3(0.0);
    finalColor += calcDirLight(
        vec3(texture(material.texture_normal1, TexCoords )),
        vec3(texture(material.texture_diffuse1, TexCoords)), 
        vec3(texture(material.texture_specular1, TexCoords)),
        material.shininess); 

    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        finalColor += calcPointLight(
        i, 
        vec3(texture(material.texture_normal1, TexCoords)),
        vec3(texture(material.texture_diffuse1, TexCoords)), 
        vec3(texture(material.texture_specular1, TexCoords)),
        material.shininess); 
    }

    finalColor += calcSpotLight(
        vec3(texture(material.texture_normal1, TexCoords )), 
        vec3(texture(material.texture_diffuse1, TexCoords)), 
        vec3(texture(material.texture_specular1, TexCoords)),
        material.shininess); 

    FragColor = vec4(finalColor, 1.0);   


}