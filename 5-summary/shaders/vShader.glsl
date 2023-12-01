#version 330 core

// Input attribute: vertex position
layout (location = 0) in vec3 pos;

// Output variable: vertex color
out vec4 vColor;

// Uniform variables: 
// 4x4 model matrix for transformations
uniform mat4 model;
// 4x4 model matrix for projection
uniform mat4 projection;

void main()
{
    // Transform vertex position using the model matrix
    gl_Position = projection * model * vec4(pos, 1.0);

    // Assign vertex color based on the clamped vertex position
    // clamp will make every value from pos to be between 0.0f and 1.0f
    vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
}
