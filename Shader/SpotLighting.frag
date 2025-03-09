#version 330 core

#include "Shader/SpotLight.include"
#include "Shader/Material.include"

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform bool showTexture;

uniform SpotLight light;
uniform Material material;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction)); 

    if(theta <= light.cutOff && light.outerCutOff == 0) {
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoord)), 1.0);
    }
    else{
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;  
        
        // attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        
        diffuse   *= attenuation;
        specular *= attenuation;   
            
        if(light.outerCutOff != 0){
            ambient  *= attenuation; 
            float epsilon   = light.cutOff - light.outerCutOff;
            float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   
    
            diffuse   *= intensity;
            specular *= intensity; 
        }
        
        
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
   
    

}