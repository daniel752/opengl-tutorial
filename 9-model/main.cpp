#include "glWindow.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "directionalLight.h"
#include "pointLight.h"
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


// CONSTANTS
// Lighting
// Directional light
const glm::vec3 DIR_LIGHT_AMBIENT_VEC = glm::vec3(0.5f, 0.5f, 0.5f);
const glm::vec3 DIR_LIGHT_DIFFUSE_VEC = glm::vec3(0.4f, 0.4f, 0.4f);
const glm::vec3 DIR_LIGHT_SPECULAR_VEC = glm::vec3(0.5f, 0.5f, 0.5f);
const glm::vec3 DIR_LIGHT_DIRECTION = glm::vec3(-0.2f, -1.0f, -0.3f);
// Directional light
// Spot light
const glm::vec3 SPOT_LIGHT_AMBIENT_VEC = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 SPOT_LIGHT_DIFFUSE_VEC = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 SPOT_LIGHT_SPECULAR_VEC = glm::vec3(1.0f, 1.0f, 1.0f);
// Spot light
// Point light
const glm::vec3 POINT_LIGHT_AMBIENT_VEC = glm::vec3(0.05f, 0.05f, 0.05f);
const glm::vec3 POINT_LIGHT_DIFFUSE_VEC = glm::vec3(0.8f, 0.8f, 0.8f);
const glm::vec3 POINT_LIGHT_SPECULAR_VEC = glm::vec3(1.0f, 1.0f, 1.0f);
// Point light

// Background
const glm::vec4 DEFAULT_BACKGROUND_COLOR = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);  // Black
const glm::vec4 DESERT_BACKGROUND_COLOR = glm::vec4(0.6882f, 0.5039f, 0.3039f, 1.0f); // Light brown
const glm::vec4 FACTORY_BACKGROUND_COLOR = glm::vec4(0.0078f, 0.0353f, 0.0806f, 1.0f);
const glm::vec4 LAB_BACKGROUND_COLOR = glm::vec4(0.955f, 0.955f, 0.955f, 1.0f);

const float CONSTANT = 1.0f;
const float LINEAR = 0.09f;
const float QUADRATIC = 0.032f;
const float SPECULAR_INTENSITY = 32;
const float CUT_OFF = 15.0f;
const float OUTER_CUT_OFF = 17.0f;

const glm::vec3 DEFAULT_LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);   // White

// Window configuration
GLfloat width = 800.0f, height = 600.0f;
std::string title = "Main Window";
bool isFullscreen = false;
// Working directory
std::string pwd = std::filesystem::current_path().string();
// Shader sources
std::string lightVShaderPath = pwd + "/../shaders/light.vs";
std::string lightFShaderPath = pwd + "/../shaders/light.fs";
std::string meshVShaderPath = pwd + "/../shaders/mesh.vs";
std::string meshFShaderPath = pwd + "/../shaders/mesh.fs";
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

// DirectionalLight set-up
glm::vec3 directionalAmbient = DIR_LIGHT_AMBIENT_VEC;
glm::vec3 directionalDiffuse = DIR_LIGHT_DIFFUSE_VEC;
glm::vec3 directionalSpecular = DIR_LIGHT_SPECULAR_VEC;
glm::vec3 lightDirection = DIR_LIGHT_DIRECTION;

// SpotLight set-up
glm::vec3 spotLightAmbient = SPOT_LIGHT_AMBIENT_VEC;
glm::vec3 spotLightDiffuse = SPOT_LIGHT_DIFFUSE_VEC;
glm::vec3 spotLightSpecular = SPOT_LIGHT_SPECULAR_VEC;
float cutOff = 15.0f;
float outerCutOff = 17.5f;

// PointLight set-up
glm::vec3 pointLightAmbient = POINT_LIGHT_AMBIENT_VEC;
glm::vec3 pointLightDiffuse = POINT_LIGHT_DIFFUSE_VEC;
glm::vec3 pointLightSpecular = POINT_LIGHT_SPECULAR_VEC;

float specularIntensity = SPECULAR_INTENSITY;

float constant = CONSTANT;
float linear = LINEAR;
float quadratic = QUADRATIC;

float lightIntensity = 0.5f;
float lightAlpha = 1.0f;

glm::vec4 backgroundColor = DEFAULT_BACKGROUND_COLOR;
glm::vec3 lightColor = DEFAULT_LIGHT_COLOR;

glm::vec3 pointLightPositions[] = 
{
	glm::vec3( 0.7f,  0.2f,  7.0f),
	glm::vec3( 3.5f, -3.5f, -6.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  3.5f, -6.0f),
    glm::vec3( 10.0f, 5.0f, 10.0f)
};

