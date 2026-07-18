#include "AssetManager.h"


SDL_Texture* AssetManager::LoadTexture(SDL_Renderer* renderer,
    const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    //SDL_Surface* surface = SDL_LoadBMP(filePath);
    if (!surface) {
        SDL_Log("Failed to load image %s: %s",
            filePath,
            SDL_GetError());
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
    return texture;
}