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
    Mesh(GLfloat* vertices, int verticesSize, GLenum type, bool normalize, GLsizei stride, GLvoid** pointers);
    /**
     * @brief Destructor.
    */
    ~Mesh();

    // Methods
    /**
     * @brief Color mesh.
    */
    void color(GLfloat* vertices, int verticesSize);
    /**
     * @brief Render mesh.
    */
    void render();
    /**
     * @brief Clear mesh.
    */
    void clear();

private:
    GLuint vertexArray, vertexBuffer;
};