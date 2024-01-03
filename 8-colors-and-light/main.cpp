#include "glWindow.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Define colors using glm::vec3
glm::vec3 colors[] = 
{
    glm::vec3(1.0f, 1.0f, 1.0f),    // White
    glm::vec3(0.0f, 0.0f, 0.0f),    // Black
    glm::vec3(1.0f, 0.0f, 0.0f),    // Red
    glm::vec3(0.0f, 1.0f, 0.0f),    // Green
    glm::vec3(0.0f, 0.0f, 1.0f),    // Blue
    glm::vec3(1.0f, 1.0f, 0.0f),    // Yellow
    glm::vec3(1.0f, 0.0f, 1.0f),    // Magenta
    glm::vec3(0.0f, 1.0f, 1.0f),    // Cyan
    glm::vec3(0.5f, 0.5f, 0.5f),    // Grey
    glm::vec3(1.0f, 0.5f, 0.0f),    // Orange
    glm::vec3(0.5f, 1.0f, 0.0f),    // Lime
    glm::vec3(0.0f, 0.5f, 1.0f)     // Royal blue
};

// Window dimensions
GLfloat WIDTH = 800.0f, HEIGHT = 600.0f;
std::string TITLE = "Main Window";
// Working directory
std::string PWD = std::filesystem::current_path().string();
// Shader sources
std::string COLOR_V_SHADER_PATH = PWD + "/../shaders/color.vs";
std::string COLOR_F_SHADER_PATH = PWD + "/../shaders/color.fs";
std::string LIGHT_V_SHADER_PATH = PWD + "/../shaders/light.vs";
std::string LIGHT_F_SHADER_PATH = PWD + "/../shaders/light.fs";

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

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

float ambientStrength = 0.1f;
float lightStrength = 0.1f;
float specularStrength = 0.1f;
float specularIntensity = 32;

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

float normals[] =
{
     // Negetive Z axis
     0.0f, 0.0f, -1.0f,
     0.0f, 0.0f, -1.0f,
     0.0f, 0.0f, -1.0f,
     0.0f, 0.0f, -1.0f,
     0.0f, 0.0f, -1.0f,
     0.0f, 0.0f, -1.0f,

     // Positive Z axis
     0.0f, 0.0f,  1.0f,
     0.0f, 0.0f,  1.0f,
     0.0f, 0.0f,  1.0f,
     0.0f, 0.0f,  1.0f,
     0.0f, 0.0f,  1.0f,
     0.0f, 0.0f,  1.0f,

     // Negetive X axis
    -1.0f, 0.0f,  0.0f,
    -1.0f, 0.0f,  0.0f,
    -1.0f, 0.0f,  0.0f,
    -1.0f, 0.0f,  0.0f,
    -1.0f, 0.0f,  0.0f,
    -1.0f, 0.0f,  0.0f,
    
     // Positive X axis
     1.0f, 0.0f,  0.0f,
     1.0f, 0.0f,  0.0f,
     1.0f, 0.0f,  0.0f,
     1.0f, 0.0f,  0.0f,
     1.0f, 0.0f,  0.0f,
     1.0f, 0.0f,  0.0f,

     // Negetive Y axis
     0.0f, -1.0f, 0.0f,
     0.0f, -1.0f, 0.0f,
     0.0f, -1.0f, 0.0f,
     0.0f, -1.0f, 0.0f,
     0.0f, -1.0f, 0.0f,
     0.0f, -1.0f, 0.0f,

     // Positive Y axis
     0.0f,  1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
};

float texture[] =
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
    glm::vec3( 0.0f,  0.0f,  7.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -20.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -15.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

void processInput(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // If user presses "W" key button - move up the y axis
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);

    // If user presses "S" key button - move down the y axis
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);

    // If user presses "A" key button - move up x axis (move right)
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);

    // If user presses "D" key button - move down x axis (move left)
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
    
    // If user presses UP key button - increase lighting on objects
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        // A Cheap trick to create a more real lighting
        // The light will always be a little brighter then objects
        // ambientStrength += 0.01;
        lightStrength += 0.01;
        // specularStrength += 0.01;
        // ambientStrength += 0.01f;
        if(lightStrength > 1.0f)
            lightStrength = 1.0f;
        // if(ambientStrength > 0.8f)
            // ambientStrength = 0.8f;
        // if(specularStrength > 1.0f)
            // specularStrength = 1.0f;
    }
    // If user presse DOWN key button - decrease lighting on objects
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        // ambientStrength -= 0.01f;
        lightStrength -= 0.01f;
        // ambientStrength -= 0.01f;
        if(lightStrength <= 0.0f)
            lightStrength = 0.0f;
        // if(ambientStrength <= 0.0f)
            // ambientStrength = 0.0f;
        // if(specularStrength <= 0.0f)
            // specularStrength = 0.0f;
    }
    // If user presses LEFT_SHIFT key button - increase camera movement
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.setMovementSpeed(SPEED * 5.0f);
    // Otherwise normal camera speed
    else
        camera.setMovementSpeed(SPEED);

    // If user presses ESC key button - exit program
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        exit(0);
}

