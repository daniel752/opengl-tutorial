#include "glWindow.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Array of colors
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
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        specularIntensity *= 2;
        if(specularIntensity >= 256)
            specularIntensity = 256;
    }
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        specularIntensity /= 2;
        if(specularIntensity <= 1)
            specularIntensity = 1;
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

    // Load emission map texture
    colorShader.loadTexture(PWD + "/../../assets/matrix.jpg", &emissionMap, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, GL_RGB, 0, GL_RGB, GL_UNSIGNED_BYTE);
    // Point material's emission to emission map at texture unit 2
    colorShader.setInt("material.emission", 2);
    // Unbind shader from global state
    colorShader.unbind();

    // Create new Mesh object for light cube
    Mesh light(0, nullptr, 0, GL_FLOAT, false, 0);
    light.initialise(GL_FLOAT, GL_FALSE);

    // Use colorShader program before setting uniforms
    colorShader.use();
    // Tell OpenGL to enable depth buffer, so we be able to perceive depth
    Shader::enableDepth();

    // Use lightShader program before setting uniforms
    lightShader.use();
    // Set light cube's color to white
    lightShader.setVec3("color", glm::value_ptr(colors[0]));

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

                // Simple use of sin to make the light cube move around a bit
                lightPosition = glm::vec3(1.0f + sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f), 0.0f);
                // Translate (move) light cube to updated position
                model = glm::translate(model, lightPosition);
                // Make light cube a fith (1/5) of it's normal size
                model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
                // Set all changes made to model
                lightShader.setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));

                // Set light object's light strength
                lightShader.setFloat("strength", lightStrength);
            }
            else
            {
                // Simple use of GLFW time function with modulo to have different angles to apply on the cubes
                angle = glfwGetTime() * 15.0f * (((i + 1) % moduloVal) + 1);
                // How much the rotation affects each axis, 1 is full effect and 0 is none
                rotationVec = glm::vec3(1.0f, 0.5f, 0.2f);
                // Update uniform matrix in colorShader program
                colorShader.use();
                // Set light properties
                colorShader.setVec3("viewPosition", glm::value_ptr(camera.getPosition()));
                colorShader.setVec3("light.position", glm::value_ptr(lightPosition));
                colorShader.setVec3("light.ambient", glm::value_ptr(glm::vec3(1.0f) * lightStrength));
                colorShader.setVec3("light.diffuse", glm::value_ptr(glm::vec3(1.0f) * lightStrength));
                colorShader.setVec3("light.specular", glm::value_ptr(glm::vec3(1.0f) * lightStrength));

                // Set material properties
                colorShader.setVec3("material.ambient", glm::value_ptr(glm::vec3(0.5) * lightStrength));
                colorShader.setVec3("material.diffuse", glm::value_ptr(glm::vec3(0.8) * lightStrength));
                colorShader.setVec3("material.specular", glm::value_ptr(glm::vec3(1.0) * lightStrength));
                colorShader.setFloat("material.shininess", specularIntensity);

                // Calculate and set object normal vector so we can calculate in shader how much the light hits the object
                colorShader.setMatrix3fv("normalMatrix", 1, GL_TRUE, glm::value_ptr(glm::mat3(glm::inverse(model))));

                // Set perspective projection matrix
                projection = glm::perspective(glm::radians(camera.getFov()), WIDTH / HEIGHT, 0.1f, 100.0f);
                colorShader.setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

                // Set view matrix
                view = camera.calculateLookAtMatrix(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
                colorShader.setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));

                // Rotate cube
                model = glm::rotate(model, glm::radians(angle), rotationVec);
                // Set changes made to model
                colorShader.setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));

                // Set texture unit 0 active (unnecessary if we have only one texture, texture unit 0 is the default)
                glActiveTexture(GL_TEXTURE0);
                // Bind "diffuseMap" to texture unit 0
                glBindTexture(GL_TEXTURE_2D, diffuseMap);

                // Set texture unit 1 active
                glActiveTexture(GL_TEXTURE1);
                // Bind "specularMap" to texture unit 1
                glBindTexture(GL_TEXTURE_2D, specularMap);

                // Set texture unit 2 active
                glActiveTexture(GL_TEXTURE2);
                // bind "emissionMap" to texture unit 2
                glBindTexture(GL_TEXTURE_2D, emissionMap);
            }
            // Render model
            mesh.render(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }

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