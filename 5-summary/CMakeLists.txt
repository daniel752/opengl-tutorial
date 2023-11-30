cmake_minimum_required(VERSION 3.12)
project(summary)

add_executable(main.o main.cpp mesh.cpp shader.cpp)

find_package(GLEW REQUIRED)
target_link_libraries(main.o GLEW::GLEW)

find_package(glfw3 REQUIRED)
target_link_libraries(main.o glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(main.o OpenGL::GL)