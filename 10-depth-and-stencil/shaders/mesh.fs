#version 330 core

out vec4 fragColor;

float near = 1.0;
float far = 50.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    // float depth = LinearizeDepth(gl_FragCoord.z) / far;
    // fragColor = vec4(vec3(depth), 1.0);
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}