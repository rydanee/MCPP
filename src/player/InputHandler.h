//
// Created by rydanee on 6/23/26.
//

#ifndef MCPP_INPUTHANDLER_H
#define MCPP_INPUTHANDLER_H
#include <set>

#include "Camera.h"


class InputHandler {
public:
    float x;
    float y;
    std::set<int> pressedKeys = std::set<int>();

    void updateKeys(int keycode, bool pressed);
    void updateMouse(float x, float y, int action);
    void sendFurtherInfo(float dtime, Camera* camera);
};


#endif //MCPP_INPUTHANDLER_H
