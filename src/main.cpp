#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window/VAO.h"
#include "window/Program.h"
#include "window/Texture.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

VAO *vao;
Program *program;
Texture *texture;

static void onLoad() {
    vao = new VAO();
    program = new Program();
    texture = new Texture();
    texture->loadFromFile("./assets/textures/hotdog.jpg");

    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, 0.5f,  0.0f), // bl
        glm::vec3( 0.5f, 0.5f,  0.0f), // br
        glm::vec3( 0.5f,  -0.5f,  0.0f),  // tc
        glm::vec3( -0.5f, -0.5f, 0.0f)
    };
    std::vector<glm::vec2> texCoords = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f)
    };

    vao->addVertexBufferObject(vertices);
    vao->addVertexBufferObject(texCoords);
    vao->addIndices({0, 1, 2, 2, 3, 0});

    program->bindAttrib(0, "position");
    program->bindAttrib(1, "texture");
    vao->bind();
}

static void onLoop() {
    program->setFloat("scRatio", 1080.0 / 1920);
    program->setInt("texture0", 0);
    program->use();
    texture->use(GL_TEXTURE0);

    vao->draw(GL_TRIANGLES);
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW initialization failed" << std::endl;
    }
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "My Title", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    onLoad();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        onLoop();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    glfwDestroyWindow(window);

    return 0;
}