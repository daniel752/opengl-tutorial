#include "glWindow.h"

glWindow::glWindow()
{
    window = 0;
    title = "Main Window";
    width = 800;
    height = 600;

    initialise();
}

glWindow::glWindow(std::string title, GLint width, GLint height)
{
    this->title = title;
    this->width = width;
    this->height = height;

    initialise();
}

glWindow::~glWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

int glWindow::initialise()
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
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!window)
    {
        std::cerr << "Failed to create main window" << std::endl;
        glfwTerminate();
        return GL_FALSE;
    }

    // Get framebuffer size
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    
    // Make the context of the main window current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return GL_FALSE;
    }

    // Set the viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSetFramebufferSizeCallback(window, staticFrameBufferSizeCallback);

    return GL_TRUE;
}

// Define the static member function for the callback
void glWindow::staticFrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Forward the call to the non-static member function
    static_cast<glWindow*>(glfwGetWindowUserPointer(window))->frameBufferSizeCallback(window, width, height);
}

// Define the non-static member function for the callback
void glWindow::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
