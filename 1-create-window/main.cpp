#include "glWindow.h"

#include <iostream>
#include <GL/gl.h>

// Window dimensions
constexpr GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    glWindow window;
    window.initialise();

    // Loop until window closed
    while(!window.isShouldClose())
    {
        // Clear window
        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        window.swapBuffers();
        // Get + Handle user input events
        glfwPollEvents();
    }

    return 0;
}
