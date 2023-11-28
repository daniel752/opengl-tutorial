cmake_minimum_required(VERSION 3.12)
project(draw_shape)

# Add source files
add_executable(main.o main.cpp)

# Find and link GLEW
find_package(GLEW REQUIRED)
target_link_libraries(main.o GLEW::GLEW)

# Find and link GLFW
find_package(glfw3 REQUIRED)
target_link_libraries(main.o glfw)

# Find and link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(main.o OpenGL::GL)