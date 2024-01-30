#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoordinates;

out vec3 fragPosition;
out vec3 normal;
out vec2 textureCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    fragPosition = vec3(model * vec4(aPos, 1.0));

    normal = normalMatrix * aNormal;

    textureCoordinates = aTextureCoordinates;
    
    gl_Position = projection * view * vec4(fragPosition, 1.0);
}