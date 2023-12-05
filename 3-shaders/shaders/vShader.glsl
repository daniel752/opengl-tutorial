#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

void main()
{
    gl_Position = vec4(0.4 * aPos.x, 0.4 * aPos.y, aPos.z, 1.0);
    
    if(all(equal(aColor, vec3(0.0))))
        color = vec3(0.0, 0.0, 1.0);
    else
        color = aColor;
        
}