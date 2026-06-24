//
// Created by rydanee on 6/23/26.
//

#ifndef MCPP_TEXTURE_H
#define MCPP_TEXTURE_H
#include "glad/glad.h"


class Texture {
public:
    Texture();
    void loadFromFile(const char* fileName);
    void use(int tunit);
    void bind();
    void unbind();
    GLuint getTextureID();
private:
    GLuint _handle;
};


#endif //MCPP_TEXTURE_H
