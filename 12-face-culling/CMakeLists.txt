cmake_minimum_required(VERSION 3.12)

project(face_culling)

add_executable(main.o main.cpp glWindow.cpp camera.cpp shader.cpp)

find_package(GLEW REQUIRED)
target_link_libraries(main.o GLEW::GLEW)

find_package(glfw3 REQUIRED)
target_link_libraries(main.o glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(main.o OpenGL::GL)

find_package(assimp REQUIRED)
target_link_libraries(main.o assimp)