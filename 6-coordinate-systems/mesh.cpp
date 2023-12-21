#include "mesh.h"

Mesh::Mesh(GLfloat* vertices, int verticesSize, GLfloat* colors, int colorsSize, GLfloat* textureCoordinates, int textureCoordinatesSize, unsigned int *indices, int indicesSize, GLenum type, bool normalize, GLsizei stride) :
    vertices(vertices), verticesSize(verticesSize), // Initialise vertices
    colors(colors), colorsSize(colorsSize), // Initialise colors
    textureCoordinates(textureCoordinates), textureCoordinatesSize(textureCoordinatesSize), // Initialise texture coordinates
    indices(indices), indicesSize(indicesSize),  // Initialise indices (elements)
    vertexArray(0),
    vertexBuffer(0),
    colorBuffer(0),
    textureBuffer(0),
    elementBuffer(0)    
{

}

Mesh::~Mesh()
{
    clear();
    vertices = nullptr;
    colors = nullptr;
    textureCoordinates = nullptr;
    indices = nullptr;
}

void Mesh::initialise(GLenum type, bool normalize)
{
    // Generate 1 vertex array (VAO)
    glGenVertexArrays(1, &vertexArray);
    // Generate 1 vertex buffer (VBO)
    glGenBuffers(1, &vertexBuffer);
    // Generate 1 element buffer (EBO)
    // glGenBuffers(1, &elementBuffer);
    
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
}

void Mesh::addAttribute(int index, GLenum target, int vertexSize, GLenum type, bool normalize, void *data, int dataSize, GLuint *buffer = nullptr)
{
    if(vertexArray == 0)
        initialise(type, normalize);
    
    if(buffer)
        glGenBuffers(1, buffer);

    glBindBuffer(target, *buffer);
    glBufferData(target, dataSize, data, GL_STATIC_DRAW);
    
    if(target == GL_ARRAY_BUFFER)
    {
        glVertexAttribPointer(index, vertexSize, type, normalize, 0, 0);
        glEnableVertexAttribArray(index);
    }
}

void Mesh::unbind()
{
    // Unbind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind vertex array
    glBindVertexArray(0);
}

void Mesh::render(GLenum mode, GLsizei count, GLenum type, const void *indices)
{
    // Bind vertex array
    glBindVertexArray(vertexArray);
    // Draw vertex array according to indices (elements)
    if(indices)
        glDrawElements(mode, count, type, indices);
    else
        glDrawArrays(mode, 0, count);
}

void Mesh::copy(GLenum mode, GLsizei count, GLenum type, const void *indices)
{
    // Draw vertex array according to indices (elements)
    glDrawElements(mode, count, type, indices);
}

void Mesh::clear()
{
    if(vertexArray != 0)
    {
        glDeleteVertexArrays(1, &vertexArray);
        vertexArray = 0;
    }

    if(vertexBuffer != 0)
    {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = 0;
    }

    if(colorBuffer != 0)
    {
        glDeleteBuffers(1, &colorBuffer);
        colorBuffer = 0;
    }

    if (textureBuffer != 0)
    {
        glDeleteBuffers(1, &textureBuffer);
        textureBuffer = 0;
    }
    
    if(elementBuffer != 0)
    {
        glDeleteBuffers(1, &elementBuffer);
        elementBuffer = 0;
    }
}