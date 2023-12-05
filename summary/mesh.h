#include <GL/glew.h>

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void create(GLfloat* vertices, unsigned int* indices, unsigned int verticesSize, unsigned int indicesSize);
    void render();
    void clear();

private:
    GLuint vertexArray, vertexBuffer, indexBuffer, indexCount;
};