//
// Created by rydanee on 6/23/26.
//

#include "Texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <stb_image.h>
#include <string>

GLuint loadTexture(const std::string& filePath) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "ERROR::TEXTURE::FAILED_TO_LOAD_PATH: " << filePath << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}

Texture::Texture() {
    stbi__vertically_flip_on_load_set = 1;
    glGenTextures(0, &_handle);
}

void Texture::loadFromFile(const char *filename) {
    use(GL_TEXTURE0);

    _handle = loadTexture(filename);
}

void Texture::use(int tunit) {
    glActiveTexture(tunit);
    glBindTexture(GL_TEXTURE_2D, _handle);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getTextureID() {
    return _handle;
}
