#include "shader.h"
#include "stb_image.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if(geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();

        }
    }
    catch(std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;       
    }
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Create shader program
    ID = glCreateProgram();

    GLuint vShader = addShader(ID, vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fShader = addShader(ID, fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    GLuint gShader  = 0;
    if(geometryPath != nullptr)
    {
        gShader = addShader(ID, geometryCode.c_str(), GL_GEOMETRY_SHADER);
    }
    glLinkProgram(ID);
    checkCompileErrors(ID, GL_PROGRAM);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    if(geometryPath != nullptr)
    {
        glDeleteShader(gShader);
    }
}

Shader::~Shader()
{
    clear();
}

std::string Shader::readFile(std::string filename)
{
    std::fstream file(filename, std::ios::in);
    if(!file.is_open())
    {
        std::cerr << "Failed to open file " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

void Shader::loadTexture(std::string filename, GLuint *texture, GLenum target, GLenum textureParam, GLenum filterParam, GLint level, GLint internalFormat, GLint border, GLint format, GLenum type)
{
    glGenTextures(1, texture);
    glBindTexture(target, *texture);
    // set the texture wrapping parameters
    glTexParameteri(target, GL_TEXTURE_WRAP_S, textureParam);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(target, GL_TEXTURE_WRAP_T, textureParam);
    // set texture filtering parameters
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filterParam);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filterParam);

    
    // stbi_set_flip_vertically_on_load(true);
    // Loading texture from local storage
    int width, height, nrChannels;
    unsigned char* data = nullptr;
    data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if(!data)
    {
        std::cout << "Failed to load texture" << std::endl;
        return;
    }
    glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
    glGenerateMipmap(target);

    free(data);
}

void Shader::unbind()
{
    glUseProgram(0);
}

GLuint Shader::addShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    // std::cout << "\nCompiling code:\n" << shaderCode;
    const GLchar* src = shaderCode;
    GLint length = strlen(shaderCode);
    glShaderSource(shader, 1, &src, &length);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderType);

    glAttachShader(program, shader);

    return shader;
    // glDeleteShader(shader);
}

void Shader::checkCompileErrors(GLuint shader, GLenum type)
{
    GLint success;
    GLchar infoLog[1024];
    if(type != GL_PROGRAM)
    {
        glGetShaderiv(shader,GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}


void Shader::use()
{
    glUseProgram(ID);
}

void Shader::clear()
{
    if(ID != 0)
    {
        glDeleteProgram(ID);
        ID = 0;
    }
}

void Shader::enableDepth()
{
    glEnable(GL_DEPTH_TEST);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const GLfloat* vector) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, vector);
}

void Shader::setVec3(const std::string &name, const GLfloat* vector) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, vector);
}

void Shader::setMatrix3fv(const std::string &name, GLsizei count, GLboolean transpose, glm::mat4 matrix) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), count, transpose, glm::value_ptr(matrix));
}

void Shader::setMatrix4fv(const std::string &name, GLsizei count, GLboolean transpose, glm::mat4 matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), count, transpose, glm::value_ptr(matrix));
}
