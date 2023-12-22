#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    fragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(texCoord.y, 1.0 - texCoord.x)), 0.5);
}