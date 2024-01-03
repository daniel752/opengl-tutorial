#version 330 core

in vec3 normal;
in vec3 fragPosition;
// Comment this line to switch to Phong lighting in world space
// in vec3 lightPosition;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 viewPosition;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
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
    vec3 ambientLight = material.ambient * light.ambient;
    // vec3 ambientLight = ambientStrength * lightColor;

    // Diffuse light calculation
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(light.position -fragPosition);
    // vec3 lightDirection = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuseLight = (material.diffuse * diff) * light.diffuse;
    // vec3 diffuseLight = (material.diffuse * diff) * lightColor;

    // Specular light calculation
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    // vec3 viewDirection = normalize(-fragPosition);
    vec3 reflectedLightDirection = reflect(-lightDirection, norm);
    float specular = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), material.shininess);
    vec3 specularLight = (specular * material.specular) * material.shininess * light.specular;
    
    // By mixing different light components with the object color we get the resulted object color with light
    vec3 result = (specularLight + diffuseLight + ambientLight) * objectColor;
    // vec3 result = specularLight + diffuseLight + ambientLight;
    // Set fragment color
    fragColor = vec4(result, 1.0);
}