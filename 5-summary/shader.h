#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

class Shader
{
public:
    Shader();
    ~Shader();

    std::string readFile(std::string filename);
    void createFromFiles(const char* vertexLocation, const char* fragmentLocation);
    void useShader();
    void clearShader();
    GLuint getProjectionLocation();
    GLuint getModelLocation();

private:
    GLuint shaderID,  uniformProjection, uniformModel;

    void compileShader(const char* vertexCode, const char* fragmentCode);
    void addShader(GLuint program, const char* shaderCode, GLenum shaderType);
};