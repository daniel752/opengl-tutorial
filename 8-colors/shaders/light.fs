#version 330 core

out vec4 fragColor;

uniform vec3 color;
uniform float strength;

void main()
{
    fragColor = vec4(color * strength, 1.0);
}