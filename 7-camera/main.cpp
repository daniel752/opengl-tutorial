#include "glWindow.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window dimensions
GLfloat WIDTH = 800.0f, HEIGHT = 600.0f;
std::string TITLE = "Main Window";
// Working directory
std::string PWD = std::filesystem::current_path().string();
// Shader sources
std::string VERTEX_SHADER_PATH = PWD + "/../shaders/vShader.glsl";
std::string FRAGMENT_SHADER_PATH = PWD + "/../shaders/fShader.glsl";

// Model transformation matrix
glm::mat4 model;
// View transformation matrix
glm::mat4 view;
// Projection transformation matrix
glm::mat4 projection;
// Translation (location/movement) vector 3 (x,y,z) to move object
glm::vec3 translationVec(0.0f);

float deltaTime = 0;
float lastFrame = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

// Float array for cube, each point has 3 coordinated (x,y,z)
float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};

GLfloat texture[] =
{
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
};

unsigned int indices[] = 
{
    0, 1, 2,    // First triangle (bottom left + bottom right + top right)
    0, 2, 3,    // Second triangle (bottom left + top right + top left)
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

void processInput(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // If user press "W" key button - move up the y axis
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);

    // If user press "S" key button - move down the y axis
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);

    // If user press "A" key button - move up x axis (move right)
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);

    // If user press "D" key button - move down x axis (move left)
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.setMovementSpeed(SPEED * 5.0f);
    else
        camera.setMovementSpeed(SPEED);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        exit(0);
}

int main()
{
    glWindow window(TITLE, WIDTH, HEIGHT);

    // Setup Shaders
    // Create new shader object from shader files and compile shader program
    Shader shader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());

    glfwSetInputMode(window.getGlWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.getGlWindow(), mouse_callback);
    glfwSetScrollCallback(window.getGlWindow(), scroll_callback);

    // Create new Mesh object to handle triangle shape.
    Mesh mesh(vertices, sizeof(vertices), nullptr, 0, texture, sizeof(texture), indices, sizeof(indices), GL_FLOAT, false, 0);
    // Initialise VAO (vertex array object) and VBO (vertex buffer object) for mesh
    mesh.initialise(GL_FLOAT, GL_FALSE);
    // Add indices (elements) attribute to mesh
    mesh.addAttribute(0, GL_ELEMENT_ARRAY_BUFFER, 3, 0, 0, indices, sizeof(indices), &mesh.elementBuffer);
    // Add texture attribute to mesh
    mesh.addAttribute(1, GL_ARRAY_BUFFER, 2, GL_FLOAT, GL_FALSE, texture, sizeof(texture), &mesh.textureBuffer);
    // Unbind mesh from global state
    mesh.unbind();

    // Generating texture and configuring parameters
    GLuint texture1, texture2;
    shader.loadTexture(std::filesystem::current_path().concat("/../../assets/container.jpg"), &texture1, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, GL_RGB, 0, GL_RGB, GL_UNSIGNED_BYTE);
    shader.loadTexture(std::filesystem::current_path().concat("/../../assets/awesomeface.png"), &texture2, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, GL_RGBA, 0, GL_RGBA, GL_UNSIGNED_BYTE);

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

    // Tell OpenGL to enable depth buffer
    shader.enableDepth();

    // Set uniform sampler2D "texture1" to texture unit 0
    shader.setInt("texture1", 0);
    // Set uniform sampler2D "texture1" to texture unit 1
    shader.setInt("texture2", 1);

    // Main loop
    // Run until window should close
    while (!window.isShouldClose())
    {
        processInput(window.getGlWindow());

        // Clear window to black screen
        glClearColor(0, 0, 0, 1);
        // Clear color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set perspective projection matrix
        projection = glm::perspective(glm::radians(camera.getFov()), WIDTH / HEIGHT, 0.1f, 100.0f);
        shader.setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

        // Set view matrix
        view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
        shader.setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));

        // Iterate over objects locations (vec3 array)
        for(unsigned int i = 0; i < 10; i++)
        {
            // Create new 4x4 diagonal matrix that equals 1
            model = glm::mat4(1.0f);
            // Translate model's location according to "cubePositions" at index i
            model = glm::translate(model, cubePositions[i]);
            // Angle variable for model's rotation
            float angle;
            // Vector 3 (x,y,z) to specify the effect of the rotation on each of the axes
            glm::vec3 rotationVec;
            int moduloVal = 5;
            // If index devides by 2
            if(i % 2 == 0)
            {
                angle = glfwGetTime() * 15.0f * ((i + 1) % moduloVal);
                rotationVec = glm::vec3(1.0f, 0.5f, 0.2f);
            }
            // If index devides by 3
            if(i % 3 == 0)
            {
                angle = glfwGetTime() * 25.0f * ((i + 1) % moduloVal);
                rotationVec = glm::vec3(0.5f, 1.0f, 0.2f);
            }
            // Rotate model according to angle and rotation vector
            model = glm::rotate(model, glm::radians(angle), rotationVec);
            // Update uniform matrix in shader program
            shader.setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
            // Render model
            mesh.render(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }

        // Render triangle
        mesh.render(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

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

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xOffset, yOffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(static_cast<float>(yoffset));
}