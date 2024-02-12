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
     * @brief Load texture from image in local storage
     * @param filename Path to image file.
     * @param texture Pointer to texture variable to bind texture image to.
     * @param target Specifies which target to apply operations (GL_TEXTURE_2D, GL_ARRAY_BUFFER, etc.).
     * @param level Specifies level of detail, level 0 is base image and level n is nth mimap image reduction.
     * @param internalFormat Specifies number of colors components in texture.
     * @param border This value must be 0.
     * @param format Specifies the format of pixel data from texture image.
     * @param type Specifies the data type of pixel data from texture image.
    */
    void loadTexture(std::string filename, GLuint *texture, GLenum target, GLenum textureParam, GLenum filterParam, GLint level, GLint internalFormat, GLint border, GLint format, GLenum type);
    
    /**
     * @brief Use this object's shader program.
    */
    void use();
    void unbind();
    /**
     * @brief Clear shader program.
    */
    void clear();

    void static enableDepth();

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