#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D textures[];

void main()
{
    fragColor = mix(texture(textures[0], texCoord), texture(textures[1], vec2(texCoord.y, 1.0 - texCoord.x)), 0.5);
}