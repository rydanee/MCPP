//
// Created by rydanee on 6/23/26.
//

#include "Camera.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <ostream>

Camera::Camera(glm::vec3 position) {
    this->position = position;
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::mat4 Camera::getProjectionMatrix() {
    float fovRadians = glm::radians(fov);
    float aspectRatio = 1920.0 / 1080;

    return glm::perspective(fovRadians, aspectRatio, 0.1f, 100.0f);
}

void Camera::update(float dtime, const std::set<int>& pressedKeys, float x, float y) {
    if (pressedKeys.contains(GLFW_KEY_W)) {
        position += front * speed * dtime;
    }
    if (pressedKeys.contains(GLFW_KEY_S)) {
        position -= front * speed * dtime;
    }
    if (pressedKeys.contains(GLFW_KEY_A)) {
        position -= right * speed * dtime;
    }
    if (pressedKeys.contains(GLFW_KEY_D)) {
        position += right * speed * dtime;
    }
    if (pressedKeys.contains(GLFW_KEY_SPACE)) {
        position += up * speed * dtime;
    }
    if (pressedKeys.contains(GLFW_KEY_LEFT_SHIFT)) {
        position -= up * speed * dtime;
    }

    if (firstMove) {
        lastPos = glm::vec2(x, y);
        firstMove = false;
    } else {
        float dx = x - lastPos.x;
        float dy = y - lastPos.y;
        lastPos = glm::vec2(x, y);

        yaw += dx * sensitivity * dtime;
        pitch -= dy * sensitivity * dtime;
    }

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    updateVectors();
}

void Camera::updateVectors() {
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}
