#include "glWindow.h"
#include "shader.h"
#include "mesh.h"


// Window dimensions
constexpr GLint WIDTH = 800, HEIGHT = 600;
// Working directory
std::string PWD = std::filesystem::current_path().string();
// Shader sources
std::string VERTEX_SHADER_PATH = PWD + "/../shaders/vShader.glsl";
std::string FRAGMENT_SHADER_PATH = PWD + "/../shaders/fShader.glsl";

// How much vertices the shape has
constexpr GLint TRIANGLE_COUNT = 3;
// Float array for triangle shape, each point has 3 coordinated (x,y,z)
GLfloat TRIANGLE[] = 
{
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
};

int main()
{
    glWindow window;
    if (!window.initialise()) {
        std::cerr << "Failed to initialize the window." << std::endl;
        return -1;
    }

    // Setup Shaders
    // Create new shader object
    Shader shader;
    // Create shader program from shader files (vertex shader, fragment shader)
    shader.createFromFiles(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());

    // Create new Mesh object to handle triangle shape
    Mesh mesh;
    mesh.create(TRIANGLE, sizeof(TRIANGLE));

    // Main loop
    // Run until window should close
    while (!window.isShouldClose())
    {
        // Clear window to black screen
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // User shader program with our triangle with shaders
        shader.useShader();
        // Render triangle
        mesh.render();
        // Unbind shader program
        glUseProgram(0);
        // OpenGL uses a double buffer (front and back buffers) technique for rendering
        // All of the rendering commands will go to back buffer
        // After all of the rendering commands are done we switch to the front buffer which has the final output image
        // If we don't do this we can see flickering in output image, cause the image is build pixel by pixel from left to right and from top to bottom
        // So basically we'll see the image being build and that's why will see the flickering (if image is small and not complex than it probably won't matter)
        window.swapBuffers();
        // Get + Handle glfwWindow I/O
        glfwPollEvents();
    }

    return 0;
}
