#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform bool showTexture;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform Material material;
uniform Light light;

void main()
{

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    vec3 ambient  = light.ambient * material.ambient;

    vec3 result = (diffuse + ambient + specular) * objectColor;
    if(showTexture){
        FragColor = vec4(result, 1.0) * texture(texture1, TexCoord);
    }
    else{
        FragColor = vec4(result, 1.0);

    }
}