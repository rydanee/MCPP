#include <iostream>
#include <set>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

#include "player/Camera.h"
#include "player/InputHandler.h"
#include "window/VAO.h"
#include "window/Program.h"
#include "window/Texture.h"

static float dtime = 0.0f;
static long frameCount = 0;
static float _time = 0.0f;
std::set<int> pressedKeys;
InputHandler *inputHandler;
Camera *cam;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        inputHandler->updateKeys(key, true);
    } else if (action == GLFW_RELEASE) {
        inputHandler->updateKeys(key, false);
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    inputHandler->updateMouse((float)xpos, (float)ypos, 0);
}

VAO *vao;
Program *program;
Texture *texture;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

static void onLoad() {
    cam = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::identity<glm::mat4>();
    projection = cam->getProjectionMatrix();

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

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

static void handleFps() {
    float current = (float) glfwGetTime();
    dtime = current - _time;
    _time = current;
    //std::cout << 1.f / dtime << std::endl;
}

static void onLoop() {
    inputHandler->sendFurtherInfo(dtime, cam);

    view = cam->getViewMatrix();

    program->setFloat("scRatio", 1080.0 / 1920);
    program->setInt("texture0", 0);
    program->setMatrix4("model", model);
    program->setMatrix4("view", view);
    program->setMatrix4("projection", projection);

    program->use();
    texture->use(GL_TEXTURE0);

    vao->draw(GL_TRIANGLES);
}

int main() {
    inputHandler = new InputHandler();

    if (!glfwInit()) {
        std::cout << "GLFW initialization failed" << std::endl;
    }
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "My Title", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    onLoad();

    while (!glfwWindowShouldClose(window))
    {
        handleFps();
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        onLoop();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    glfwDestroyWindow(window);

    return 0;
}