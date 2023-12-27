#version 330 core

in vec3 normal;
in vec3 fragPosition;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform float ambientStrength;
uniform float specularIntensity;

void main()
{
    if(ambientStrength == 0) 
    {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0f);
    }
    else
    {
        // Ambient light calculation
        vec3 ambientLight = ambientStrength * lightColor;
        // Diffuse light calculation
        vec3 norm = normalize(normal);
        vec3 lightDirection = normalize(lightPosition - fragPosition);
        float diff = max(dot(norm, lightDirection), 0.0);
        vec3 diffuseLight = diff * lightColor;
        // Specular light calculation
        vec3 viewDirection = normalize(viewPosition - fragPosition);
        vec3 reflectedLightDirection = reflect(-lightDirection, norm);
        float specularLight = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), specularIntensity);
        // Final resulted light
        vec3 result = (specularLight + diffuseLight + ambientLight) * objectColor;
        // Set fragment color
        fragColor = vec4(result, 1.0);
    }
}