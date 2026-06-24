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
#include "window/TextureManager.h"
#include "world/ChunkGenerator.h"

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

Program *program;
Texture *texture;
ChunkGenerator *chunkGenerator;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

static void onLoad() {
    TextureManager::init();

    chunkGenerator = new ChunkGenerator(1234);

    cam = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::identity<glm::mat4>();
    projection = cam->getProjectionMatrix();

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

    program = new Program();
    texture = TextureManager::getTexture(1);

    program->bindAttrib(0, "position");
    program->bindAttrib(1, "texture");

    chunkGenerator->GenerateChunks();
}

static void handleFps() {
    float current = (float) glfwGetTime();
    dtime = current - _time;
    _time = current;
    std::cout << 1.f / dtime << std::endl;
}

static void onLoop() {
    inputHandler->sendFurtherInfo(dtime, cam);

    view = cam->getViewMatrix();

    program->setFloat("scRatio", 1080.0 / 1920);
    program->setInt("texture0", 0);

    program->use();
    program->setMatrix4("model", model);
    program->setMatrix4("view", view);
    program->setMatrix4("projection", projection);
    texture->use(GL_TEXTURE0);

    chunkGenerator->RenderChunks();
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

    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    onLoad();

    while (!glfwWindowShouldClose(window))
    {
        handleFps();
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        onLoop();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    glfwDestroyWindow(window);

    return 0;
}