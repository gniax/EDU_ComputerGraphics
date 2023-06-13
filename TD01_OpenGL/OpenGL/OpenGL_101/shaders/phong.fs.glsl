#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // diffuse 
    vec3 diff = max(dot(norm, lightDir), 0.0) * light.diffuse * vec3(texture(material.diffuse, TexCoord));

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    vec3 spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * light.specular * vec3(texture(material.specular, TexCoord));  
    
    vec3 result = ambient + diff + spec;
    FragColor = vec4(result, 1.0);
}