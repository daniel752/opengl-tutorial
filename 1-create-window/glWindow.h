#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class glWindow
{
public:
    glWindow();
    glWindow(std::string windowTitle, GLint windowWidth, GLint windowHeight);
    ~glWindow();

    int initialise();

    bool isShouldClose() { return glfwWindowShouldClose(window); }
    
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
    void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};