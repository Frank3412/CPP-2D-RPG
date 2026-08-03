#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>

class AssetManager {
public:
    ~AssetManager();

    SDL_Texture* LoadTexture(SDL_Renderer* renderer,
        const char* filePath);

    void Shutdown();


private:
    std::unordered_map<std::string, SDL_Texture*> textures;
};