#include "shader.h"
#include "stb_image.h"


Shader::Shader(const char* vertexLocation, const char* fragmentLocation)
{
    ID = 0;

    // Read shader source code from files
    std::string vertexShaderSource = readFile(vertexLocation);
    std::string fragmentShaderSource = readFile(fragmentLocation);

    compileShader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
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

void Shader::loadTexture(std::string filename, GLuint *texture, GLenum target, GLint level, GLint internalFormat, GLint border, GLint format, GLenum type)
{
    glGenTextures(1, texture);
    glBindTexture(target, *texture);
    // set the texture wrapping parameters
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
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

void Shader::addShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const GLchar* src = shaderCode;
    GLint length = strlen(shaderCode);
    glShaderSource(shader, 1, &src, &length);
    glCompileShader(shader);

    GLint success = 0;
    GLchar error[1024] = { 0 };
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        std::cerr << "Failed to compile shader" << std::endl;
        glGetShaderInfoLog(shader, sizeof(error), nullptr, error);
        std::cerr << "Error:\n" << error << std::endl;
        return;
    }

    glAttachShader(program, shader);

    glDeleteShader(shader);
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode)
{
    // Create shader program
    ID = glCreateProgram();
    if(!ID)
    {
        std::cerr << "Failed to create shader program" << std::endl;
        return;
    }

    addShader(ID, vertexCode, GL_VERTEX_SHADER);
    addShader(ID, fragmentCode, GL_FRAGMENT_SHADER);

    // Link the program
    glLinkProgram(ID);
    GLint success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        char error[1024];
        glGetProgramInfoLog(ID, sizeof(error), nullptr, error);
        std::cerr << "Program linking error:\n" << error << std::endl;
        return;
    }

    // Validate the program
    glValidateProgram(ID);
    glGetProgramiv(ID, GL_VALIDATE_STATUS, &success);
    if(!success)
    {
        char error[1024];
        glGetProgramInfoLog(ID, sizeof(error), nullptr, error);
        std::cerr << "Program validation error:\n" << error << std::endl;
        return;
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
