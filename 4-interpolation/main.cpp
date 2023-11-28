#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Constants for window dimensions
constexpr GLint WIDTH = 800, HEIGHT = 600;
// Title for the window
std::string TITLE = "Main Window";

// Path to the current working directory
std::string PWD = std::filesystem::current_path().string();

// Paths to the vertex and fragment shader source files
std::string VERTEX_SHADER_PATH = PWD + "/../vShader.glsl";
std::string FRAGMENT_SHADER_PATH = PWD + "/../fShader.glsl";

// OpenGL variables: Vertex array, vertex buffer, shader program, and uniform variable for the model matrix
GLuint vertexArray, vertexBuffer, program, uniformModel;

float scale = 0.4;

// Number of vertices for the triangle
GLuint SQUARE_VERTICES_COUNT = 6;

// Vertex data for the triangle
GLfloat SQUARE[] = 
{
    -1.0f, 1.0f, 0.0f,  // top left
    1.0f, 1.0f, 0.0f,   // top right
    1.0f, -1.0f, 0.0f,  // bottom right
    1.0f, -1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f, // bottom left
    -1.0f, 1.0f, 0.0f,  // top left
};

// Function to create a shape and bind it to OpenGL buffers
void createShape(GLfloat* vertices, GLsizei size)
{
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Function to compile a shader from source code
GLuint compileShader(GLenum type, const std::string& sourceCode)
{
    GLuint shader = glCreateShader(type);
    const GLchar* src = sourceCode.c_str();
    GLint length = sourceCode.size();
    glShaderSource(shader, 1, &src, &length);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        std::cerr << "Failed to compile shader" << std::endl;
        return 0;
    }

    return shader;
}

// Function to read the content of a file into a string
std::string readFile(std::string filename)
{
    std::fstream file(filename, std::ios::in);
    if(!file.is_open())
    {
        std::cerr << "Failed to open file " << filename << std::endl;
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

GLFWwindow* initWindow()
{
    // Initialize GLFW
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Set GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the main window
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(), nullptr, nullptr);
    if(!mainWindow)
    {
        std::cerr << "Failed to create main window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Get framebuffer size
    int widthBuffer, heightBuffer;
    glfwGetFramebufferSize(mainWindow, &widthBuffer, &heightBuffer);
    
    // Make the context of the main window current
    glfwMakeContextCurrent(mainWindow);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return nullptr;
    }

    // Set the viewport size
    glViewport(0, 0, widthBuffer, heightBuffer);
    
    return mainWindow;
}

int main()
{
    GLFWwindow* mainWindow = initWindow();
    if(!mainWindow)
    {
        std::cerr << "Failed to initialise main window" << std::endl;
        return 1;
    }

    // Read shader source code from files
    std::string vertexShaderSource = readFile(VERTEX_SHADER_PATH);
    std::string fragmentShaderSource = readFile(FRAGMENT_SHADER_PATH);

    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Create shader program
    program = glCreateProgram();
    if(!program)
    {
        std::cerr << "Failed to create shader program" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Attach shaders to the program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link the program
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        char error[1024];
        glGetProgramInfoLog(program, sizeof(error), nullptr, error);
        std::cerr << "Program linking error:\n" << error << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Validate the program
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if(!success)
    {
        char error[1024];
        glGetProgramInfoLog(program, sizeof(error), nullptr, error);
        std::cerr << "Program validation error:\n" << error << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Delete shaders as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create the shape (triangle)
    createShape(SQUARE, sizeof(SQUARE));

    // Get the location of the uniform variable in the shader program
    uniformModel = glGetUniformLocation(program, "model");

    while(!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glm::mat4 model(1.0f);
        model = glm::scale(model, scale * glm::vec3(1.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, SQUARE_VERTICES_COUNT);

        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(mainWindow);
    }
}