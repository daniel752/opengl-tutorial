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
     * @brief Constructor
    */
    Mesh();
    /**
     * @brief Destructor
    */
    ~Mesh();

    // Methods
    /**
     * Create new mesh.
     * @param vertices Vertices (points) of mesh in space.
     * @param verticesSize Size of vertices in memory.
    */
    void create(GLfloat* vertices, int verticesSize);
    /**
     * Render mesh
    */
    void render();
    /**
     * Clear mesh
    */
    void clear();

private:
    GLuint vertexArray, vertexBuffer;
};