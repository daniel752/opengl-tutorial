#version 330 core

// in vec2 texCoords;

out vec4 fragColor;

uniform vec3 color;
// uniform sampler2D texture_diffuse1;

void main()
{
    // fragColor = texture(texture_diffuse1, texCoords);
    fragColor = vec4(color, 1.0);
}