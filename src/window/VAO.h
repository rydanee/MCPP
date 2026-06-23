//
// Created by rydanee on 6/23/26.
//

#ifndef MCPP_VAO_H
#define MCPP_VAO_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class VAO {
public:
    VAO();
    void addVertexBufferObject(std::vector<float> data);
    void addVertexBufferObject(std::vector<glm::vec3> data);
    void addVertexBufferObject(std::vector<glm::vec2> data);
    void addIndices(std::vector<GLuint> indices);
    void bind();
    void draw(GLenum mode, int count);
    void draw(GLenum mode);
    ~VAO();
private:
    std::vector<GLuint> mBuffers;
    GLuint mIndices;
    uint mIndicesCount;
    GLuint _handle;
};


#endif //MCPP_VAO_H
