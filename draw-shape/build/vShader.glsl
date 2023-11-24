#version 330 core

layout (location = 0) in vec3 pos;

uniform float xMove;

void main()
{
    gl_Position = vec4(0.4 * pos.x + xMove, 0.4 * pos.y, pos.z, 1.0);
}