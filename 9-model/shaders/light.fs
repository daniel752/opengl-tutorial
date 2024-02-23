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

uniform Material material;
uniform SpotLight spotLight;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUM_LIGHT_POINTS];

vec3 calculateAmbientLight(vec3 ambient, sampler2D diffuse);
vec3 calculateDiffuseLight(vec3 lightDirection, vec3 normal, vec3 lightDiffuse, sampler2D materialDiffuse);
vec3 calculateSpecularLight(vec3 lightDirection, vec3 normal, vec3 viewDirection, vec3 LightSpecular, float materialShininess, sampler2D materialSpecular);
vec3 calculateDirectionLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight pointLight, vec3 normal, vec3 fragPosition, vec3 viewDirection);
vec3 calculateSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPosition, vec3 viewDirection);

void main()
{
    vec3 result = vec3(0.0);

    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 norm = normalize(normal);

    result += calculateDirectionLight(directionalLight, norm, viewDirection);

    for(int i = 0; i < NUM_LIGHT_POINTS; i++)
        result += calculatePointLight(pointLights[i], norm, fragPosition, viewDirection);

    if(spotLight.cutOff > 0)
        result += calculateSpotLight(spotLight, norm, fragPosition, viewDirection);

    fragColor = vec4(result, 1.0);
}

vec3 calculateAmbientLight(vec3 ambient, sampler2D diffuse)
{
    return ambient * vec3(texture(diffuse, textureCoordinates));
}

vec3 calculateDiffuseLight(vec3 lightDirection, vec3 normal, vec3 lightDiffuse, sampler2D materialDiffuse)
{
    lightDirection = normalize(-lightDirection);
    normal = normalize(normal);
    float diff = max(dot(normal, lightDirection), 0.0);
    
    return lightDiffuse * diff * vec3(texture(materialDiffuse, textureCoordinates));
}

vec3 calculateSpecularLight(vec3 lightDirection, vec3 normal, vec3 viewDirection, vec3 LightSpecular, float materialShininess, sampler2D materialSpecular)
{
    normal = normalize(normal);
    vec3 reflectedLightDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), materialShininess);

    return spec * LightSpecular * vec3(texture(materialSpecular, textureCoordinates));
}

vec3 calculateDirectionLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection)
{
    vec3 ambientLight = calculateAmbientLight(directionalLight.ambient, material.diffuse);

    vec3 diffuseLight = calculateDiffuseLight(directionalLight.direction, normal, directionalLight.diffuse, material.diffuse);

    vec3 specularLight = calculateSpecularLight(directionalLight.direction, normal, viewDirection, directionalLight.specular, material.shininess, material.specular);

    return (ambientLight + diffuseLight + specularLight);
}

vec3 calculatePointLight(PointLight pointLight, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
    vec3 ambientLight = calculateAmbientLight(pointLight.ambient, material.diffuse);
    vec3 diffuseLight = calculateDiffuseLight(normalize(pointLight.position - fragPosition), normal, pointLight.diffuse, material.diffuse);
    vec3 specularLight = calculateSpecularLight(normalize(pointLight.position - fragPosition), normal, viewDirection, pointLight.specular, material.shininess, material.specular);

    float distance = length(pointLight.position - fragPosition);
    float attenuation = 1.0 / (pointLight.constant + (pointLight.linear * distance) + pointLight.quadratic * (distance * distance));

    ambientLight *= attenuation;
    diffuseLight *= attenuation;
    specularLight *= attenuation;

    return (ambientLight + diffuseLight + specularLight);
}

vec3 calculateSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
    vec3 ambientLight = calculateAmbientLight(spotLight.ambient, material.diffuse);
    vec3 diffuseLight = calculateDiffuseLight(spotLight.direction, normal, spotLight.diffuse, material.diffuse);
    vec3 specularLight = calculateSpecularLight(spotLight.direction, normal, viewDirection, spotLight.specular, material.shininess, material.specular);

    vec3 lightDirection = normalize(spotLight.position - fragPosition);

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

    return (ambientLight + diffuseLight + specularLight);
}