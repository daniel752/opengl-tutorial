#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D diffuse;

void main()
{
    // fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    fragColor = texture(diffuse, texCoords);
}