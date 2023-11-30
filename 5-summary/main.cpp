#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>

#include "mesh.h"
#include "shader.h"

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

// GLuint program, uniformModel, uniformProjection;
std::vector<Mesh*> meshes;
std::vector<Shader> shaders;

float scale = 0.4;

const float TO_RADIANS = 3.14159 / 180;
float angle = 0.0;
float rotationSpeed = 1.0;

GLfloat projectionX = 0.0f, projectionY = 0.0f, projectionZ = -7.0f;

unsigned int indices[] = 
{
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
};

// Vertex data for the triangle
GLfloat TRIANGLE1[] = 
{
    -1.0f, -1.0f, 0.0f,     // down left
    0.0f, -1.0f, 1.0f,       // far middle
    1.0f, -1.0f, 0.0f,       // down right
    0.0f, 1.0f, 0.0f        // top middle
};

GLfloat TRIANGLE2[] = 
{
    -6.0f, -1.0f, 0.0f,     // down left
    -5.0f, -1.0f, 1.0f,       // far middle
    -4.0f, -1.0f, 0.0f,       // down right
    -5.0f, 1.0f, 0.0f        // top middle
};

GLfloat TRIANGLE3[] = 
{
    4.0f, -1.0f, 0.0f,     // down left
    5.0f, -1.0f, 1.0f,       // far middle
    6.0f, -1.0f, 0.0f,       // down right
    5.0f, 1.0f, 0.0f        // top middle
};

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

Mesh* createShape(int index)
{
    Mesh* mesh = new Mesh();
    switch(index)
    {
        case 1:
            mesh->create(TRIANGLE1, indices, 12, 12);
            break;
        
        case 2:
            mesh->create(TRIANGLE2, indices, 12, 12);
            break;
        
        case 3:
            mesh->create(TRIANGLE3, indices, 12, 12);
            break;
    }

    return mesh;
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

    glEnable(GL_DEPTH_TEST);

    // Set the viewport size
    glViewport(0, 0, widthBuffer, heightBuffer);
    
    return mainWindow;
}

int main()
{
    GLFWwindow* window = initWindow();

    Shader* shader = new Shader();
    shader->createFromFiles(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());
    shaders.push_back(*shader);

    for(int i = 1; i <= 3; i++)
        meshes.push_back(createShape(i));

    glm::mat4 projection = glm::perspective(45.0f,  (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
    projection = glm:: translate(projection, glm::vec3(projectionX, projectionY, projectionZ));

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glUseProgram(program);
        shaders[0].useShader();

        glm::mat4 model(1.0f);

        angle += rotationSpeed;

        if(angle == 360)
            angle = 0;

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.5f));
        model = glm::rotate(model, angle * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(shaders[0].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shaders[0].getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));

        for(auto& mesh : meshes)
            mesh->render();

        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    // Cleanup and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}