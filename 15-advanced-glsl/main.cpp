#include "glWindow.h"
#include "shader.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processMovement(GLFWwindow* window, Camera* camera);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Window configuration
GLfloat width = 800.0f, height = 600.0f;
std::string title = "Main Window";
bool isFullscreen = false;
// Working directory
std::string pwd = std::filesystem::current_path().string();
// Model transformation matrix
glm::mat4 model;
// View transformation matrix
glm::mat4 view;
// Projection transformation matrix
glm::mat4 projection;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Create Camera object with starting location
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastX = width / 2.0f;
float lastY = height / 2.0f;

float points[] = 
{
    -2.0f,  2.0f, 0.0f,   // Top left
     0.0f,  2.0f, 0.0f,   // Top center
     2.0f,  2.0f, 0.0f,   // Top right 
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
    glfwSetFramebufferSizeCallback(window.getGlWindow(), framebuffer_size_callback);

    // Build and compile shaders
    Shader shaderRed((pwd + "/../shaders/red.vs").c_str(), (pwd + "/../shaders/red.fs").c_str());
    Shader shaderGreen((pwd + "/../shaders/green.vs").c_str(), (pwd + "/../shaders/green.fs").c_str());
    Shader shaderBlue((pwd + "/../shaders/blue.vs").c_str(), (pwd + "/../shaders/blue.fs").c_str());
    Shader shaderYellow((pwd + "/../shaders/yellow.vs").c_str(), (pwd + "/../shaders/yellow.fs").c_str());
    
    Shader::enableGL(GL_PROGRAM_POINT_SIZE);
    Shader::enableGL(GL_DEPTH_TEST);

    unsigned int pointsVao, pointsVbo;
    glGenVertexArrays(1, &pointsVao);
    glGenBuffers(1, &pointsVbo);
    glBindVertexArray(pointsVao);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Uniform buffer creation
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    // Uncomment to render models in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    // -----------
    while (!window.isShouldClose())
    {
        // Process input
        // -------------
        processMovement(window.getGlWindow(), &camera);

        // Clearing framebuffer
        // ------------------------------------------------------------
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render
        // ------------------------------------------------------------
        // Calculating view and projection matrices
        view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront());
        projection = glm::perspective(camera.getFov(), width / height, 0.1f, 100.0f);
        // Binding uniform buffer to make changes
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        // Inserting view matrix to uniform block at offset 0 with size of glm::mat4 (matrix 4x4)
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
        // Inserting projection matrix to uniform block at offset sizeof(glm::mat4) with size of glm::mat4
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
        shaderRed.use();
        shaderRed.setMatrix4fv("model", 1, GL_FALSE, model);
        shaderRed.setVec3("color", glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
        glBindVertexArray(pointsVao);
        glDrawArrays(GL_POINTS, 0, 3);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shaderGreen.use();
        shaderGreen.setMatrix4fv("model", 1, GL_FALSE, model);
        shaderGreen.setVec3("color", glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(GL_POINTS, 0, 3);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        shaderBlue.use();
        shaderBlue.setMatrix4fv("model", 1, GL_FALSE, model);
        shaderBlue.setVec3("color", glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
        glDrawArrays(GL_POINTS, 0, 3);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
        shaderYellow.use();
        shaderYellow.setMatrix4fv("model", 1, GL_FALSE, model);
        shaderYellow.setVec3("color", glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_POINTS, 0, 3);
        
        glBindVertexArray(0);
        
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}