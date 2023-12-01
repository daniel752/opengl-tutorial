#include "window.h"

Window::Window()
{
    mainWindow = 0;
    title = "Main Window";
    width = 800;
    height = 600;
}

Window::Window(std::string windowTitle, GLint windowWidth, GLint windowHeight)
{
    title = windowTitle;
    width = windowWidth;
    height = windowHeight;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

int Window::initialise()
{
    // Initialize GLFW
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        return GL_FALSE;
    }

    // Set GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the main window
    mainWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!mainWindow)
    {
        std::cerr << "Failed to create main window" << std::endl;
        glfwTerminate();
        return GL_FALSE;
    }

    // Get framebuffer size
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    
    // Make the context of the main window current
    glfwMakeContextCurrent(mainWindow);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return GL_FALSE;
    }

    glEnable(GL_DEPTH_TEST);

    // Set the viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    return GL_TRUE;
}

