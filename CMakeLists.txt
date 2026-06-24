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
        src/player/InputHandler.h
        src/world/BlockData.hpp
        src/world/ChunkGenerator.cpp
        src/world/ChunkGenerator.h
        src/world/Chunk.cpp
        src/world/Chunk.h
        src/window/TextureManager.cpp
        src/window/TextureManager.h
        src/imgui/imgui.cpp
        src/imgui/imgui_demo.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui_tables.cpp
        src/imgui/imgui_widgets.cpp
        include/imgui/imconfig.h
        include/imgui/imgui.h
        include/imgui/imgui_internal.h
        include/imgui/imstb_rectpack.h
        include/imgui/imstb_textedit.h
        include/imgui/imstb_truetype.h
        include/imgui/imgui_impl_glfw.h
        src/imgui/imgui_impl_glfw.cpp
        src/imgui/imgui_impl_opengl3.cpp
        include/imgui/imgui_impl_opengl3.h
        include/imgui/imgui_impl_opengl3_loader.h
        src/player/UI.cpp
        src/player/UI.h)

target_include_directories(MCPP PRIVATE include src)
target_include_directories(MCPP PRIVATE ./include/imgui src/imgui)
include_directories(/usr/include/stb)

target_link_libraries(MCPP PRIVATE
        glfw
        OpenGL::GL
        ${CMAKE_DL_LIBS} # Required on Linux for glad to load OpenGL functions
)