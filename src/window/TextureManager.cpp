//
// Created by rydanee on 6/24/26.
//

#include "TextureManager.h"

#include <filesystem>
#include <string>

void TextureManager::init() {
    std::string path = "./assets/textures/";
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        Texture *tex = new Texture();
        tex->loadFromFile(entry.path().c_str());
        textures.push_back(tex);
    }
}

Texture * TextureManager::getTexture(int id) {
    return textures[id];
}
