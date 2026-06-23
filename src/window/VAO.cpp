//
// Created by rydanee on 6/23/26.
//

#include "VAO.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

VAO::VAO() {
    glGenVertexArrays(1, &_handle);
    bind();
}

void VAO::addVertexBufferObject(std::vector<float> data) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mbuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    mbuffers.push_back(vbo);
}

void VAO::addVertexBufferObject(std::vector<glm::vec3> data) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mbuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    mbuffers.push_back(vbo);
}

void VAO::addVertexBufferObject(std::vector<glm::vec2> data) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec2), data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mbuffers.size(), 2, GL_FLOAT, GL_FALSE, 0, 0);
    mbuffers.push_back(vbo);
}

void VAO::bind() {
    glBindVertexArray(_handle);
}

void VAO::draw(GLenum mode, int count) {
    bind();
    for (unsigned int i = 0; i < mbuffers.size(); i++) {
        glEnableVertexArrayAttrib(_handle, i);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _handle);
    glDrawArrays(mode, 0, count);
    for (unsigned int i = 0; i < mbuffers.size(); i++) {
        glDisableVertexArrayAttrib(_handle, i);
    }
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &_handle);
}
