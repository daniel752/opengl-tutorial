#version 330 core

in vec3 normal;
in vec3 fragPosition;
in vec2 textureCoordinates;

out vec4 fragColor;

uniform vec3 viewPosition;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

void main()
{
    vec3 lightDirection = normalize(light.position - fragPosition);

    // Ambient light calculation
    vec3 ambientLight = light.ambient * texture(material.diffuse, textureCoordinates).rgb;

    // Diffuse light calculation
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, light.direction), 0.0);
    vec3 diffuseLight = light.diffuse * diff * texture(material.diffuse, textureCoordinates).rgb;

    // Specular light calculation
    vec3 viewDirection = normalize(viewPosition -fragPosition);
    vec3 reflectedLightDirection = reflect(-light.direction, norm);
    float specular = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), material.shininess);
    vec3 specularLight = specular * light.specular * material.shininess * texture(material.specular, textureCoordinates).rgb;

    // Emission light from object
    vec3 emissionLight = texture(material.emission, textureCoordinates).rgb;

    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuseLight *= intensity;
    specularLight *= intensity;

    // Calculate attenuation
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    ambientLight *= attenuation;
    diffuseLight *= attenuation;
    specularLight *= attenuation;

    // By mixing different light components we get the Phong light model
    vec3 result = specularLight + diffuseLight + ambientLight + emissionLight;
    // Set fragment color
    fragColor = vec4(result, 1.0);
}