#version 330 core

out vec4 fragColor;

in vec3 normal;
in vec3 position;

uniform samplerCube skybox;
uniform vec3 cameraPosition;

void main()
{
    // Reflection
    // ---------------------------------------------
    // vec3 I = normalize(position - cameraPosition);
    // vec3 R = reflect(I, normalize(normal));
    // fragColor = vec4(texture(skybox, R).rgb, 1.0);
    
    // Refraction
    // ---------------------------------------------
    vec3 I = normalize(position - cameraPosition);
    const float airRefraction = 1.0;
    const float glassRefraction = 1.52;
    vec3 R = refract(I, normalize(normal), airRefraction / glassRefraction);
    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}