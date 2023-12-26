#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


/**
 * @brief Class glWindow handles all GLFW window operations.
*/
class glWindow
{
public:
    /**
     * @brief Empty Constructor with default window settings.
    */
    glWindow();
    /**
     * @brief Constructor to build GLFWwindow object according to params.
     * @param title Title of window.
     * @param width Width of window.
     * @param height Height of window.
    */
    glWindow(std::string title, GLint width, GLint height);
    /**
     * @brief Destructor
    */
    ~glWindow();

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

    GLFWwindow* getGlWindow() { return window; }

    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }

private:
    GLFWwindow* window;

    std::string title;
    GLint width, height;
    GLint bufferWidth, bufferHeight;

    static void staticFrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    /**
     * @brief Handles resizing of window.
     * @param window Window object.
     * @param width Window width.
     * @param height Window height.
    */
    void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};