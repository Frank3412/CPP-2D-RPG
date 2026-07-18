#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class AssetManager {
public:
    SDL_Texture* LoadTexture(SDL_Renderer* renderer,
        const char* filePath);
};