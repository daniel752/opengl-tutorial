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
    Mesh(GLfloat* vertices, int verticesSize, GLfloat* colors, int colorsSize, GLfloat* textureCoordinates, int textureCoordinatesSize, unsigned int* indices, int indicesSize, GLenum type, bool normalize, GLsizei stride);
    /**
     * @brief Destructor.
    */
    ~Mesh();

    // Methods
    /**
     * @brief Render mesh.
    */
    void render();
    /**
     * @brief Clear mesh.
    */
    void clear();

private:
    GLuint vertexArray, vertexBuffer, elementBuffer, colorBuffer, textureBuffer;
    // unsigned int* indices;
};