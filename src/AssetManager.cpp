#include "AssetManager.h"

#include <SDL3_image/SDL_image.h>


SDL_Texture* AssetManager::LoadTexture(SDL_Renderer* renderer,
    const char* filePath) {

    std::string key(filePath);

    auto it = textures.find(key);

    if (it != textures.end()) {
        return it->second;
    }

    SDL_Surface* surface = IMG_Load(filePath);

    if (!surface) {
        SDL_Log("PNG load failed for: %s", filePath);
        SDL_Log("SDL_image version: %d", IMG_Version());
        return nullptr;
    }

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);

    if (!texture) {
        SDL_Log("Failed to create texture: %s",
            SDL_GetError());
        return nullptr;
    }

    textures.emplace(key, texture);

    return texture;
}

AssetManager::~AssetManager() {
    Shutdown();
}

void AssetManager::Shutdown() {
    for (auto& pair : textures)
    {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
}