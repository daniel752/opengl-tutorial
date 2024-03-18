#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

float offset = 1.0 / 400.0;

vec2 offsets[9] = vec2[]
(
    vec2(-offset, offset),   // Top left
    vec2(0.0f, offset),      // Top middle
    vec2(offset, offset),    // Top right
    vec2(-offset, 0.0f),     // Center left
    vec2(0.0f, 0.0f),        // Center middle
    vec2(offset, 0.0f),      // Center right
    vec2(-offset, -offset),  // Bottom left
    vec2(0.0f, -offset),     // Bottom middle
    vec2(offset, -offset)    // Bottom right
);

// KERNEL EFFECTS
// High contrast
float kernel[9] = float[]
(
    2.0, 2.0, 2.0,
    2.0, -15.0, 2.0,
    2.0, 2.0, 2.0
);

// Blur
// float kernel[9] = float[]
// (
//     1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
//     2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
//     1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
// );

// Edge detection 1
// float kernel[9] = float[]
// (
//     -1.0, -1.0, -1.0,
//     -1.0, 9.0, -1.0,
//     -1.0, -1.0, -1.0
// );

// Cartoon
// const mat3 sobelX = mat3(
//     -1, 0, 1,
//     -2, 0, 2,
//     -1, 0, 1
// );
// const mat3 sobelY = mat3(
//     -1, -2, -1,
//      0,  0,  0,
//      1,  2,  1
// );

vec3 sampleTex[9];

void main()
{
    // Normal textures and colors
    // vec3 color = texture(screenTexture, texCoords).rgb;
    // fragColor = vec4(color, 1.0);

    // Inverse color
    // fragColor = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0);

    // Grey scale
    // fragColor = texture(screenTexture, texCoords);
    // float avg = (fragColor.r + fragColor.g + fragColor.b) / 3.0;
    // More accurate grey scale for the human eye
    // float avg = 0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b;
    // fragColor = vec4(avg, avg, avg, 1.0);

    // KERNEL EFFECTS
    for(int i = 0; i < 9; i++)
        sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));

    vec3 color = vec3(0.0);

    for(int i = 0; i < 9; i++)
        color += sampleTex[i] * kernel[i];

    // vec2 texelSize = 1.0 / textureSize(screenTexture, 0);
    // vec3 pixel = texture(screenTexture, texCoords).rgb;
    
    // // Apply Sobel edge detection
    // float edgeX = dot(texture(screenTexture, texCoords - texelSize).rgb, sobelX[0]) +
    //               dot(texture(screenTexture, texCoords).rgb, sobelX[1]) +
    //               dot(texture(screenTexture, texCoords + texelSize).rgb, sobelX[2]);
    
    // float edgeY = dot(texture(screenTexture, texCoords - texelSize).rgb, sobelY[0]) +
    //               dot(texture(screenTexture, texCoords).rgb, sobelY[1]) +
    //               dot(texture(screenTexture, texCoords + texelSize).rgb, sobelY[2]);
    
    // float edgeMagnitude = length(vec2(edgeX, edgeY));
    
    // // Apply color quantization
    // vec3 quantizedColor = round(pixel) / 2.0;
    
    // // Apply contrast enhancement
    // vec3 color = quantizedColor * (1.0 + edgeMagnitude);

    fragColor = vec4(color, 1.0);
}