//
// Created by rydanee on 6/23/26.
//

#ifndef MCPP_PROGRAM_H
#define MCPP_PROGRAM_H
#include <string>
#include <glm/fwd.hpp>

#include "glad/glad.h"


class Program {
public:
    Program();
    void link();
    void dispose();
    void bindAttrib(int index, std::string name);
    void use();
    void setFloat(std::string name, float value);
    void setInt(std::string name, int value);
    void setMatrix4(std::string name, glm::mat4 value);
    int getLocation(std::string name);
private:
    GLuint _handle;
};


#endif //MCPP_PROGRAM_H
