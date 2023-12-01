#version 330 core

// Input variable: interpolated vertex color
in vec4 vColor;

// Output variable: final fragment color
out vec4 fragColor;

void main()
{
    // Assign the interpolated vertex color to the fragment color
    fragColor = vColor;
}
