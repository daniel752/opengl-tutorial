#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture1;

void main()
{
    vec4 textureColor = texture(texture1, texCoords);
    // If texture color's alpha is less than 0.1 (barely visible anyways), discard current fragment
    if(textureColor.a < 0.1)
        discard;
    fragColor = textureColor;
}