#version 330 core

#define NUM_LIGHT_POINTS 5

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

struct SpotLight
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

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform SpotLight spotLight;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUM_LIGHT_POINTS];

vec3 calculateDirectionLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight pointLight, vec3 normal, vec3 fragPosition, vec3 viewDirection);
vec3 calculateSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPosition, vec3 viewDirection);

void main()
{
    vec3 result = vec3(0.0);

    vec3 viewDirection = normalize(viewPosition -fragPosition);

    result += calculateDirectionLight(directionalLight, normal, viewDirection);

    for(int i = 0; i < NUM_LIGHT_POINTS; i++)
        result += calculatePointLight(pointLights[i], normal, fragPosition, viewDirection);

    result += calculateSpotLight(spotLight, normal, fragPosition, viewDirection);

    fragColor = vec4(result, 1.0);
}

vec3 calculateDirectionLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-directionalLight.direction);
    // Ambient light calculation
    vec3 ambientLight = directionalLight.ambient * texture(material.diffuse, textureCoordinates).rgb;

    // Diffuse light calculation
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuseLight = directionalLight.diffuse * diff * texture(material.diffuse, textureCoordinates).rgb;

    vec3 reflectedLightDirection = reflect(-directionalLight.direction, norm);
    float specular = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), material.shininess);
    vec3 specularLight = specular * directionalLight.specular * vec3(texture(material.specular, textureCoordinates));

    return (ambientLight * diffuseLight * specularLight);
}

vec3 calculatePointLight(PointLight pointLight, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
    vec3 lightDirection = normalize(pointLight.position - fragPosition);

    vec3 ambientLight = pointLight.ambient * vec3(texture(material.diffuse, textureCoordinates));

    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuseLight = diff * pointLight.diffuse * vec3(texture(material.diffuse, textureCoordinates));

    vec3 reflectedLightDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(norm, lightDirection), 0.0), material.shininess);
    vec3 specularLight = specular * pointLight.specular * vec3(texture(material.specular, textureCoordinates));

    float distance = length(pointLight.position - fragPosition);
    float attenuation = 1.0 / (pointLight.constant + (pointLight.linear * distance) + pointLight.quadratic * (distance * distance));

    ambientLight *= attenuation;
    diffuseLight *= attenuation;
    specularLight *= attenuation;

    return (ambientLight * diffuseLight * specularLight);
}

vec3 calculateSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
    vec3 lightDirection = normalize(spotLight.position - fragPosition);

    vec3 ambientLight = spotLight.ambient * vec3(texture(material.diffuse, textureCoordinates));

    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuseLight = diff * spotLight.diffuse * vec3(texture(material.diffuse, textureCoordinates));

    vec3 reflectedLightDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), material.shininess);
    vec3 specularLight = specular * spotLight.specular * vec3(texture(material.specular, textureCoordinates));

    float theta = dot(lightDirection, normalize(-spotLight.direction));
    float epsilon = (spotLight.cutOff - spotLight.outerCutOff);
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    diffuseLight *= intensity;
    specularLight *= intensity;

    float distance = length(spotLight.position - fragPosition);
    float attenuation = 1.0 / (spotLight.constant + (spotLight.linear * distance) + spotLight.quadratic * (distance * distance));

    ambientLight *= attenuation;
    diffuseLight *= attenuation;
    specularLight *= attenuation;

    return (ambientLight * diffuseLight * specularLight);
}