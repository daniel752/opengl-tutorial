#include "glWindow.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"

#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window dimensions
GLfloat WIDTH = 800.0f, HEIGHT = 600.0f;
const std::string TITLE = "Main Window";
// Working directory
std::string PWD = std::filesystem::current_path().string();
// Shader sources
const std::string COLOR_V_SHADER_PATH = PWD + "/../shaders/color.vs";
const std::string COLOR_F_SHADER_PATH = PWD + "/../shaders/color.fs";
const std::string LIGHT_V_SHADER_PATH = PWD + "/../shaders/light.vs";
const std::string LIGHT_F_SHADER_PATH = PWD + "/../shaders/light.fs";

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
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

// Lighting set-up constants
const glm::vec3 DIR_LIGHT_AMBIENT_VEC = glm::vec3(0.05f, 0.05f, 0.05f);
const glm::vec3 DIR_LIGHT_DIFFUSE_VEC = glm::vec3(0.4f, 0.4f, 0.4f);
const glm::vec3 DIR_LIGHT_SPECULAR_VEC = glm::vec3(0.5f, 0.5f, 0.5f);
const glm::vec3 DIR_LIGHT_DIRECTION = glm::vec3(-0.2f, -1.0f, -0.3f);

const glm::vec3 SPOT_LIGHT_AMBIENT_VEC = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 SPOT_LIGHT_DIFFUSE_VEC = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 SPOT_LIGHT_SPECULAR_VEC = glm::vec3(1.0f, 1.0f, 1.0f);

const glm::vec3 POINT_LIGHT_AMBIENT_VEC = glm::vec3(0.05f, 0.05f, 0.05f);
const glm::vec3 POINT_LIGHT_DIFFUSE_VEC = glm::vec3(0.8f, 0.8f, 0.8f);
const glm::vec3 POINT_LIGHT_SPECULAR_VEC = glm::vec3(1.0f, 1.0f, 1.0f);

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

float alpha = 1.0f;

glm::vec4 backgroundColor = DEFAULT_BACKGROUND_COLOR;
glm::vec3 lightColor = DEFAULT_LIGHT_COLOR;

glm::vec3 colors[] = 
{
    glm::vec3(1.0, 0.0, 0.0),     // Red
    glm::vec3(1.0, 0.5, 0.0),     // Orange
    glm::vec3(1.0, 1.0, 0.0),     // Yellow
    glm::vec3(0.0, 1.0, 0.0),     // Green
    glm::vec3(0.0, 0.0, 1.0),     // Blue
    glm::vec3(0.5, 0.0, 1.0),     // Indigo
    glm::vec3(0.8, 0.6, 0.7)      // Violet
};


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

// Float array to represent cube normals.
// A Cube has 6 vertices on each face (Every square is made by triangle, so 2 triangles means 6 vertices)
// so each 6 rows represent a face on the cube.
// Each face has a different direction with it's normal vector.
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

// Float array for texture coordinates.
// Texture coordinates are in 2D and that's why we have only two components (X,Y) for each vertex.
// Texture coordinates start from bottom-left (0,0) of image.
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

// Vector 3 (X,Y,Z) array for cube positions in space
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

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f),
    glm::vec3( 10.0f, 5.0f, -10.0f)
};  

