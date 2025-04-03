#version 330 core

#include "Resources/Shader/Material.include"
#include "Resources/Shader/PointLight.include"
#include "Resources/Shader/DirLight.include"
#include "Resources/Shader/SpotLight.include"

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform bool showTexture;

uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4  
uniform PointLight[NR_POINT_LIGHTS] pointLight;
uniform SpotLight spotLight;

vec3 calcDirLight(){
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuse, TexCoord));  

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, TexCoord));

    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, TexCoord));

    return (diffuse + ambient + specular);
}

vec3 calcSpotLight(){

    
    vec3 lightDir = normalize(spotLight.position - FragPos);
    float theta = dot(lightDir, normalize(-spotLight.direction)); 

    if(theta <= spotLight.cutOff && spotLight.outerCutOff == 0) {
        FragColor = vec4(spotLight.ambient * vec3(texture(material.diffuse, TexCoord)), 1.0);
    }

    else{
        // ambient
        vec3 ambient = spotLight.ambient * texture(material.diffuse, TexCoord).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = spotLight.diffuse * diff * texture(material.diffuse, TexCoord).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spotLight.specular * spec * texture(material.specular, TexCoord).rgb;  
        
        // attenuation
        float distance    = length(spotLight.position - FragPos);
        float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));    

        
        diffuse   *= attenuation;
        specular *= attenuation;   
            
        if(spotLight.outerCutOff != 0){
            ambient  *= attenuation; 
            float epsilon   = spotLight.cutOff - spotLight.outerCutOff;
            float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);   
    
            diffuse   *= intensity;
            specular *= intensity; 
        }
        return ambient + diffuse + specular;
    }   
}

vec3 calcPointLight(int i){
    
    float distance    = length(pointLight[i].position - FragPos);
    float attenuation = 1.0 / (pointLight[i].constant + pointLight[i].linear * distance + 
    		    pointLight[i].quadratic * (distance * distance));   

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight[i].position - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight[i].diffuse * diff * vec3(texture(material.diffuse, TexCoord)) * attenuation;  

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pointLight[i].specular * spec * vec3(texture(material.specular, TexCoord)) * attenuation;

    vec3 ambient = pointLight[i].ambient * vec3(texture(material.diffuse, TexCoord)) * attenuation;

    return (diffuse + ambient + specular);
}

void main()
{
      vec3 finalColor = vec3(0.0);
      finalColor += calcDirLight();
      for(int i = 0; i < NR_POINT_LIGHTS; i++)
  	    finalColor += calcPointLight(i);
      finalColor += calcSpotLight();
  
    finalColor *= objectColor;
      FragColor = vec4(finalColor, 1.0);    
}

