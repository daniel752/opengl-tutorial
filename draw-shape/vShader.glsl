#version 330 core

layout (location = 0) in vec3 pos;
out vec4 color;

void main()
{
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);
    color = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);
}