cmake_minimum_required(VERSION 3.12)

project(advanced_glsl)

# Add your executable
add_executable(main.o main.cpp glWindow.cpp camera.cpp shader.cpp)

# Find required packages
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Link your executable with the found packages
target_link_libraries(main.o PRIVATE 
    GLEW::GLEW
    glfw
    OpenGL::GL
)