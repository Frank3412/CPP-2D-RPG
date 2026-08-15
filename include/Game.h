#pragma once

#include "AssetManager.h"
#include "TileMap.h"
#include "Player.h"
#include "Camera.h"
#include "Sign.h"
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
    Sign sign;
    Camera camera;
    bool running;
    bool signInRange;
    bool interactPressed;
    bool interactKeyDown;
    Uint64 previousCounter;
    SDL_Event event;
    void ProcessInput();
    void Update(float deltaTime);
    void Render();
};
