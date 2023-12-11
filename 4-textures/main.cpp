#include "glWindow.h"
#include "shader.h"
#include "mesh.h"
#include "stb_image.h"


// Window dimensions
constexpr GLint WIDTH = 800, HEIGHT = 600;
std::string TITLE = "Main Window";
// Working directory
std::string PWD = std::filesystem::current_path().string();
// Shader sources
std::string VERTEX_SHADER_PATH = PWD + "/../shaders/vShader.glsl";
std::string FRAGMENT_SHADER_PATH = PWD + "/../shaders/fShader.glsl";

// Float array for triangle shape, each point has 3 coordinated (x,y,z)
GLfloat RECTANGLE[] = 
{   // Positions             // Colors
    -1.0f, -1.0f, 0.0f,     //1.0f, 0.0f, 0.0f,   // Bottom left
     1.0f, -1.0f, 0.0f,     //0.0f, 1.0f, 0.0f,   // Bottom right
     1.0f,  1.0f, 0.0f,     //0.0f, 0.0f, 1.0f,   // Top right
    -1.0f,  1.0f, 0.0f,     //1.0f, 0.0f, 1.0f,   // Top left
};

GLfloat COLORS[] =
{
    1.0f, 0.0f, 0.0f,   // Red
    0.0f, 1.0f, 0.0f,   // Green
    0.0f, 0.0f, 1.0f,   // Blue
    1.0f, 0.0f, 1.0f,   // Red + Blue
};

GLfloat TEXTURE[] =
{
    0.0f, 0.0f,     // Bottom left
    0.0f, 1.0f,     // Top left
    1.0f, 1.0f,     // Top right
    1.0f, 0.0f,     // Bottom right
};

unsigned int INDICES[] = 
{
    0, 1, 2,    // First triangle (bottom left + bottom right + top right)
    0, 2, 3,    // Second triangle (bottom left + top right + top left)
};

int main()
{
    glWindow window(TITLE, WIDTH, HEIGHT);

    // Setup Shaders
    // Create new shader object from shader files and compile shader program
    Shader shader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());
    
    // Create new Mesh object to handle triangle shape.
    Mesh mesh(RECTANGLE, sizeof(RECTANGLE), COLORS, sizeof(COLORS), TEXTURE, sizeof(TEXTURE), INDICES, sizeof(INDICES), GL_FLOAT, false, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(std::filesystem::current_path().concat("/../texture.jpg").c_str(), &width, &height, &nrChannels, 0);
    if(!data)
    {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(data);

    // Main loop
    // Run until window should close
    while (!window.isShouldClose())
    {
        // Clear window to black screen
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);

        // User shader program with our triangle with shaders.
        shader.use();

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
