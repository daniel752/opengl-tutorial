#pragma once

#include <GL/glew.h>
#include <iostream>


/**
 * @brief Class Mesh handles mesh operations such as creating new mesh, rendering, and clearing mesh.
*/
class Mesh
{
public:
    /**
     * @brief Constructor to build new Mesh object from vertices and their size.
     * @param vertices Vertices (points) of mesh in space.
     * @param verticesSize Size of vertices in memory.
    */
    Mesh(unsigned int index, GLfloat *vertices, int verticesSize, GLfloat *colors, int colorsSize, GLfloat *textureCoordinates, int textureCoordinatesSize, unsigned int *indices, int indicesSize, GLenum type, bool normalize, GLsizei stride);
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
    GLuint vertexArray, vertexBuffer, colorBuffer, textureBuffer, elementBuffer;

private:
    unsigned int index;
    GLfloat *vertices, *colors, *textureCoordinates;
    int verticesSize, colorsSize, textureCoordinatesSize, indicesSize;
    GLuint *indices;
};