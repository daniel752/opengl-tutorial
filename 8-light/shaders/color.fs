#version 330 core

in vec3 normal;
in vec3 fragPosition;
in vec2 textureCoordinates;

out vec4 fragColor;

uniform vec3 viewPosition;

struct Material
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    // Ambient light calculation
    vec3 ambientLight = light.ambient * texture(material.diffuse, textureCoordinates).rgb;

    // Diffuse light calculation
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(light.position -fragPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuseLight = light.diffuse * diff * texture(material.diffuse, textureCoordinates).rgb;

    // Specular light calculation
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectedLightDirection = reflect(-lightDirection, norm);
    float specular = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), material.shininess);
    vec3 specularLight = (specular * material.specular) * material.shininess * light.specular;
    
    // By mixing different light components we get the Phong light model
    vec3 result = specularLight + diffuseLight + ambientLight;
    // Set fragment color
    fragColor = vec4(result, 1.0);
}