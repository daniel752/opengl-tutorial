#include "glWindow.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"

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
// Shader paths
std::string meshVShaderPath = pwd + "/../shaders/mesh.vs";
std::string meshFShaderPath = pwd + "/../shaders/mesh.fs";
std::string screenVShaderPath = pwd + "/../shaders/screen.vs";
std::string screenFShaderPath = pwd + "/../shaders/screen.fs";

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
    // Back face          // Texture coordinates
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
};

float floorVerticies[] =
{
    // First triangle
     5.0f, -0.5f,  5.0f, 5.0f, 0.0f,
    -5.0f, -0.5f,  5.0f, 0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 5.0f,

    // Second triangle
     5.0f, -0.5f,  5.0f, 5.0f, 0.0f,
     5.0f, -0.5f, -5.0f, 5.0f, 5.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 5.0f,
};

float quadVerticies[] = 
{
    // First triangle
    -1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,

    // Second triangle
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
};

// Comment quad verticies above and uncomment this for rear-view mirror effect
// float quadVerticies[] = 
// {
//     -0.3f,  1.0f,  0.0f, 1.0f,
//     -0.3f,  0.7f,  0.0f, 0.0f,
//     0.3f,  0.7f,  1.0f, 0.0f,

//     -0.3f,  1.0f,  0.0f, 1.0f,
//     0.3f,  0.7f,  1.0f, 0.0f,
//     0.3f,  1.0f,  1.0f, 1.0f
// };

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
    Shader meshShader(meshVShaderPath.c_str(), meshFShaderPath.c_str());
    Shader screenShader(screenVShaderPath.c_str(), screenFShaderPath.c_str());

    // Cube creation
    unsigned int cubeVao, cubeVbo;
    glGenVertexArrays(1, &cubeVao);
    glGenBuffers(1, &cubeVbo);
    glBindVertexArray(cubeVao);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    
    // Floor creation
    unsigned int floorVao, floorVbo;
    glGenVertexArrays(1, &floorVao);
    glGenBuffers(1, &floorVbo);
    glBindVertexArray(floorVao);
    glBindBuffer(GL_ARRAY_BUFFER, floorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVerticies), floorVerticies, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Model backpack((pwd + "/../../assets/backpack/backpack.obj").c_str());

    // 2D Quad creation
    unsigned int quadVao, quadVbo;
    glGenVertexArrays(1, &quadVao);
    glGenBuffers(1, &quadVbo);
    glBindVertexArray(quadVao);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerticies), quadVerticies, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    Texture smileyFace = Texture(pwd + "/../../assets/awesomeface.png");
    Texture woodContainer = Texture(pwd + "/../../assets/container.jpg");

    smileyFace.loadTexture();
    smileyFace.setWrappingParams(GL_REPEAT);
    woodContainer.loadTexture();

    meshShader.use();
    meshShader.setInt("diffuse", 0);

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    // Framebuffer configuration
    // --------------------------------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int textureColorBuffer;
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    unsigned int renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Can't initialise framebuffer, exiting program." << std::endl;
        std::exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Uncomment to render models in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    // -----------
    while (!window.isShouldClose())
    {
        // Process input
        // -------------
        processMovement(window.getGlWindow(), &camera);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        // Clearing framebuffer
        // ------------------------------------------------------------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader::enableGL(GL_DEPTH_TEST);

        // Render
        // ----------------------------------------------
        // Rear-view mirror effect -----------------------
        // glm::mat4 projection = glm::perspective(camera.getFov(), width / height, 0.1f, 100.0f);

        // camera.setYaw(camera.getYaw() + 180);
        // camera.processMouseMovement(0, 0, false);
        // glm::mat4 view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
        // camera.setYaw(camera.getYaw() - 180);
        // camera.processMouseMovement(0, 0, true);

        // meshShader.use();
        // meshShader.setMatrix4fv("projection", 1, GL_FALSE, projection);
        // meshShader.setMatrix4fv("view", 1, GL_FALSE, view);

        // glBindVertexArray(cubeVao);
        // woodContainer.useTexture();
        // model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        // meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // model = glm::mat4(1.0f);
        // model = glm::translate(model ,glm::vec3(2.0f, 0.0f, 0.0f));
        // meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // glBindVertexArray(floorVao);
        // smileyFace.useTexture();
        // model = glm::mat4(1.0f);
        // meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        // glBindVertexArray(0);

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // glClearColor(0.15f, 0.15, 0.15f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Rear-view mirror effect -----------------------

        view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());

        // Comment this line if using rear-view code
        projection = glm::perspective(camera.getFov(), width / height, 0.1f, 100.0f);

        meshShader.use();
        meshShader.setMatrix4fv("view", 1, GL_FALSE, view);
        meshShader.setMatrix4fv("projection", 1, GL_FALSE, projection);

        glBindVertexArray(cubeVao);
        woodContainer.useTexture();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model ,glm::vec3(2.0f, 0.0f, 0.0f));
        meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(floorVao);
        smileyFace.useTexture();
        model = glm::mat4(1.0f);
        meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        Shader::disableGL(GL_DEPTH_TEST);

        screenShader.use();
        glBindVertexArray(quadVao);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);        

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