int main()
{
    // Create window object
    glWindow window(TITLE, WIDTH, HEIGHT);

    // Set callback function to capture cursor (focus on window)
    glfwSetInputMode(window.getGlWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Set callback function to capture mouse movement
    glfwSetCursorPosCallback(window.getGlWindow(), mouse_callback);
    // Set callback function to capture mouse scrolling
    glfwSetScrollCallback(window.getGlWindow(), scroll_callback);

    // Setup Shaders
    // Create new colorShader object from colorShader files and compile colorShader program
    Shader colorShader(COLOR_V_SHADER_PATH.c_str(), COLOR_F_SHADER_PATH.c_str());
    Shader lightShader(LIGHT_V_SHADER_PATH.c_str(), LIGHT_F_SHADER_PATH.c_str());

    // Create new Mesh object
    // Mesh mesh(0, vertices, sizeof(vertices), nullptr, 0, texture, sizeof(texture), indices, sizeof(indices), GL_FLOAT, false, 0);
    Mesh mesh(0, vertices, sizeof(vertices), GL_FLOAT, false, 0);
    mesh.setNormals(normals);
    mesh.setIndices(indices, sizeof(indices));
    mesh.setTextureCoordinates(texture, sizeof(texture));
    // Initialise VAO (vertex array object) and VBO (vertex buffer object) for mesh
    mesh.initialise(GL_FLOAT, GL_FALSE);
    // Add indices (elements) attribute to mesh
    mesh.addAttribute(0, GL_ELEMENT_ARRAY_BUFFER, 3, 0, 0, indices, sizeof(indices), &mesh.elementBuffer);
    // Add texture attribute to mesh
    mesh.addAttribute(1, GL_ARRAY_BUFFER, 3, GL_FLOAT, false, normals, sizeof(normals), &mesh.normalsBuffer);
    // Unbind mesh from global state
    mesh.unbind();

    Mesh light(0, nullptr, 0, GL_FLOAT, false, 0);
    light.initialise(GL_FLOAT, GL_FALSE);

    // Use colorShader program before setting uniforms
    colorShader.use();
    // colorShader.setVec3("lightColor", glm::value_ptr(colors[0]));
    // Tell OpenGL to enable depth buffer
    Shader::enableDepth();

    // Use lightShader program before setting uniforms
    // lightShader.use();
    // lightShader.setVec3("color", glm::value_ptr(colors[0]));

    // Main loop
    // Run until window should close
    while (!window.isShouldClose())
    {
        processInput(window.getGlWindow());

        // Clear window to black screen
        glClearColor(0, 0, 0, 1);
        // Clear color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
            unsigned int moduloVal = 6;
            unsigned int lightCubeIndex = 0;
            glm::vec3 lightPosition;
            // If index devides by 3, cube will be a light source
            if(i == lightCubeIndex)
            {
                lightShader.use();
                // Set perspective projection matrix
                projection = glm::perspective(glm::radians(camera.getFov()), WIDTH / HEIGHT, 0.1f, 100.0f);
                lightShader.setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
                // Set view matrix
                view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
                lightShader.setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
                lightPosition = glm::vec3(1.0f + sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f), 0.0f);
                model = glm::translate(model, lightPosition);
                lightShader.setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
                // Set light object's light strength
                lightShader.setVec3("color", glm::value_ptr(colors[lightCubeIndex]));
                lightShader.setFloat("strength", lightStrength);
            }
            else
            {
                angle = glfwGetTime() * 15.0f * (((i + 1) % moduloVal) + 1);
                rotationVec = glm::vec3(1.0f, 0.5f, 0.2f);
                // Update uniform matrix in colorShader program
                colorShader.use();
                colorShader.setVec3("objectColor", glm::value_ptr(colors[i]));
                colorShader.setVec3("viewPosition", glm::value_ptr(camera.getPosition()));
                colorShader.setVec3("light.position", glm::value_ptr(lightPosition));
                colorShader.setVec3("light.ambient", glm::value_ptr(glm::vec3(1.0f) * lightStrength));
                colorShader.setVec3("light.diffuse", glm::value_ptr(glm::vec3(1.0f) * lightStrength));
                colorShader.setVec3("light.specular", glm::value_ptr(glm::vec3(1.0f) * lightStrength));

                colorShader.setVec3("material.ambient", glm::value_ptr(glm::vec3(0.5) * colors[i] * lightStrength));
                colorShader.setVec3("material.diffuse", glm::value_ptr(glm::vec3(0.8) * colors[i] * lightStrength));
                colorShader.setVec3("material.specular", glm::value_ptr(glm::vec3(1.0) * colors[i] * lightStrength));
                colorShader.setFloat("material.shininess", specularIntensity);
                // Calculate and set object normal vector so we can calculate in shader how much the light hits the object
                colorShader.setMatrix3fv("normalMatrix", 1, GL_TRUE, glm::value_ptr(glm::mat3(glm::inverse(model))));
                // Set perspective projection matrix
                projection = glm::perspective(glm::radians(camera.getFov()), WIDTH / HEIGHT, 0.1f, 100.0f);
                colorShader.setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
                // Set view matrix
                view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
                colorShader.setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
                model = glm::rotate(model, glm::radians(angle), rotationVec);
                colorShader.setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
            }
            // Render model
            mesh.render(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }

        // mesh.render(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

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
    colorShader.unbind();
    colorShader.clear();

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