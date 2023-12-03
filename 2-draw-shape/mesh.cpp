#include "mesh.h"

Mesh::Mesh()
{
    vertexArray = 0;
    vertexBuffer = 0;
}

Mesh::~Mesh()
{
    clear();
}

void Mesh::create(GLfloat* vertices, int verticesSize)
{
    // Generate 1 vertex array (VAO)
    glGenVertexArrays(1, &vertexArray);
    // Generate 1 vertex buffer (VBO)
    glGenBuffers(1, &vertexBuffer);
    
    // Bind vertex array and vertex buffer so we can create the actual mesh data
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    // First param we're telling program to look for a vertex attribute at index 0 (layout = 0 in vertex shader)
    // Second param specifies how many components each vertex has, meaning how many axes ( X | Y | Z ) are involved, can be from 1-4 (1D, 2D, 3D, 4D)
    // Third param specifies the type of data for each component in array (int, float, etc.)
    // Fourth param specifies whether we want to normalize data (clamped to range of -1 to 1 for signed values and 0 to 1 for unsigned values)
    // Fifth param specifies stride (specifies the byte offset between consecutive vertex attributes), if stride is 0 than there's is no gap between vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Enable generic vertex attribute array for vertex attribute at index 0
    glEnableVertexAttribArray(0);

    // Unbind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind vertex array
    glBindVertexArray(0);
}

void Mesh::render()
{
    // Bind vertex array
    glBindVertexArray(vertexArray);
    // Draw vertex array
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Unbind vertex array
    glBindVertexArray(0);
}

void Mesh::clear()
{
    if(vertexBuffer != 0)
    {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = 0;
    }

    if(vertexArray != 0)
    {
        glDeleteBuffers(1, &vertexArray);
        vertexArray = 0;
    }
}