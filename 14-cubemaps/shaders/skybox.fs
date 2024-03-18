#version 330 core

out vec4 fragColor;

in vec3 textureCoordinates;

uniform samplerCube skybox;

void main()
{
    fragColor = texture(skybox, textureCoordinates);
}