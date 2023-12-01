#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    Window(std::string windowTitle, GLint windowWidth, GLint windowHeight);
    ~Window();

    int initialise();

    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }

    bool isShouldClose() { return glfwWindowShouldClose(mainWindow); }
    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    GLFWwindow* getMainWindow() { return mainWindow; }

private:
    GLFWwindow* mainWindow;

    std::string title;
    GLint width, height;
    GLint bufferWidth, bufferHeight;

};