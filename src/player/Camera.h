//
// Created by rydanee on 6/23/26.
//

#ifndef MCPP_CAMERA_H
#define MCPP_CAMERA_H
#include <set>
#include <glm/glm.hpp>

#include "window/Program.h"
#include "window/VAO.h"


class Camera {
    glm::vec3 position;
public:
    Program *wireframeProgram;
    VAO *wireframe;

    Camera(glm::vec3 position);

    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    float speed = 40;
    float sensitivity = 50.0f;
    glm::vec2 lastPos;

    glm::vec3 getPosition();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    void update(float dtime, const std::set<int>& pressedKeys, float x, float y);
    void switchFirstMove();

    void initWireFrame();
private:
    void updateVectors();
    bool firstMove = true;
    float yaw = 0;
    float pitch = -90;
    float fov = 60.0f;

};


#endif //MCPP_CAMERA_H
