#pragma once

#include "AssetManager.h"
#include "TileMap.h"
#include <SDL3/SDL.h>


class Game {
public:
    Game();
    bool Initialize();
    void Run();
    void Shutdown();
    SDL_Window* GetWindow() const;
    SDL_Renderer* GetRenderer() const;
    AssetManager& GetAssetManager();
    TileMap& GetTileMap();


private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    AssetManager assetManager;
    TileMap tileMap;
};
