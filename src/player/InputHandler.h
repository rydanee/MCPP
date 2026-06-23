//
// Created by rydanee on 6/23/26.
//

#ifndef MCPP_INPUTHANDLER_H
#define MCPP_INPUTHANDLER_H
#include <set>

#include "Camera.h"


class InputHandler {
public:
    float dx;
    float dy;
    std::set<int> pressedKeys = std::set<int>();

    void update(int keycode, bool pressed);
    void sendFurtherInfo(float dtime, Camera* camera);
};


#endif //MCPP_INPUTHANDLER_H
