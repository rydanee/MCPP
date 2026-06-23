//
// Created by rydanee on 6/23/26.
//

#include "Program.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string readShaderFile(const std::string& filePath) {
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(filePath);
        std::stringstream shaderStream;

        shaderStream << shaderFile.rdbuf();

        shaderFile.close();

        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: "
                  << filePath << " (Check path structure or file existence)" << std::endl;
    }

    return shaderCode;
}


Program::Program() {
    const std::string vertShader = readShaderFile("./assets/shaders/default.vert");
    const char* vertShaderCode = vertShader.c_str();
    const std::string fragShader = readShaderFile("./assets/shaders/default.frag");
    const char* fragShaderCode = fragShader.c_str();

    int success;
    char infoLog[512];

    int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertShaderCode, NULL);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragShaderCode, NULL);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    _handle = glCreateProgram();
    glAttachShader(_handle, vert);
    glAttachShader(_handle, frag);

    link();
}

void Program::link() {
    glLinkProgram(_handle);

    int success;
    char infoLog[512];
    glGetProgramiv(_handle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_handle, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void Program::dispose() {
    glDeleteProgram(_handle);
}

void Program::bindAttrib(int index, std::string name) {
    glBindAttribLocation(_handle, index, name.c_str());
}

void Program::use() {
    glUseProgram(_handle);
}

void Program::setFloat(std::string name, float value) {
    glUniform1f(getLocation(name), value);
}

void Program::setInt(std::string name, int value) {
    glUniform1i(getLocation(name), value);
}

void Program::setMatrix4(std::string name, glm::mat4 value) {
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, &value[0][0]);
}

int Program::getLocation(std::string name) {
    return glGetUniformLocation(_handle, name.c_str());
}
