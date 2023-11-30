#include "mesh.h"

Mesh::Mesh()
{
    vertexArray = 0;
    vertexBuffer = 0;
    indexBuffer = 0;
    indexCount = 0;
}

Mesh::~Mesh()
{
    clear();
}

void Mesh::create(GLfloat* vertices, unsigned int* indices, unsigned int verticesSize, unsigned int indicesSize)
{
    indexCount = indicesSize;

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indicesSize, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * verticesSize, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::render()
{
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    // Draw by elements according to 'indexBuffer'
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::clear()
{
    if(indexBuffer != 0)
    {
        glDeleteBuffers(1, &indexBuffer);
        indexBuffer = 0;
    }

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

    indexCount = 0;
}