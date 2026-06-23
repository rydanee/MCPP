cmake_minimum_required(VERSION 4.3)
project(MCPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(MCPP src/main.cpp
        src/window/VAO.cpp
        src/window/VAO.h)

target_link_libraries(MCPP glfw)
target_link_libraries(MCPP glm)
target_link_libraries(MCPP glad)
find_package(OpenGL REQUIRED)
target_link_libraries(MCPP OpenGL::GL)
