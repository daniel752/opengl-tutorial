#include "shader.h"

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

// Function to read the content of a file into a string
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
