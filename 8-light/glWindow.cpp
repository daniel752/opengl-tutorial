#include "glWindow.h"

GlWindow::GlWindow()
{
    window = 0;
    title = "Main Window";
    width = 800;
    height = 600;
    isFullscreen = false;

    initialise();
}

GlWindow::GlWindow(std::string title, GLint width, GLint height, bool isFullscreen)
{
    this->title = title;
    this->width = width;
    this->height = height;
    this->isFullscreen = isFullscreen;

    initialise();
}

GlWindow::~GlWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

int GlWindow::initialise()
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

void GlWindow::resizeWindow()
{
    if(!isFullscreen)
    {
        isFullscreen = true;
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_REFRESH_RATE);
    }
    else
    {
        isFullscreen = false;
        glfwSetWindowMonitor(window, NULL, 0, 0, 800, 600, GLFW_REFRESH_RATE);
    }
}

// Define the static member function for the callback
void GlWindow::staticFrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Forward the call to the non-static member function
    static_cast<GlWindow*>(glfwGetWindowUserPointer(window))->frameBufferSizeCallback(window, width, height);
}

// Define the non-static member function for the callback
void GlWindow::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
