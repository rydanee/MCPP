//
// Created by rydanee on 6/23/26.
//

#include "InputHandler.h"

#include <GLFW/glfw3.h>

void InputHandler::updateKeys(int keycode, bool pressed) {
    if (!pressedKeys.contains(keycode) && pressed) {
        pressedKeys.insert(keycode);
    } else if (pressedKeys.contains(keycode) && !pressed) {
        pressedKeys.erase(keycode);
    }
}

void InputHandler::sendFurtherInfo(float dtime, Camera *camera) {
    if (camera != nullptr) {
        camera->update(dtime, pressedKeys, x, y);
    }
}

void InputHandler::updateMouse(float x, float y, int action) {
    this->x = x;
    this->y = y;

    if (action == GLFW_MOUSE_BUTTON_1) {
        //whatever
    }
}
