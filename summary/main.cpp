#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>

#include "mesh.h"
#include "shader.h"
#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Constants for window dimensions
constexpr GLint WIDTH = 800, HEIGHT = 600;
// Title for the window
std::string TITLE = "Main Window";

// Path to the current working directory
std::string PWD = std::filesystem::current_path().string();

// Paths to the vertex and fragment shader source files
std::string VERTEX_SHADER_PATH = PWD + "/../shaders/vShader.glsl";
std::string FRAGMENT_SHADER_PATH = PWD + "/../shaders/fShader.glsl";

// GLuint program, uniformModel, uniformProjection;
std::vector<Mesh*> meshes;
std::vector<Shader> shaders;

float scale = 0.4;

const float TO_RADIANS = 3.14159 / 180;
float angle = 0.0;
float rotationSpeed = 1.0;

GLfloat projectionX = 0.0f, projectionY = 0.0f, projectionZ = -7.0f;

unsigned int indices[] = 
{
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
};

// Vertex data for the triangle
GLfloat TRIANGLE[] = 
{
    -1.0f, -1.0f, 0.0f,     // down left
    0.0f, -1.0f, 1.0f,       // far middle
    1.0f, -1.0f, 0.0f,       // down right
    0.0f, 1.0f, 0.0f        // top middle
};

int main()
{
    // Create new Window object with window 800x600 (width X height) with title "Main Window"
    Window window("Main Window", 800, 600);
    // Initialise window
    window.initialise();

    // Create new Shader object
    Shader* shader = new Shader();
    // Create vertex & fragment shaders from external files
    shader->createFromFiles(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());
    // Add shader to shaders vector
    shaders.push_back(*shader);

    // Creating new Mesh object to create and render triangle meshes
    Mesh* mesh = new Mesh();
    mesh->create(TRIANGLE, indices, 12, 12);
    meshes.push_back(mesh);
    mesh->create(TRIANGLE, indices, 12, 12);
    meshes.push_back(mesh);
    mesh->create(TRIANGLE, indices, 12, 12);
    meshes.push_back(mesh);

    GLint uniformProjection = 0, uniformModel = 0;

    // Setup projection (camera)
    // Field-of-view = 45
    // Aspect ratio is window width devided by window height
    // Near & far are responsible for setting the range of the projection, meaning
    // what's inside the range is seen by the projection and what's outside is discarded
    // Near is set to 0.1 from origin point (0, 0, 0) and far to 100
    glm::mat4 projection = glm::perspective(45.0f,  window.getBufferWidth() / window.getBufferHeight(), 0.1f, 100.0f);
    // Set projection's location in space
    projection = glm:: translate(projection, glm::vec3(projectionX, projectionY, projectionZ));

    // Keep window open until should close
    while(!window.isShouldClose())
    {
        // Clear background to black
        glClearColor(0, 0, 0, 1);
        // Clear color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Increase angle
        angle += rotationSpeed;

        // If angle is 360 degrees (full rotation) then set to 0, it's the same
        // and angle variable won't get huge
        if(angle == 360)
            angle = 0;

        // Use shader program
        shaders[0].useShader();
        uniformProjection = shaders[0].getProjectionLocation();
        uniformModel = shaders[0].getModelLocation();

        shaders[0].setMat4("projection", projection);

        // First Triangle
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, angle * TO_RADIANS, glm::vec3(1.0f, 0.0f, 0.0f));
        shaders[0].setMat4("model", model);
        meshes[0]->render();

        // Second Triangle
        model = glm::mat4(1.0f); // Reset the model matrix
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        model = glm::rotate(model, angle * TO_RADIANS, glm::vec3(-1.0f, 0.0f, 0.0f));
        shaders[0].setMat4("model", model);
        meshes[1]->render();

        // Third Triangle
        model = glm::mat4(1.0f); // Reset the model matrix
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
        model = glm::rotate(model, angle * TO_RADIANS, glm::vec3(-1.0f, 0.0f, 0.0f));
        shaders[0].setMat4("model", model);
        meshes[2]->render();

        glUseProgram(0);

        // GLFW swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        window.swapBuffers();
        glfwPollEvents();
    }

    // Cleanup and terminate GLFW
    // This time the glfwWindow will be destroyed when Window's destructor is called.
    // Class destructor is called when the scope of the object is terminated (NOT THE CASE FOR POINTERS!!!), when main function ends.
}