// Function to handle user input from keyboard
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
    
    // If user presses LEFT_SHIFT key button - increase camera movement
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.setMovementSpeed(SPEED * 5.0f);
    // Otherwise normal camera speed
    else
        camera.setMovementSpeed(SPEED);

    // Default light set-up when user presses "0" key
    if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
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
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        directionalAmbient = (DIR_LIGHT_AMBIENT_VEC + glm::vec3(0.1f)) * colors[1];
        directionalDiffuse = (DIR_LIGHT_DIFFUSE_VEC + glm::vec3(0.55f)) * colors[1];
        directionalSpecular = (DIR_LIGHT_SPECULAR_VEC - glm::vec3(0.4f)) * colors[1];

        spotLightAmbient = glm::vec3(0.0f);
        spotLightDiffuse = glm::vec3(0.0f);
        spotLightSpecular = glm::vec3(0.0f);

        pointLightAmbient = glm::vec3(0.0f);
        pointLightDiffuse = glm::vec3(0.0f);
        pointLightSpecular = glm::vec3(0.0f);

        backgroundColor = DESERT_BACKGROUND_COLOR;

        lightColor = colors[1];

        specularIntensity = 1;

        cutOff = 0;
        outerCutOff = 0;

        constant = CONSTANT;
        linear = LINEAR;
        quadratic = QUADRATIC;
    }

    // Simple factory light set-up when user presses "2" key
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        directionalAmbient = (DIR_LIGHT_AMBIENT_VEC + glm::vec3(0.05f)) * colors[4];
        directionalDiffuse = (DIR_LIGHT_DIFFUSE_VEC + glm::vec3(0.1f)) * colors[4];
        directionalSpecular = (DIR_LIGHT_SPECULAR_VEC - glm::vec3(0.2f)) * colors[4];

        spotLightAmbient = SPOT_LIGHT_AMBIENT_VEC * colors[4];
        spotLightDiffuse = SPOT_LIGHT_DIFFUSE_VEC * colors[4];
        spotLightSpecular = SPOT_LIGHT_SPECULAR_VEC * colors[4];

        pointLightAmbient = POINT_LIGHT_AMBIENT_VEC * colors[4];
        pointLightDiffuse = POINT_LIGHT_DIFFUSE_VEC * colors[4];
        pointLightSpecular = POINT_LIGHT_SPECULAR_VEC * colors[4];

        backgroundColor = FACTORY_BACKGROUND_COLOR;

        lightColor = colors[4];

        specularIntensity = SPECULAR_INTENSITY;

        cutOff = CUT_OFF;
        outerCutOff = OUTER_CUT_OFF;

        constant = CONSTANT;
        linear = LINEAR;
        quadratic = QUADRATIC;
    }

    // Simple horror light set-up when user presses "3" key
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        directionalAmbient = glm::vec3(0.0f);
        directionalDiffuse = glm::vec3(0.0f);
        directionalSpecular = glm::vec3(0.0f);

        spotLightAmbient = SPOT_LIGHT_AMBIENT_VEC;
        spotLightDiffuse = SPOT_LIGHT_DIFFUSE_VEC;
        spotLightSpecular = SPOT_LIGHT_SPECULAR_VEC;

        pointLightAmbient = POINT_LIGHT_AMBIENT_VEC - glm::vec3(0.02f);
        pointLightDiffuse = POINT_LIGHT_DIFFUSE_VEC - glm::vec3(0.2f);
        pointLightSpecular = POINT_LIGHT_SPECULAR_VEC - glm::vec3(0.2f);

        backgroundColor = DEFAULT_BACKGROUND_COLOR;

        lightColor = glm::vec3(1.0f);   // White color

        specularIntensity = SPECULAR_INTENSITY;

        cutOff = 5.0f;
        outerCutOff = 15.0f;

        constant = CONSTANT;
        linear = 0.3f;
        quadratic = 0.1f;
    }

    // Simple lab light set-up when user presses "4" key
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        directionalAmbient = DIR_LIGHT_AMBIENT_VEC * colors[3];
        directionalDiffuse = DIR_LIGHT_DIFFUSE_VEC * colors[3];
        directionalSpecular = DIR_LIGHT_SPECULAR_VEC * colors[3];

        spotLightAmbient = SPOT_LIGHT_AMBIENT_VEC;
        spotLightDiffuse = SPOT_LIGHT_DIFFUSE_VEC;
        spotLightSpecular = SPOT_LIGHT_SPECULAR_VEC;

        pointLightAmbient = POINT_LIGHT_AMBIENT_VEC * colors[3];
        pointLightDiffuse = POINT_LIGHT_DIFFUSE_VEC;
        pointLightSpecular = POINT_LIGHT_SPECULAR_VEC;

        backgroundColor = LAB_BACKGROUND_COLOR;

        lightColor = colors[3];

        specularIntensity = SPECULAR_INTENSITY;

        cutOff = CUT_OFF;
        outerCutOff = OUTER_CUT_OFF;

        constant = CONSTANT;
        linear = LINEAR;
        quadratic = QUADRATIC;
    }

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

    // Create new Mesh object for cubes
    Mesh mesh(0, vertices, sizeof(vertices), GL_FLOAT, false, 0);
    mesh.setNormals(normals);
    mesh.setIndices(indices, sizeof(indices));
    mesh.setTextureCoordinates(texture, sizeof(texture));
    // Initialise VAO (vertex array object) and VBO (vertex buffer object) for mesh
    mesh.initialise(GL_FLOAT, GL_FALSE);
    // Add indices (elements) attribute to mesh
    mesh.addAttribute(0, GL_ELEMENT_ARRAY_BUFFER, 3, 0, 0, indices, sizeof(indices), &mesh.elementBuffer);
    // Add normals attribute to mesh
    mesh.addAttribute(1, GL_ARRAY_BUFFER, 3, GL_FLOAT, false, normals, sizeof(normals), &mesh.normalsBuffer);
    // Add texture attribute to mesh
    mesh.addAttribute(2, GL_ARRAY_BUFFER, 2, GL_FLOAT, false, texture, sizeof(texture), &mesh.textureBuffer);
    // Unbind mesh from global state
    mesh.unbind();

    GLuint diffuseMap, specularMap, emissionMap;
    colorShader.use();
    // Load diffuse map texture
    colorShader.loadTexture(PWD + "/../../assets/container2.png", &diffuseMap, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, GL_RGBA, 0, GL_RGBA, GL_UNSIGNED_BYTE);
    // Point material's diffuse to diffuse map at texture unit 0
    colorShader.setInt("material.diffuse", 0);

    // Load specular map texture
    colorShader.loadTexture(PWD + "/../../assets/container2_specular.png", &specularMap, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, GL_RGBA, 0, GL_RGBA, GL_UNSIGNED_BYTE);
    // Point material's specular to specular map at texture unit 1
    colorShader.setInt("material.specular", 1);

    // Unbind shader from global state
    colorShader.unbind();

    // Create new Mesh object for light cube
    Mesh light(0, nullptr, 0, GL_FLOAT, false, 0);
    light.initialise(GL_FLOAT, GL_FALSE);

    // Use colorShader program before setting uniforms
    colorShader.use();
    // Tell OpenGL to enable depth buffer, so we be able to perceive depth
    Shader::enableDepth();

    // Main loop
    // Run until window should close
    while (!window.isShouldClose())
    {
        processInput(window.getGlWindow());

        // Clear window to black screen
        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.t);
        // Clear color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        colorShader.use();

        colorShader.setFloat("material.shininess", specularIntensity);

        colorShader.setVec3("directionalLight.direction", glm::value_ptr(lightDirection));
        colorShader.setVec3("directionalLight.ambient", glm::value_ptr(directionalAmbient));
        colorShader.setVec3("directionalLight.diffuse", glm::value_ptr(directionalDiffuse));
        colorShader.setVec3("directionalLight.specular", glm::value_ptr(directionalSpecular));

        for(unsigned int i = 0; i < 5; i++)
        {
            std::stringstream strPosition, strConstant, strLinear, strQudratic, strAmbient, strDiffuse, strSpecular;
            
            strPosition << "pointLights[" << i << "].position";
            colorShader.setVec3(strPosition.str(), glm::value_ptr(pointLightPositions[i]));

            strConstant << "pointLights[" << i << "].constant";
            colorShader.setFloat(strPosition.str(), constant);

            strLinear << "pointLights[" << i << "].linear";
            colorShader.setFloat(strLinear.str(), linear);

            strQudratic << "pointLights[" << i << "].quadratic";
            colorShader.setFloat(strQudratic.str(), quadratic);

            strAmbient << "pointLights[" << i << "].ambient";
            colorShader.setVec3(strAmbient.str(), glm::value_ptr(glm::vec3(pointLightAmbient)));

            strDiffuse << "pointLights[" << i << "].diffuse";
            colorShader.setVec3(strDiffuse.str(), glm::value_ptr(glm::vec3(pointLightDiffuse)));

            strSpecular << "pointLights[" << i << "].specular";
            colorShader.setVec3(strSpecular.str(), glm::value_ptr(glm::vec3(pointLightSpecular)));
        }

        colorShader.setVec3("spotLight.position", glm::value_ptr(camera.getPosition()));
        colorShader.setVec3("spotLight.direction", glm::value_ptr(glm::vec3(camera.getFront().x, camera.getFront().y, camera.getFront().z)));
        colorShader.setVec3("spotLight.ambient", glm::value_ptr(spotLightAmbient));
        colorShader.setVec3("spotLight.diffuse", glm::value_ptr(spotLightDiffuse));
        colorShader.setVec3("spotLight.specular", glm::value_ptr(spotLightSpecular));
        colorShader.setFloat("spotLight.costant", constant);
        colorShader.setFloat("spotLight.linear", linear);
        colorShader.setFloat("spotLight.quadratic", quadratic);
        colorShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(cutOff)));
        colorShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(outerCutOff)));

        // Set perspective projection matrix
        projection = glm::perspective(glm::radians(camera.getFov()), WIDTH / HEIGHT, 0.1f, 100.0f);
        colorShader.setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

        // Set view matrix
        view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
        colorShader.setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        colorShader.setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));

        // Set texture unit 0 active (unnecessary if we have only one texture, texture unit 0 is the default)
        glActiveTexture(GL_TEXTURE0);
        // Bind "diffuseMap" to texture unit 0
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // Set texture unit 1 active
        glActiveTexture(GL_TEXTURE1);
        // Bind "specularMap" to texture unit 1
        glBindTexture(GL_TEXTURE_2D, specularMap);

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

            // Simple use of GLFW time function with modulo to have different angles to apply on the cubes
            angle = glfwGetTime() * 15.0f * (((i + 1) % moduloVal) + 1);
            // How much the rotation affects each axis, 1 is full effect and 0 is none
            rotationVec = glm::vec3(1.0f, 0.5f, 0.2f);

            // Rotate cube
            model = glm::rotate(model, glm::radians(angle), rotationVec);

            // Calculate and set object normal vector so we can calculate in shader how much the light hits the object
            colorShader.setMatrix3fv("normalMatrix", 1, GL_TRUE, glm::value_ptr(glm::mat3(glm::inverse(model))));

            // Set changes made to model
            colorShader.setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));

            // Render model
            mesh.render(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }

        lightShader.use();
        lightShader.setVec3("color", glm::value_ptr(lightColor));

        lightShader.setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
        lightShader.setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));

        for(unsigned int i = 0; i < 5; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
            light.render(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);    
        }
        lightShader.unbind();

        // Unbind mesh from global state
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
    colorShader.clear();
    lightShader.clear();

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