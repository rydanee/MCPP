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

#include <imgui/imgui.h>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

static float dtime = 0.0f;
static long frameCount = 0;
static float _time = 0.0f;
bool menuFocus = false;
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
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        menuFocus = !menuFocus;
        if (menuFocus) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            cam->switchFirstMove();
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!menuFocus) {
        inputHandler->updateMouse((float)xpos, (float)ypos, 0);
    }

    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
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

void ApplyMinecraftTheme() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Disable modern styling smooth elements
    style.WindowRounding = 0.0f;  // Sharp pixel corners
    style.FrameRounding = 0.0f;   // Sharp buttons
    style.PopupRounding = 0.0f;
    style.WindowBorderSize = 2.0f;       // Thicker retro borders

    // Color Palette setup
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]             = ImVec4(0.15f, 0.15f, 0.15f, 0.85f); // Dark transparent inventory backing
    colors[ImGuiCol_Border]               = ImVec4(0.23f, 0.23f, 0.23f, 1.00f); // Light gray inventory border
    colors[ImGuiCol_Button]               = ImVec4(0.55f, 0.55f, 0.55f, 1.00f); // Classic gray button background
    colors[ImGuiCol_ButtonHovered]        = ImVec4(0.65f, 0.65f, 0.90f, 1.00f); // Bluish highlight outline on hover
    colors[ImGuiCol_ButtonActive]         = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_Text]                 = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); // Crisp white text
}


int main() {
    inputHandler = new InputHandler();

    if (!glfwInit()) {
        std::cout << "GLFW initialization failed" << std::endl;
    }
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "My Title", NULL, NULL);
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 450");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    onLoad();

    while (!glfwWindowShouldClose(window))
    {
        handleFps();
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        onLoop();

        io = ImGui::GetIO();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ApplyMinecraftTheme();

        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoTitleBar);
        glm::vec3 plrPos = cam->getPosition();
        ImGui::Text(("X: " + std::to_string(plrPos.x) + " Y:" + std::to_string(plrPos.y) + " Z:" + std::to_string(plrPos.z)).c_str());
        ImGui::Text(("FPS: " + std::to_string(1.f / dtime)).c_str());
        ImGui::End();

        ImGui::Begin("Tutorial", nullptr, ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("Press ESC to exit the game.");
        ImGui::Text("Press TAB to switch between game GUI and camera rotating.");
        ImGui::End();

        if (menuFocus) {
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        } else {
            io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
            io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    glfwDestroyWindow(window);

    return 0;
}