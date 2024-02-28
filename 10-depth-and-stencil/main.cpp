#include "glWindow.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "stb_image.h"

#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processMovement(GLFWwindow* window, Camera* camera);

// Window configuration
GLfloat width = 800.0f, height = 600.0f;
std::string title = "Main Window";
bool isFullscreen = false;
// Working directory
std::string pwd = std::filesystem::current_path().string();
// Shader paths
std::string meshVShaderPath = pwd + "/../shaders/mesh.vs";
std::string meshFShaderPath = pwd + "/../shaders/mesh.fs";
std::string colorVShaderPath = pwd + "/../shaders/singleColor.vs";
std::string colorFShaderPath = pwd + "/../shaders/singleColor.fs";
std::string modelPath = pwd + "/../../assets/backpack/backpack.obj";
std::string cubePath = pwd + "/../../assets/cube/cube.obj";

// Model transformation matrix
glm::mat4 model;
// View transformation matrix
glm::mat4 view;
// Projection transformation matrix
glm::mat4 projection;
// Translation (location/movement) vector 3 (x,y,z) to move object
glm::vec3 translationVec(0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Create Camera object with starting location
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastX = width / 2.0f;
float lastY = height / 2.0f;

int main()
{
    // Create window object
    glWindow window(title, width, height);

    // Set callback function to capture cursor (focus on window)
    glfwSetInputMode(window.getGlWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Set callback function to capture mouse movement
    glfwSetCursorPosCallback(window.getGlWindow(), mouse_callback);
    // Set callback function to capture mouse scrolling
    glfwSetScrollCallback(window.getGlWindow(), scroll_callback);
    glfwSetKeyCallback(window.getGlWindow(), key_callback);

    // Tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // Build and compile shaders
    // -------------------------
    Shader meshShader(meshVShaderPath.c_str(), meshFShaderPath.c_str());
    Shader colorShader(colorVShaderPath.c_str(), colorFShaderPath.c_str());

    // Configure OpenGL to use depth buffer
    // -----------------------------
    Shader::enableDepth();
    // We can disable writing to the depth buffer by using "glDepthMask(GL_FALSE)", which is what "lockDepthBuffer()" does
    // Shader::lockDepthBuffer();
    Shader::depthFunctionality(GL_ALWAYS);
    // "GL_LESS" is the default functionality for OpenGL
    // Shader::depthFunctionality(GL_LESS);

    // Configure OpenGL to use stencil buffer
    Shader::enableStencil();
    // Pass to stencil buffer only values as is that are NOT equal to 1
    Shader::stencilFunctionality(GL_NOTEQUAL, 1, 0xFF);
    // Keep values when stencil test fails or stencil test passes and depth test fails,
    // and if stencil test and depth test pass then replace value with reference value
    Shader::stencilOperation(GL_KEEP, GL_KEEP, GL_REPLACE);
    // Shader::unlockStencilBuffer();

    // Load model
    // -----------
    Model cube(cubePath.c_str());

    // Uncomment to render models in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    // -----------
    while (!window.isShouldClose())
    {
        // Process input
        // -------------
        processMovement(window.getGlWindow(), &camera);

        // Set background color and clear color buffer, depth buffer and stencil buffer
        // ------------------------------------------------------------
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Calculate projection and view
        // ----------------------------------------------
        glm::mat4 projection = glm::perspective(camera.getFov(), width / height, 0.1f, 100.0f);
        glm::mat4 view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());

        // Set stencil test to always pass and write values as is
        // This will fill the stencil buffer with 1's wherever the object fragments are rendered
        Shader::stencilFunctionality(GL_ALWAYS, 1, 0xFF);
        // Unlock wrting to stencil buffer
        Shader::unlockStencilBuffer();

        // Render cubes
        // ------------------------------------------------
        // Enable shader before setting uniforms
        meshShader.use();
        meshShader.setMatrix4fv("projection", 1, GL_FALSE, projection);
        meshShader.setMatrix4fv("view", 1, GL_FALSE, view);

        glm::mat4 model = glm::mat4(1.0f);
        // Set first cube at the center of the scene
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        // Scale it down cause the cube model is huge
        model = glm::scale(model, glm::vec3(0.015f));
        // Set uniform model matrix in shader
        meshShader.setMatrix4fv("model", 1, GL_FALSE, model);

        // Render first cube
        cube.draw(meshShader);

        // Set second cube with a little offset on the Z-axis
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
        // Scale it down cause the cube model is huge
        model = glm::scale(model, glm::vec3(0.015f));
        meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
        // Render second cube
        cube.draw(meshShader);

        // Set stencil test to pass only values as is that are NOT equal to 1
        // This way it will only render fragments that don't overlap with the fragments from the cubes
        // Since we scale the cubes (a few lines below) a little bigger than only the fragments between the edges of the cubes will be rendered
        Shader::stencilFunctionality(GL_NOTEQUAL, 1, 0xFF);
        Shader::lockStencilBuffer();

        colorShader.use();
        // Set the color of the border, the rendered fragments between the edges of the cubes
        colorShader.setVec3("color", glm::value_ptr(glm::vec3(0.24f, 0.48f, 0.86f)));

        colorShader.setMatrix4fv("view", 1, GL_FALSE, view);
        colorShader.setMatrix4fv("projection", 1, GL_FALSE, projection);

        model = glm::mat4(1.0f);
        // Set first cube at the center of the scene
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        // Scale it down cause the cube model is huge
        model = glm::scale(model, glm::vec3(0.017f));
        // Set uniform model matrix in shader
        colorShader.setMatrix4fv("model", 1, GL_FALSE, model);
        // Render first cube
        cube.draw(meshShader);

        // Set second cube with a little offset on the Z-axis
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
        // Scale it down cause the cube model is huge
        model = glm::scale(model, glm::vec3(0.017f));
        colorShader.setMatrix4fv("model", 1, GL_FALSE, model);
        // Render second cube
        cube.draw(meshShader);

        // When done with stencil buffer then enable back writing to it so the rest of the scene can render normaly
        Shader::unlockStencilBuffer();

        window.swapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
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

void processMovement(GLFWwindow* window, Camera* camera)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // If user presses "W" key - move up the y axis
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(FORWARD, deltaTime);

    // If user presses "S" key - move down the y axis
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(BACKWARD, deltaTime);

    // If user presses "A" key - move up x axis (move right)
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(LEFT, deltaTime);

    // If user presses "D" key - move down x axis (move left)
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(RIGHT, deltaTime);
    
    // If user presses LEFT_SHIFT key - increase camera movement
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->setMovementSpeed(SPEED * 5.0f);
    // Otherwise normal camera speed
    else
        camera->setMovementSpeed(SPEED);
}

// Function to handle user input from keyboard
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if(!isFullscreen)
        {
            isFullscreen = true;
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_REFRESH_RATE);
        }
        else
        {
            isFullscreen = false;
            glfwSetWindowMonitor(window, NULL, 0, 0, 800, 600, GLFW_REFRESH_RATE);
        }
    }

    // If user presses ESC key button - exit program
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        exit(0);
}