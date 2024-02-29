#include "glWindow.h"
#include "shader.h"
#include "texture.h"
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
std::string blendingVShaderPath = pwd + "/../shaders/blending.vs";
std::string blendingFShaderPath = pwd + "/../shaders/blending.fs";
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

// Verticies for a simple 2D quad to display grass texture on
float windowVertices[] =
{
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

std::vector<glm::vec3> windowsPositions = 
{
    glm::vec3(0.0f, 0.5f, -3.0f),
    glm::vec3(3.0f, 0.5f, -3.0f),
    glm::vec3(-3.0f, 0.5f, 3.0f),
    glm::vec3(5.0f, 0.5f, 7.0f),
    glm::vec3(-5.0f, 0.5f, -7.0f),
    glm::vec3(-6.25f, 0.5f, -2.0f),
    glm::vec3(1.75f, 0.5f, 5.5f),
    glm::vec3(3.0f, 0.5f, 10.0f),
    glm::vec3(0.0f, 0.5f, -10.0f),
};

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
    // stbi_set_flip_vertically_on_load(true);

    // Build and compile shaders
    Shader meshShader(meshVShaderPath.c_str(), meshFShaderPath.c_str());
    Shader colorShader(colorVShaderPath.c_str(), colorFShaderPath.c_str());
    Shader blendShader(blendingVShaderPath.c_str(), blendingFShaderPath.c_str());

    // Configure OpenGL to use depth buffer
    Shader::enableGL(GL_DEPTH_TEST);
    Shader::enableGL(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load model
    Model cube(cubePath.c_str());

    // Creating 2D quad for grass texture
    unsigned int windowVAO, windowVBO;
    glGenVertexArrays(1, &windowVAO);
    glGenBuffers(1, &windowVBO);
    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), windowVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Create new texture object
    Texture windowTexture(pwd + "/../../assets/transparent_window.png");
    windowTexture.setTextureId(0);
    windowTexture.setTextureUnit(GL_TEXTURE0);
    // The default wrapping parameter is GL_REPEAT which is usually ok but when using transparent textures
    // we can have a small white line at the edge of the transparent texture, so using GL_CLAMP_TO_EDGE fixes that
    // windowTexture.setWrappingParams(GL_CLAMP_TO_EDGE);
    // Load texture
    windowTexture.loadTexture();
    windowTexture.useTexture();

    blendShader.use();
    // Point shader's uniform sampler2D to point to texture unit 0
    blendShader.setInt("texture1", 0);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate projection and view
        // ----------------------------------------------
        glm::mat4 projection = glm::perspective(camera.getFov(), width / height, 0.1f, 100.0f);
        glm::mat4 view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());

        meshShader.use();
        meshShader.setMatrix4fv("projection", 1, GL_FALSE, projection);
        meshShader.setMatrix4fv("view", 1, GL_FALSE, view);

        // Render our floor in scene without writing any values to stencil buffer, we only care about the other rendered cubes
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.1f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 0.01f, 1.0f));
        meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
        meshShader.setVec3("color", glm::value_ptr(glm::vec3(0.2f, 0.4f, 0.7f)));
        cube.draw(meshShader);

        // meshShader.use();
        // meshShader.setMatrix4fv("projection", 1, GL_FALSE, projection);
        // meshShader.setMatrix4fv("view", 1, GL_FALSE, view);

        // Render cubes
        // ------------------------------------------------
        meshShader.setVec3("color", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        // Set first cube at the center of the scene
        model = glm::mat4(1.0f);
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

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
        model = glm::scale(model, glm::vec3(0.015f));
        meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
        cube.draw(meshShader);

        // Render transparent windows
        // ------------------------------------------------
        // Sorting windows positions according to their distance from camera
        // We need to render the windows from the farthest window to the closest one
        // We just create a map with the distance as key and the map will automatically sort the keys for us from low to high
        std::map<float, glm::vec3> sortedWindows;
        for(unsigned int i = 0; i < windowsPositions.size(); i++)
        {
            float distance = glm::length(camera.getPosition() - windowsPositions[i]);
            sortedWindows[distance] = windowsPositions[i];
        }

        blendShader.use();
        blendShader.setMatrix4fv("projection", 1, GL_FALSE, projection);
        blendShader.setMatrix4fv("view" , 1, GL_FALSE, view);
        glBindVertexArray(windowVAO);
        // Iterate sorted windows in reverse order so we start from the highest (farthest) to the lowest (closest)
        // and render the windows in this order
        for(std::map<float, glm::vec3>::reverse_iterator iter = sortedWindows.rbegin(); iter != sortedWindows.rend(); ++iter)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, iter->second);
            blendShader.setMatrix4fv("model", 1, GL_FALSE, model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

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