#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window/VAO.h"
#include "window/Program.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

VAO *vao;
Program *program;

static void onLoad() {
    vao = new VAO();
    program = new Program();

    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, -0.5f,  0.0f), // bl
        glm::vec3( 0.5f, -0.5f,  0.0f), // br
        glm::vec3( 0.0f,  0.5f,  0.0f)  // tc
    };

    vao->addVertexBufferObject(vertices);

    program->bindAttrib(0, "position");
    program->use();
}

static void onLoop() {
    vao->draw(GL_TRIANGLES, 3);
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW initialization failed" << std::endl;
    }
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
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