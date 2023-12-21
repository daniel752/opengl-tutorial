#include "glWindow.h"
#include "shader.h"
#include "mesh.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Window dimensions
constexpr GLint WIDTH = 800, HEIGHT = 600;
std::string TITLE = "Main Window";
// Working directory
std::string PWD = std::filesystem::current_path().string();
// Shader sources
std::string VERTEX_SHADER_PATH = PWD + "/../shaders/vShader.glsl";
std::string FRAGMENT_SHADER_PATH = PWD + "/../shaders/fShader.glsl";

// Matrix 4x4 for tranformation calculations
glm::mat4 transform;
// Translation (location/movement) vector 3 (x,y,z) to move object
glm::vec3 translationVec(0.0f);
// Speed of object movement
float translationSpeed = 0.01;
// Scale value to multiply scaling matrix
float scaleValue = 0.2;
// Rotation value to multiply rotation matrix
float rotationValue = 0;

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

void processInput(GLFWwindow* window)
{
    // If user press up key button - increase scale
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        scaleValue += 0.01;
        if((scaleValue) >= 1.0)
            scaleValue = 0.1;

    // If user press down key button - decrease scale
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        scaleValue -= 0.01;
        if(scaleValue <= 0)
            scaleValue = 0.99;

    // If user press right key button - rotate to right
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotationValue -= 1;
        if(rotationValue <= -360)
            rotationValue = 0;
    
    // If user press left key button - rotate to left
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotationValue += 1;
            if(rotationValue >= 360)
                rotationValue = 0;

    // If user press "W" key button - move up the y axis
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        translationVec = glm::vec3(translationVec.x, translationVec.y + translationSpeed, 0.0f);
    }

    // If user press "S" key button - move down the y axis
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        translationVec = glm::vec3(translationVec.x, translationVec.y - translationSpeed, 0.0f);
    }

    // If user press "A" key button - move up x axis (move right)
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        translationVec = glm::vec3(translationVec.x - translationSpeed, translationVec.y, 0.0f);
    }

    // If user press "D" key button - move down x axis (move left)
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        translationVec = glm::vec3(translationVec.x + translationSpeed, translationVec.y, 0.0f);
    }
}

int main()
{
    glWindow window(TITLE, WIDTH, HEIGHT);

    // Setup Shaders
    // Create new shader object from shader files and compile shader program
    Shader shader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());
    
    // Create new Mesh object to handle triangle shape.
    Mesh mesh(RECTANGLE, sizeof(RECTANGLE), COLORS, sizeof(COLORS), TEXTURE, sizeof(TEXTURE), INDICES, sizeof(INDICES), GL_FLOAT, false, 0);
    // Initialise VAO (vertex array object) and VBO (vertex buffer object) for mesh
    mesh.initialise(GL_FLOAT, GL_FALSE);
    // Add indices (elements) attribute to mesh
    mesh.addAttribute(0, GL_ELEMENT_ARRAY_BUFFER, 3, 0, 0, INDICES, sizeof(INDICES), &mesh.elementBuffer);
    // Add color attribute to mesh
    mesh.addAttribute(1, GL_ARRAY_BUFFER, 3, GL_FLOAT, GL_FALSE, COLORS, sizeof(COLORS), &mesh.colorBuffer);
    // Add texture attribute to mesh
    mesh.addAttribute(2, GL_ARRAY_BUFFER, 2, GL_FLOAT, GL_FALSE, TEXTURE, sizeof(TEXTURE), &mesh.textureBuffer);
    // Unbind mesh from global state
    mesh.unbind();

    // Generating texture and configuring parameters
    GLuint texture1, texture2;
    shader.loadTexture(std::filesystem::current_path().concat("/../container.jpg"), &texture1, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, GL_RGB, 0, GL_RGB, GL_UNSIGNED_BYTE);
    shader.loadTexture(std::filesystem::current_path().concat("/../awesomeface.png"), &texture2, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, GL_RGBA, 0, GL_RGBA, GL_UNSIGNED_BYTE);

    // OpenGL should have provide 16 texture units from the GPU
    // Texture units can be activated using "GL_TEXTURE0" to "GL_TEXTURE15"
    // They are defined in order so we could also get GL_TEXTURE8 via GL_TEXTURE0 + 8 for example, 
    // which is useful when we'd have to loop over several texture units. 
    glActiveTexture(GL_TEXTURE0);   // Activate texture unit 0
    glBindTexture(GL_TEXTURE_2D, texture1); // Bind texture unit 0 to "texture1"
    glActiveTexture(GL_TEXTURE1);   // Activate texture unit 1
    glBindTexture(GL_TEXTURE_2D, texture2); // Bind texture unit 1 to "texture2"

    // Use shader program before setting uniforms
    shader.use();

    // Set uniform sampler2D "texture1" to texture unit 0
    shader.setInt("texture1", 0);
    // Set uniform sampler2D "texture1" to texture unit 1
    shader.setInt("texture2", 1);

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

        // Tranformation matrix
        // Creating new 4x4 matrix with all zeroes except the diagonal which equals 1
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, translationVec);
        // Applying scaling to transformation matrix
        transform = glm::scale(transform, glm::vec3(scaleValue));
        // Applying rotation to transformation matrix
        transform = glm::rotate(transform, glm::radians(rotationValue), glm::vec3(0.0, 0.0, 1.0));
        // Setting transformation matrix in shader program
        shader.setMatrix4fv("transform", glm::value_ptr(transform));

        // Render triangle
        mesh.render(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Uncomment to render second container with smiley face :)
        // transform = glm::mat4(1.0f);
        // transform = glm::translate(transform, glm::vec3(-0.3f, 0.3f, 0.0f));
        // transform = glm::scale(transform, glm::vec3(scaleValue));
        // transform = glm::rotate(transform, glm::radians(rotationValue), glm::vec3(0.0, 0.0, 1.0));
        // shader.setMatrix4fv("transform", &transform[0][0]);
        // mesh.copy(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        mesh.unbind();

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

    return 0;
}
