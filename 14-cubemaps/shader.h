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
    GLuint ID; // Holds shader program ID
    /**
     * @brief Costructor to build Shader object from shader files.
     * @param vertexPath Path to vertex shader file.
     * @param fragmentPath Path to fragment shader file.
    */
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    /**
     * @brief Destructor
    */
    ~Shader();

    /**
     * @brief Read file from filesystem.
     * @param filename Path to file.
    */
    std::string readFile(std::string filename);

    /**
     * @brief Use this object's shader program.
    */
    void use();
    void unbind();
    /**
     * @brief Clear shader program.
    */
    void clear();

    static void enableGL(GLenum func);
    static void disableGL(GLenum func);
    
    // OpenGL depth test functions
    static void depthMask(bool mask);
    static void depthFunctionality(GLenum functionality);

    // OpenGL stencil test functions
    static void stencilMask(unsigned int mask);
    static void stencilFunctionality(GLenum functionality, GLint reference, GLuint mask);
    static void stencilOperation(GLenum sFail, GLenum dpFail, GLenum dpPass);

    GLuint getID() const { return ID; }

    // Utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string &name, const GLfloat* vector) const;
    void setVec3(const std::string &name, const GLfloat* vector) const;
    void setMatrix3fv(const std::string &name, GLsizei count, GLboolean transpose, glm::mat4 matrix) const;
    void setMatrix4fv(const std::string &name, GLsizei count, GLboolean transpose, glm::mat4 matrix) const;

private:
    /**
     * @brief Add shader to shader program.
     * @param program Shader program to add shader to.
     * @param shaderCode Shader source code to add.
     * @param shaderType Specifies if it's vertex or fragment shader.
    */
    GLuint addShader(GLuint program, const char* shaderCode, GLenum shaderType);

    void checkCompileErrors(GLuint shader, GLenum type);
};