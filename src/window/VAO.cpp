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
    glVertexAttribPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(mBuffers.size());
    mBuffers.push_back(vbo);
}

void VAO::addVertexBufferObject(std::vector<glm::vec3> data) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(mBuffers.size());
    mBuffers.push_back(vbo);
}

void VAO::addVertexBufferObject(std::vector<glm::vec2> data) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec2), data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mBuffers.size(), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(mBuffers.size());
    mBuffers.push_back(vbo);
}

void VAO::bind() {
    glBindVertexArray(_handle);
}

void VAO::draw(GLenum mode, int count) {
    bind();
    glBindBuffer(GL_ARRAY_BUFFER, _handle);
    glDrawArrays(mode, 0, count);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &_handle);
}

void VAO::addIndices(std::vector<GLuint> indices) {
    glGenBuffers(1, &mIndices);
    mIndicesCount = indices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);
}

void VAO::draw(GLenum mode) {
    bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
    glDrawElements(mode, mIndicesCount, GL_UNSIGNED_INT, 0);
}
