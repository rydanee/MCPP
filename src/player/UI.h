//
// Created by rydanee on 6/24/26.
//

#ifndef MCPP_UI_H
#define MCPP_UI_H


class UI {
public:
    UI(const UI&) = delete;
    void operator=(const UI&) = delete;

    static UI& Get() {
        static UI instance;
        return instance;
    }

private:
    UI() = default;
};


#endif //MCPP_UI_H
