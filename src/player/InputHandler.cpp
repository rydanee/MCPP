//
// Created by rydanee on 6/23/26.
//

#include "InputHandler.h"

void InputHandler::update(int keycode, bool pressed) {
    if (!pressedKeys.contains(keycode) && pressed) {
        pressedKeys.insert(keycode);
    } else if (pressedKeys.contains(keycode) && !pressed) {
        pressedKeys.erase(keycode);
    }
}

void InputHandler::sendFurtherInfo(float dtime, Camera *camera) {
    if (camera != nullptr) {
        camera->update(dtime, pressedKeys, dx, dy);
    }
}
