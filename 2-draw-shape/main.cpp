#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

// Window dimensions
constexpr GLint WIDTH = 800, HEIGHT = 600;
// Working directory
std::string PWD = std::filesystem::current_path().string();
// Shader sources
std::string VERTEX_SHADER_PATH = PWD + "/../vShader.glsl";
std::string FRAGMENT_SHADER_PATH = PWD + "/../fShader.glsl";

// Global variables
// VAO - vertex array object, VBO - vertex buffer object
// shaderProgram hold the program for shaders
GLuint VAO, VBO, shaderProgram;

// How much vertices the shape has
constexpr GLint TRIANGLE_COUNT = 3;
// Float array for triangle shape, each point has 3 coordinated (x,y,z)
GLfloat TRIANGLE[] = 
{
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
};

// Function to create shape and bind to buffer
void createShape(GLfloat* vertices, GLsizei size)
{
    // Generate vertex arrays - create 1 array and put in VAO
    glGenVertexArrays(1, &VAO);
    // Bind the vertex arrays
    glBindVertexArray(VAO);

    // Generate buffers - create 1 buffer and put in VBO
    glGenBuffers(1, &VBO);
    // bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Give buffer actual data - size of array and vertices (points in space)
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

// Function to compile shader source code (as string)
GLuint compileShader(GLenum type, const std::string& sourceCode)
{
    // Generate shader
    GLuint shader = glCreateShader(type);
    // Get const char* from string source code
    const GLchar* src = sourceCode.c_str();
    // Get size of source code
    GLint length = (int) sourceCode.size();
    // Put source code in shader
    glShaderSource(shader, 1, &src, &length);
    // Compile shader
    glCompileShader(shader);

    GLint success;
    // Initialise shader
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        // If initialisation failed throw error message and return 0;
        char error[512];
        glGetShaderInfoLog(shader, sizeof(error), nullptr, error);
        std::cerr << "Shader compilation error:\n" << error << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// Function to read file - for the shader codes
std::string readFile(std::string filename)
{
    // Create new fstream read-only object from file
    std::fstream file(filename, std::ios::in);
    if(!file.is_open())
    {
        // If failed to open file
        std::cerr << "Failed to open file " << filename << std::endl;
        return nullptr;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

int main()
{
    // Initialise GLFW
    if(!glfwInit())
    {
        std::cout << "GLFW initialisation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile - No backwards compatibility 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Initialise GLFW window
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Main Window", nullptr, nullptr);
    if(!mainWindow)
    {
        std::cout << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialisation failed" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup Shaders
    // Read shader source code
    std::string vertexShaderSource = readFile(VERTEX_SHADER_PATH);
    std::string fragmentShaderSource = readFile(FRAGMENT_SHADER_PATH);

    // Compile shader source code
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    // Create shader program
    shaderProgram = glCreateProgram();
    if(!shaderProgram)
    {
        std::cerr << "Error creating program" << std::endl;
        return 1;
    }

    // Attach shader to program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    // Check for linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        char error[1024];
        glGetProgramInfoLog(shaderProgram, sizeof(error), nullptr, error);
        std::cerr << "Shader program linking error:\n" << error << std::endl;
        return 1;
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if(!success)
    {
        char error[1024];
        glGetProgramInfoLog(shaderProgram, sizeof(error), nullptr, error);
        std::cerr << "Shader program linking error:\n" << error << std::endl;
        return 1;
    }

    // Delete shader (after they are linked into the program they are no longer needed)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Create triangle shape
    createShape(TRIANGLE, sizeof(TRIANGLE));

    // Main loop
    // Loop until window closed
    while(!glfwWindowShouldClose(mainWindow))
    {
        // Get + Handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_COUNT);

        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
