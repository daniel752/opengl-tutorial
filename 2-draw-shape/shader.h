# pragma once

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


/**
 * @brief Class Shader handles all shader program operations such as reading shaders from files, compiling shaders, and creating new shader program.
*/
class Shader
{
public:
    /**
     * @brief Constructor
    */
    Shader();
    /**
     * @brief Destructor
    */
    ~Shader();

    /**
     * @brief Read file from filesystem.
    */
    std::string readFile(std::string filename);
    
    /**
     * @brief Create shader program from shader files.
     * @param vertexLocation Path to vertex shader.
     * @param fragmentLocation Path to fragment shader. 
    */
    void createFromFiles(const char* vertexLocation, const char* fragmentLocation);
    /**
     * @brief Use this object's shader program.
    */
    void useShader();
    /**
     * @brief Clear shader program.
    */
    void clearShader();

private:
    GLuint shaderID; // Holds shader program ID

    /**
     * @brief Compile vertex and fragment shaders.
     * @param vertexCode Vertex shader source code.
     * @param fragmentCode Fragment shader source code.
    */
    void compileShader(const char* vertexCode, const char* fragmentCode);
    /**
     * @brief Add shader to shader program.
     * @param program Shader program to add shader to.
     * @param shaderCode Shader source code to add.
     * @param shaderType Specifies if it's vertex or fragment shader.
    */
    void addShader(GLuint program, const char* shaderCode, GLenum shaderType);
};