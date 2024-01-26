#version 330 core

#define NUM_TEXTURES 2

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D textures[NUM_TEXTURES];

void main()
{
    fragColor = mix(texture(textures[0], texCoord), texture(textures[1], vec2(texCoord.y, texCoord.x)), 0.5);
}