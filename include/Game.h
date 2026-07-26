#pragma once

#include "AssetManager.h"
#include "TileMap.h"
#include "Player.h"
#include "Camera.h"
#include <SDL3/SDL.h>


class Game {
public:
    Game();
    bool Initialize();
    void Run();
    void Shutdown();



private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    AssetManager assetManager;
    TileMap tileMap;
    Player player;
    Camera camera;
    bool running;
    Uint64 previousCounter;
    SDL_Event event;
    void ProcessInput();
    void Update(float deltaTime);
    void Render();
};
