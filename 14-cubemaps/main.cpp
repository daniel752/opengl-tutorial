#include "glWindow.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "cubemap.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

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

float cubeVertices[] = 
{
    // positions          // normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

float skyboxVerticies[] = 
{
    -20.0f,  20.0f, -20.0f,
    -20.0f, -20.0f, -20.0f,
     20.0f, -20.0f, -20.0f,
     20.0f, -20.0f, -20.0f,
     20.0f,  20.0f, -20.0f,
    -20.0f,  20.0f, -20.0f,
    -20.0f, -20.0f,  20.0f,
    -20.0f, -20.0f, -20.0f,
    -20.0f,  20.0f, -20.0f,
    -20.0f,  20.0f, -20.0f,
    -20.0f,  20.0f,  20.0f,
    -20.0f, -20.0f,  20.0f,
     20.0f, -20.0f, -20.0f,
     20.0f, -20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f,  20.0f, -20.0f,
     20.0f, -20.0f, -20.0f,
    -20.0f, -20.0f,  20.0f,
    -20.0f,  20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f, -20.0f,  20.0f,
    -20.0f, -20.0f,  20.0f,
    -20.0f,  20.0f, -20.0f,
     20.0f,  20.0f, -20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
    -20.0f,  20.0f,  20.0f,
    -20.0f,  20.0f, -20.0f,
    -20.0f, -20.0f, -20.0f,
    -20.0f, -20.0f,  20.0f,
     20.0f, -20.0f, -20.0f,
     20.0f, -20.0f, -20.0f,
    -20.0f, -20.0f,  20.0f,
     20.0f, -20.0f,  20.0f   
};

std::vector<std::string> faces =
{
    pwd + "/../../assets/skybox/right.jpg",
    pwd + "/../../assets/skybox/left.jpg",
    pwd + "/../../assets/skybox/top.jpg",
    pwd + "/../../assets/skybox/bottom.jpg",
    pwd + "/../../assets/skybox/front.jpg",
    pwd + "/../../assets/skybox/back.jpg"
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

    Shader::enableGL(GL_DEPTH_TEST);

    // Build and compile shaders
    Shader shader((pwd + "/../shaders/cubemap.vs").c_str(), (pwd + "/../shaders/cubemap.fs").c_str());
    Shader skyboxShader((pwd + "/../shaders/skybox.vs").c_str(), (pwd + "/../shaders/skybox.fs").c_str());

    Cubemap skybox(faces);

    unsigned int cubeVao, cubeVbo;
    glGenVertexArrays(1, &cubeVao);
    glGenBuffers(1, &cubeVbo);
    glBindVertexArray(cubeVao);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    unsigned int skyboxVao, skyboxVbo;
    glGenVertexArrays(1, &skyboxVao);
    glGenBuffers(1, &skyboxVbo);
    glBindVertexArray(skyboxVao);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVerticies), skyboxVerticies, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    Texture container(pwd + "/../../assets/container.jpg");
    container.loadTexture();

    shader.use();
    shader.setInt("texture1", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

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
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        model = glm::mat4(1.0f);
        view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront());
        projection = glm::perspective(camera.getFov(), width / height, 0.1f, 100.0f);
        shader.setMatrix4fv("model", 1, GL_FALSE, model);
        shader.setMatrix4fv("view", 1, GL_FALSE, view);
        shader.setMatrix4fv("projection", 1, GL_FALSE, projection);
        shader.setVec3("cameraPosition", glm::value_ptr(camera.getPosition()));

        glBindVertexArray(cubeVao);
        container.useTexture();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        Shader::depthFunctionality(GL_LEQUAL);
        skyboxShader.use();
        // Converting "view" matrix to 3x3 matrix and back to 4x4 matrix to update only rotation in skybox
        view = glm::mat4(glm::mat3(camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront())));
        skyboxShader.setMatrix4fv("view", 1, GL_FALSE, view);
        skyboxShader.setMatrix4fv("projection", 1, GL_FALSE, projection);

        glBindVertexArray(skyboxVao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextureId());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        Shader::depthFunctionality(GL_LESS);

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