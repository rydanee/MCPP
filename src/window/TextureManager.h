//
// Created by rydanee on 6/24/26.
//

#ifndef MCPP_TEXTUREMANAGER_H
#define MCPP_TEXTUREMANAGER_H
#include <vector>

#include "Texture.h"


class TextureManager {
public:
    TextureManager(const TextureManager&) = delete;
    void operator=(const TextureManager&) = delete;

    static TextureManager& Get() {
        static TextureManager instance;
        return instance;
    }

    static void init();

    static Texture* getTexture(int id);

private:
    TextureManager() = default;
    static inline std::vector<Texture*> textures;
};


#endif //MCPP_TEXTUREMANAGER_H
