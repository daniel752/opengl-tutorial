#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fragPosition;
out vec3 normal;
// Comment this line if you want to switch to Phong lighting in world space
out vec3 lightPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
// Comment this line if you want to switch to Phong lighting in world space
uniform vec3 lightPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // Comment this line and uncomment line after this to switch Phong lighting in world space
    fragPosition = vec3(view * model * vec4(aPos, 1.0));
    // fragPosition = vec3(model * vec4(aPos, 1.0));
    normal = normalMatrix * aNormal;
    // Comment this line to switch Phong lighting in world space
    lightPosition = vec3(view * vec4(lightPos, 1.0));
}