cmake_minimum_required(VERSION 4.3)
project(MCPP)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(MCPP src/main.cpp
        src/glad.c
        src/window/VAO.cpp
        src/window/VAO.h
        src/window/Program.cpp
        src/window/Program.h
        src/window/Texture.cpp
        src/window/Texture.h
        src/player/Camera.cpp
        src/player/Camera.h
        src/player/InputHandler.cpp
        src/player/InputHandler.h)

target_include_directories(MCPP PRIVATE include)
include_directories(/usr/include/stb)

target_link_libraries(MCPP PRIVATE
        glfw
        OpenGL::GL
        ${CMAKE_DL_LIBS} # Required on Linux for glad to load OpenGL functions
)