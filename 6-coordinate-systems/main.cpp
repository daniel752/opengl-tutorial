#include "glWindow.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Window dimensions
GLfloat width = 800.0f, height = 600.0f;
std::string title = "Main Window";
// Working directory
std::string pwd = std::filesystem::current_path().string();
// Shader sources
std::string vShaderPath = pwd + "/../shaders/vShader.glsl";
std::string fShaderPath = pwd + "/../shaders/fShader.glsl";

// Model transformation matrix
glm::mat4 model;
// View transformation matrix
glm::mat4 view;
// Projection transformation matrix
glm::mat4 projection;
// Translation (location/movement) vector 3 (x,y,z) to move object
glm::vec3 translationVec(0.0f);
// Speed of object movement
float translationSpeed = 0.01;
// Scale value to multiply scaling matrix
float scaleValue = 0.2;
// Rotation value to multiply rotation matrix
float rotationValue = 0;

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

GLfloat texture[] =
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
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

void processInput(GLFWwindow* window)
{
    // If user presses ESC key button - exit program
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        exit(0);
}

int main()
{
    glWindow window(title, width, height);

    // Setup Shaders
    // Create new shader object from shader files and compile shader program
    Shader shader(vShaderPath.c_str(), fShaderPath.c_str());
    
    // Create new Mesh object to handle triangle shape.
    Mesh mesh(vertices, sizeof(vertices), nullptr, 0, texture, sizeof(texture), indices, sizeof(indices), GL_FLOAT, false, 0);
    // Initialise VAO (vertex array object) and VBO (vertex buffer object) for mesh
    mesh.initialise(GL_FLOAT, GL_FALSE);
    // Add indices (elements) attribute to mesh
    mesh.addAttribute(0, GL_ELEMENT_ARRAY_BUFFER, 3, 0, 0, indices, sizeof(indices), &mesh.elementBuffer);
    // Add texture attribute to mesh
    mesh.addAttribute(1, GL_ARRAY_BUFFER, 2, GL_FLOAT, GL_FALSE, texture, sizeof(texture), &mesh.textureBuffer);
    // Unbind mesh from global state
    mesh.unbind();

    // Textures
    Texture container(0, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, 0, pwd + "/../../assets/container.jpg", GL_TEXTURE0);
    Texture smileyFace(1, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, 0, 0, pwd + "/../../assets/awesomeface.png", GL_TEXTURE1);
    container.loadTexture();
    smileyFace.loadTexture();
    container.useTexture();
    smileyFace.useTexture();

    // Use shader program before setting uniforms
    shader.use();

    // Tell OpenGL to enable depth buffer
    shader.enableDepth();

    // Set uniform sampler2D at index 0 in sampler2D array to point to 0
    shader.setInt("textures[0]", 0);
    // Set uniform sampler2D at index 1 in sampler2D array to point to 1
    shader.setInt("textures[1]", 1);

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
            // If index devides by 2
            if(i % 2 == 0)
            {
                angle = glfwGetTime() * 15.0f * (i + 1);
                rotationVec = glm::vec3(1.0f, 0.5f, 0.2f);
            }
            // If index devides by 3
            if(i % 3 == 0)
            {
                angle = glfwGetTime() * 25.0f * (i + 1);
                rotationVec = glm::vec3(0.5f, 1.0f, 0.2f);
            }
            // Rotate model according to angle and rotation vector
            model = glm::rotate(model, glm::radians(angle), rotationVec);
            // Update uniform matrix in shader program
            shader.setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
            // Render model
            mesh.render(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }

        view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        shader.setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));

        projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
        shader.setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

        mesh.render(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        mesh.unbind();

        window.swapBuffers();
        glfwPollEvents();
    }
    
    mesh.clear();
    shader.unbind();
    shader.clear();

    return 0;
}