glm::vec3 colors[] = 
{
    glm::vec3(1.0f, 1.0f, 1.0f),     // White
    glm::vec3(0.0f, 0.0f, 0.0f),     // Black
    glm::vec3(1.0f, 0.0f, 0.0f),     // Red
    glm::vec3(1.0f, 0.5f, 0.0f),     // Orange
    glm::vec3(1.0f, 1.0f, 0.0f),     // Yellow
    glm::vec3(0.0f, 1.0f, 0.0f),     // Green
    glm::vec3(0.0f, 0.0f, 1.0f),     // Blue
    glm::vec3(0.5f, 0.0f, 1.0f),     // Indigo
    glm::vec3(0.8f, 0.6f, 0.7f)      // Violet
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
    stbi_set_flip_vertically_on_load(true);

    // Build and compile shaders
    // -------------------------
    Shader meshShader(meshVShaderPath.c_str(), meshFShaderPath.c_str());
    Shader lightShader(lightVShaderPath.c_str(), lightFShaderPath.c_str());

    Shader::enableDepth();

    // Load models
    // -----------
    Model backpack(modelPath.c_str());
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

        // Set background color and clear color buffer and depth buffer
        // ------------------------------------------------------------
        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.t);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enabling our shader before using it's uniforms
        // ----------------------------------------------
        lightShader.use();

        lightShader.setFloat("material.shininess", specularIntensity);

        // Directional light creation
        // --------------------------
        DirectionalLight directionalLight(lightDirection, directionalAmbient, directionalDiffuse, directionalSpecular);
        directionalLight.load(lightShader.getID());

        // Point lights creation
        // ---------------------
        std::vector<PointLight> pointLights;
        for(unsigned int i = 0; i < 5; i++)
        {
            pointLights.push_back(PointLight(pointLightPositions[i], constant, linear, quadratic, pointLightAmbient, pointLightDiffuse, pointLightSpecular));
            pointLights[i].injectIndex(i);
            pointLights[i].load(lightShader.getID());
        }

        // Transformations for view and projection
        // ---------------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), width / height, 0.1f, 100.0f);
        glm::mat4 view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
        lightShader.setMatrix4fv("projection", 1, GL_FALSE, projection);
        lightShader.setMatrix4fv("view", 1, GL_FALSE, view);

        // Transformations for model
        // -------------------------
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        lightShader.setMatrix3fv("normalMatrix", 1, GL_TRUE, glm::mat3(glm::inverse(model)));
        lightShader.setMatrix4fv("model", 1, GL_FALSE, model);

        // Render model
        // ------------
        backpack.draw(lightShader);
        lightShader.unbind();

        // Light cubes creation
        // --------------------
        meshShader.use();
        meshShader.setVec3("color", glm::value_ptr(lightColor));
        meshShader.setMatrix4fv("projection", 1, GL_FALSE, projection);
        meshShader.setMatrix4fv("view", 1, GL_FALSE, view);

        for(unsigned int i = 0; i < 5; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.01f));
            meshShader.setMatrix4fv("model", 1, GL_FALSE, model);
            cube.draw(meshShader);
        }
        meshShader.unbind();

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

    // Default light set-up when user presses "0" key
    if(key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        directionalAmbient = DIR_LIGHT_AMBIENT_VEC;
        directionalDiffuse = DIR_LIGHT_DIFFUSE_VEC;
        directionalSpecular = DIR_LIGHT_SPECULAR_VEC;

        spotLightAmbient = SPOT_LIGHT_AMBIENT_VEC;
        spotLightDiffuse = SPOT_LIGHT_DIFFUSE_VEC;
        spotLightSpecular = SPOT_LIGHT_SPECULAR_VEC;

        pointLightAmbient = POINT_LIGHT_AMBIENT_VEC;
        pointLightDiffuse = POINT_LIGHT_DIFFUSE_VEC;
        pointLightSpecular = POINT_LIGHT_SPECULAR_VEC;

        backgroundColor = DEFAULT_BACKGROUND_COLOR;

        lightColor = DEFAULT_LIGHT_COLOR;

        specularIntensity = SPECULAR_INTENSITY;

        cutOff = CUT_OFF;
        outerCutOff = OUTER_CUT_OFF;

        constant = CONSTANT;
        linear = LINEAR;
        quadratic = QUADRATIC;
    }

    // Simple Day light set-up when user presses "1" key
    if(key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        lightColor = colors[4];
        lightAlpha = 0.5f;

        directionalAmbient = (DIR_LIGHT_AMBIENT_VEC + glm::vec3(0.2f)) * (lightColor * lightAlpha);
        directionalDiffuse = (DIR_LIGHT_DIFFUSE_VEC + glm::vec3(0.4f)) * (lightColor * lightAlpha);
        directionalSpecular = (DIR_LIGHT_SPECULAR_VEC - glm::vec3(0.4f)) * (lightColor * lightAlpha);

        spotLightAmbient = glm::vec3(0.0f);
        spotLightDiffuse = glm::vec3(0.0f);
        spotLightSpecular = glm::vec3(0.0f);


        pointLightAmbient = POINT_LIGHT_AMBIENT_VEC * (lightColor * lightAlpha);
        pointLightDiffuse = POINT_LIGHT_DIFFUSE_VEC * (lightColor * lightAlpha);
        pointLightSpecular = POINT_LIGHT_SPECULAR_VEC * (lightColor * lightAlpha);

        backgroundColor = DESERT_BACKGROUND_COLOR;

        specularIntensity = 1;

        cutOff = 0;
        outerCutOff = 0;

        constant = CONSTANT;
        linear = LINEAR;
        quadratic = QUADRATIC;
    }

    // Simple under the sea light set-up when user presses "2" key
    if(key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        lightColor = colors[6];
        lightAlpha = 0.5f;

        directionalAmbient = DIR_LIGHT_AMBIENT_VEC * lightColor * lightAlpha;
        directionalDiffuse = DIR_LIGHT_DIFFUSE_VEC * lightColor * lightAlpha;
        directionalSpecular = DIR_LIGHT_SPECULAR_VEC * lightColor * lightAlpha;

        spotLightAmbient = SPOT_LIGHT_AMBIENT_VEC * lightColor;
        spotLightDiffuse = SPOT_LIGHT_DIFFUSE_VEC * lightColor;
        spotLightSpecular = SPOT_LIGHT_SPECULAR_VEC * lightColor;

        pointLightAmbient = POINT_LIGHT_AMBIENT_VEC * lightColor * lightAlpha;
        pointLightDiffuse = POINT_LIGHT_DIFFUSE_VEC * lightColor * lightAlpha;
        pointLightSpecular = POINT_LIGHT_SPECULAR_VEC * lightColor * lightAlpha;

        backgroundColor = FACTORY_BACKGROUND_COLOR;

        specularIntensity = SPECULAR_INTENSITY;

        cutOff = CUT_OFF;
        outerCutOff = OUTER_CUT_OFF;

        constant = CONSTANT;
        linear = LINEAR;
        quadratic = QUADRATIC;
    }

    // Simple horror light set-up when user presses "3" key
    if(key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        lightColor = colors[0];
        lightAlpha = 0.3f;

        directionalAmbient = (DIR_LIGHT_AMBIENT_VEC - glm::vec3(0.15f)) * lightColor * lightAlpha;
        directionalDiffuse = (DIR_LIGHT_DIFFUSE_VEC - glm::vec3(0.2f)) * lightColor * lightAlpha;
        directionalSpecular = (DIR_LIGHT_SPECULAR_VEC - glm::vec3(0.2f)) * lightColor * lightAlpha;

        spotLightAmbient = SPOT_LIGHT_AMBIENT_VEC;
        spotLightDiffuse = SPOT_LIGHT_DIFFUSE_VEC;
        spotLightSpecular = SPOT_LIGHT_SPECULAR_VEC;

        pointLightAmbient = POINT_LIGHT_AMBIENT_VEC * lightColor * lightAlpha;
        pointLightDiffuse = POINT_LIGHT_DIFFUSE_VEC * lightColor * lightAlpha;
        pointLightSpecular = POINT_LIGHT_SPECULAR_VEC * lightColor * lightAlpha;

        backgroundColor = DEFAULT_BACKGROUND_COLOR;

        specularIntensity = SPECULAR_INTENSITY;

        cutOff = 5.0f;
        outerCutOff = 15.0f;

        constant = CONSTANT;
        linear = 0.3f;
        quadratic = 0.1f;
    }

    // Simple lab light set-up when user presses "4" key
    if(key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        lightColor = colors[5];
        lightAlpha = 0.5f;

        directionalAmbient = (DIR_LIGHT_AMBIENT_VEC + glm::vec3(0.1f)) * lightColor * lightAlpha;
        directionalDiffuse = (DIR_LIGHT_DIFFUSE_VEC + glm::vec3(0.1f)) * lightColor * lightAlpha;
        directionalSpecular = (DIR_LIGHT_SPECULAR_VEC + glm::vec3(0.1f)) * lightColor * lightAlpha;

        spotLightAmbient = SPOT_LIGHT_AMBIENT_VEC;
        spotLightDiffuse = SPOT_LIGHT_DIFFUSE_VEC;
        spotLightSpecular = SPOT_LIGHT_SPECULAR_VEC;

        pointLightAmbient = (POINT_LIGHT_AMBIENT_VEC + glm::vec3(0.1f)) * lightColor * lightAlpha;
        pointLightDiffuse = (POINT_LIGHT_DIFFUSE_VEC + glm::vec3(0.1f)) * lightColor * lightAlpha;
        pointLightSpecular = (POINT_LIGHT_SPECULAR_VEC + glm::vec3(0.1f)) * lightColor * lightAlpha;

        backgroundColor = LAB_BACKGROUND_COLOR;

        specularIntensity = SPECULAR_INTENSITY;

        cutOff = CUT_OFF;
        outerCutOff = OUTER_CUT_OFF;

        constant = CONSTANT;
        linear = LINEAR;
        quadratic = QUADRATIC;
    }

    // If user presses ESC key button - exit program
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        exit(0);
}