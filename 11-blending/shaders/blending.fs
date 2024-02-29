#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture1;

void main()
{
    fragColor = texture(texture1, texCoords);;
}