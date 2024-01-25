#version 330 core

#define NUM_TEXTURES 2

in vec3 color;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D textures[NUM_TEXTURES];
uniform float mixValue;

void main()
{
    fragColor = mix(texture(textures[0], vec2(texCoord.y, 1.0 - texCoord.x)), texture(textures[1], texCoord), mixValue) * vec4(color, 1.0);
}