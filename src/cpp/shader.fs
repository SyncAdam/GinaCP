#version 330 core

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.15;
    vec3 ambient = vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * vec3(texture(material.specular, TexCoords));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords));    

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);

}