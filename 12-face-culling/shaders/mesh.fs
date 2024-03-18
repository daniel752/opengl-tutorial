#version 330 core

out vec4 fragColor;

float near = 1.0;
float far = 10.0;

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = linearizeDepth(gl_FragCoord.z) / far;
    fragColor = vec4(vec3(1.0 - depth), 1.0);
}