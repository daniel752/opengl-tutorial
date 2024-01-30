cmake_minimum_required(VERSION 3.12)

project(light)

add_executable(main.o main.cpp glWindow.cpp mesh.cpp shader.cpp texture.cpp stb_image.cpp camera.cpp directionalLight.cpp pointLight.cpp spotLight.cpp)

find_package(GLEW REQUIRED)
target_link_libraries(main.o GLEW::GLEW)

find_package(glfw3 REQUIRED)
target_link_libraries(main.o glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(main.o OpenGL::GL)