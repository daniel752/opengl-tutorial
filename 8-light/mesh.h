#pragma once

#include <GL/glew.h>
#include <iostream>


/**
 * @brief Class Mesh handles mesh operations such as creating new mesh, rendering, and clearing mesh.
*/
class Mesh
{
private:
    unsigned int index;
    GLfloat *vertices, *normals, *textureCoordinates;
    int verticesSize, textureCoordinatesSize, indicesSize;
    GLuint *indices;

public:
    /**
     * @brief Constructor to build new Mesh object from vertices and their size.
     * @param vertices Vertices (points) of mesh in space.
     * @param verticesSize Size of vertices in memory.
    */
    Mesh(unsigned int index, GLfloat *vertices, int verticesSize, GLenum type, bool normalize, GLsizei stride);
    /**
     * @brief Destructor.
    */
    ~Mesh();

    // Methods
    /**
     * @brief Initialise mesh object
    */
    void initialise(GLenum type, bool normalize);

    void addAttribute(int index, GLenum target, int vertexSize, GLenum type, bool normalize, void *data, int dataSize, GLuint *buffer);

    void unbind();

    /**
     * @brief Render mesh.
    */
    void render(GLenum mode, GLsizei count, GLenum type, const void *indices);

    void copy(GLenum mode, GLsizei count, GLenum type, const void *indices);
    
    /**
     * @brief Clear mesh.
    */
    void clear();

    // Fields
    GLuint vertexArray, vertexBuffer, normalsBuffer, textureBuffer, elementBuffer;

    // Setters
    void setVertices(GLfloat *newVertices, int newSize) 
    {
        vertices = newVertices;
        verticesSize = newSize;
    }

    void setNormals(GLfloat *newNormals) 
    {
        normals = newNormals;
    }

    void setTextureCoordinates(GLfloat *newTextureCoordinates, int newSize) 
    {
        textureCoordinates = newTextureCoordinates;
        textureCoordinatesSize = newSize;
    }

    void setIndices(GLuint *newIndices, int newSize) 
    {
        indices = newIndices;
        indicesSize = newSize;
    }

    // Getters
    GLfloat* getVertices() const 
    {
        return vertices;
    }

    GLfloat* getNormals() const 
    {
        return normals;
    }

    GLfloat* getTextureCoordinates() const 
    {
        return textureCoordinates;
    }

    GLuint* getIndices() const 
    {
        return indices;
    }

    int getVerticesSize() const 
    {
        return verticesSize;
    }

    int getTextureCoordinatesSize() const 
    {
        return textureCoordinatesSize;
    }

    int getIndicesSize() const 
    {
        return indicesSize;
    }
};