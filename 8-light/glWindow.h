#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


/**
 * @brief Class GlWindow handles all GLFW window operations.
*/
class GlWindow
{
private:
    GLFWwindow* window;

    std::string title;
    GLint width, height;
    GLint bufferWidth, bufferHeight;

    bool isFullscreen;

public:
    /**
     * @brief Empty Constructor with default window settings.
    */
    GlWindow();
    /**
     * @brief Constructor to build GLFWwindow object according to params.
     * @param title Title of window.
     * @param width Width of window.
     * @param height Height of window.
    */
    GlWindow(std::string title, GLint width, GLint height, bool isFullscreen);
    /**
     * @brief Destructor
    */
    ~GlWindow();

    /**
     * @brief Initialise new window.
    */
    int initialise();
    
    /**
     * @brief Check whether window should close
    */
    bool isShouldClose() { return glfwWindowShouldClose(window); }
    
    /**
     * @brief Swap buffers (front and back buffers)
    */
    void swapBuffers() { glfwSwapBuffers(window); }

    void resizeWindow();

    GLFWwindow* getGlWindow() { return window; }

    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }

    /**
     * @brief Handles resizing of window.
     * @param window Window object.
     * @param width Window width.
     * @param height Window height.
    */
    void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

    static void staticFrameBufferSizeCallback(GLFWwindow* window, int width, int height);
};