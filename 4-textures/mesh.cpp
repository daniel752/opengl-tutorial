#include "mesh.h"

Mesh::Mesh(GLfloat* vertices, int verticesSize, GLfloat* colors, int colorsSize, GLfloat* textureCoordinates, int textureCoordinatesSize, unsigned int* indices, int indicesSize, GLenum type, bool normalize = GL_FALSE, GLsizei stride = 0)
{
    // Generate 1 vertex array (VAO)
    glGenVertexArrays(1, &vertexArray);
    // Generate 1 vertex buffer (VBO)
    glGenBuffers(1, &vertexBuffer);
    // Generate 1 element buffer (EBO)
    glGenBuffers(1, &elementBuffer);
    glGenBuffers(1, &colorBuffer);
    glGenBuffers(1, & textureBuffer);
    
    // Bind vertex array and vertex buffer so we can create the actual mesh data
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
    // First param we're telling program to look for a vertex attribute at index 0 (layout = 0 in vertex shader)
    // Second param specifies how many components each vertex has, meaning how many axes ( X | Y | Z ) are involved, can be from 1-4 (1D, 2D, 3D, 4D)
    // Third param specifies the type of data for each component in array (int, float, etc.)
    // Fourth param specifies whether we want to normalize data (clamped to range of -1 to 1 for signed values and 0 to 1 for unsigned values)
    // Fifth param specifies stride (specifies the byte offset between consecutive vertex attributes), if stride is 0 than there's is no gap between vertex attributes
    glVertexAttribPointer(0, 3, type, normalize, 0, 0);
    // Enable generic vertex attribute array for vertex attribute at index 0
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colorsSize, colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, type, normalize, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, textureCoordinatesSize, textureCoordinates, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, type, normalize, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
    // Unbind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind vertex array
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    clear();
}


void Mesh::render()
{
    // Bind vertex array
    glBindVertexArray(vertexArray);
    // Draw vertex array according to indices (elements)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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