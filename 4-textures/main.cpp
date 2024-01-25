#include "glWindow.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "stb_image.h"


// Window dimensions
GLint width = 800, height = 600;
std::string title = "Main Window";
// Working directory
std::string pwd = std::filesystem::current_path().string();
// Shader sources
std::string vShaderPath = pwd + "/../shaders/vShader.vs";
std::string fShaderPath = pwd + "/../shaders/fShader.fs";

float mixValue = 0.2f;

// Float array for triangle shape, each point has 3 coordinated (x,y,z)
GLfloat rectangle[] = 
{   // Positions             // Colors
    -1.0f, -1.0f, 0.0f,     //1.0f, 0.0f, 0.0f,   // Bottom left
     1.0f, -1.0f, 0.0f,     //0.0f, 1.0f, 0.0f,   // Bottom right
     1.0f,  1.0f, 0.0f,     //0.0f, 0.0f, 1.0f,   // Top right
    -1.0f,  1.0f, 0.0f,     //1.0f, 0.0f, 1.0f,   // Top left
};

GLfloat colors[] =
{
    1.0f, 0.0f, 0.0f,   // Red
    0.0f, 1.0f, 0.0f,   // Green
    0.0f, 0.0f, 1.0f,   // Blue
    1.0f, 0.0f, 1.0f,   // Red + Blue
};

GLfloat texture[] =
{
    0.0f, 0.0f,     // Bottom left
    0.0f, 1.0f,     // Top left
    1.0f, 1.0f,     // Top right
    1.0f, 0.0f,     // Bottom right
};

unsigned int indices[] = 
{
    0, 1, 2,    // First triangle (bottom left + bottom right + top right)
    0, 2, 3,    // Second triangle (bottom left + top right + top left)
};

void processInput(GLFWwindow* window)
{
    // If user press up key button
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        mixValue += 0.01;
        if((mixValue) >= 1.0)
            mixValue = 1.0;

    // If user press down key button
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        mixValue -= 0.01;
        if(mixValue <= 0.0)
            mixValue = 0.0;

    // If user presses ESC key button - exit program
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        exit(0);
}

int main()
{
    glWindow window(title, width, height);

    // Setup Shaders
    // Create new shader object from shader files and compile shader program
    Shader shader(vShaderPath.c_str(), fShaderPath.c_str());
    
    // Create new Mesh object to handle triangle shape.
    Mesh mesh(rectangle, sizeof(rectangle), colors, sizeof(colors), texture, sizeof(texture), indices, sizeof(indices), GL_FLOAT, false, 0);
    // Initialise VAO (vertex array object) and VBO (vertex buffer object) for mesh
    mesh.initialise(GL_FLOAT, GL_FALSE);
    // Add indices (elements) attribute to mesh
    mesh.addAttribute(0, GL_ELEMENT_ARRAY_BUFFER, 3, 0, 0, indices, sizeof(indices), &mesh.elementBuffer);
    // Add color attribute to mesh
    mesh.addAttribute(1, GL_ARRAY_BUFFER, 3, GL_FLOAT, GL_FALSE, colors, sizeof(colors), &mesh.colorBuffer);
    // Add texture attribute to mesh
    mesh.addAttribute(2, GL_ARRAY_BUFFER, 2, GL_FLOAT, GL_FALSE, texture, sizeof(texture), &mesh.textureBuffer);
    // Unbind mesh from global state
    mesh.unbind();

    // Generating textures with their configurations
    Texture smileyFace(0, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, 0, pwd + "/../../assets/awesomeface.png", GL_TEXTURE0);
    Texture container(1, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, 0, pwd + "/../../assets/container.jpg", GL_TEXTURE1);

    smileyFace.loadTexture();
    container.loadTexture();
    smileyFace.useTexture();
    container.useTexture();

    // Use shader program before setting uniforms
    shader.use();

    // Set uniform sampler2D in textures array to texture unit 0
    shader.setInt("textures[0]", 0);
    // Set uniform sampler2D in textures array to texture unit 1
    shader.setInt("textures[1]", 1);

    // Main loop
    // Run until window should close
    while (!window.isShouldClose())
    {
        processInput(window.getGlWindow());
        // window.processInput(GLFW_KEY_UP, &mixValue, 0.01f);
        // window.processInput(GLFW_KEY_DOWN, &mixValue, -0.01f);

        // Clear window to black screen
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Updating shader with new "mixValue" value
        // The alpha between the textures can be increased or decreases by pressing the UP/DOWN button on keyboard 
        shader.setFloat("mixValue", mixValue);

        // Render triangle
        mesh.render(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // OpenGL uses a double buffer (front and back buffers) technique for rendering
        // All of the rendering commands will go to back buffer
        // After all of the rendering commands are done we switch to the front buffer which has the final output image
        // If we don't do this we can see flickering in output image, cause the image is build pixel by pixel from left to right and from top to bottom
        // So basically we'll see the image being build and that's why will see the flickering (if image is small and not complex than it probably won't matter)
        window.swapBuffers();
        // Get + Handle glfwWindow I/O
        glfwPollEvents();
    }
    mesh.clear();
    shader.unbind();
    shader.clear();
    smileyFace.clearTexture();
    container.clearTexture();

    return 0;
}
