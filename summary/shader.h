#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    GLuint shaderID,  uniformProjection, uniformModel;

    void compileShader(const char* vertexCode, const char* fragmentCode);
    void addShader(GLuint program, const char* shaderCode, GLenum shaderType);
};