#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "AssetManager.h"
#include "Player.h"
#include "TileMap.h"
#include "Camera.h"
#include "Game.h"

//Forward declarations
void ProcessInput(bool& running, SDL_Event& event);

// Constants
constexpr int TILE_EMPTY = 0;
constexpr int TILE_GRASS = 1;
constexpr int TILE_STONE = 2;
constexpr int TILE_TREE = 3;
constexpr int TILE_WATER = 4;
constexpr int TILE_HOUSE = 5;

constexpr int TILE_COUNT = 6;
constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 800;
constexpr int TILE_SIZE = 32;

// FIX: Keeping your original world map data size perfectly
// matching array indices
constexpr int MAP_ROWS = 60;
constexpr int MAP_COLUMNS = 100;

constexpr int worldMap[MAP_ROWS][MAP_COLUMNS]={
    {1,0,2,2,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,2,0,0,0,0,1,1,1,1},

    {1,4,1,2,1,3,2,0,1,0,0,1,0,1,0,0,0,0,0,1,0,2,0,4,4,4,1,0,2,0,1},

    {1,1,1,3,1,1,1,2,1,3,1,3,0,3,1,2,2,2,2,1,0,0,0,3,0,4,4,4,4,0,1},

    {1,0,1,0,1,0,1,0,1,0,0,1,2,0,1,2,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1},
}; //The other unassigned cells will automatically be 0.

int main() {
    //Added this chuck of code into Game::Initialize.

    Game game;
    if (!game.Initialize()) {
        return 1;
    }


    // Player Texture
   //Moved  AssetManager assetManager; to Game.
    SDL_Texture* playerTexture =
        game.GetAssetManager().LoadTexture(game.GetRenderer(), "../assets/player.bmp");

    // This if statement needs to be removed/moved.
    if (!playerTexture) {
        game.Shutdown();
        return 1;
    }


    //TileMap tileMap;
    if (!game.GetTileMap().Initialize(game.GetRenderer(), game.GetAssetManager())) {
        return 1;
    }


    Player player;
    player.SetTexture(playerTexture);

    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
    bool running = true;

    SDL_Event event;

    // The Game Loop
    Uint64 previousCounter = SDL_GetPerformanceCounter();

    while (running) {

        Uint64 currentCounter = SDL_GetPerformanceCounter();
        // Make sure you divide by the frequency AFTER
        // subtracting the counters
        double deltaTime =
            static_cast<double>(currentCounter - previousCounter)/
                SDL_GetPerformanceFrequency();
        previousCounter = currentCounter;

        ProcessInput(running, event);

        // CLEANUP: Object-Oriented Update
        player.Update(static_cast<float>(deltaTime), game.GetTileMap());


        camera.Update(player.GetRect(), game.GetTileMap());
        //Took this chunk of code here and attached it to Camera.cpp
        // in Camera::Update function.

        // Render pass
        SDL_SetRenderDrawColor(game.GetRenderer(), 40, 60, 100, 255);
        SDL_RenderClear(game.GetRenderer());

        // RenderMap is going to be moved to TileMap.cpp
        game.GetTileMap().Render(game.GetRenderer(), camera.GetX(), camera.GetY());

        //CLEANUP: Player draws itself cleanly now
        player.Render(game.GetRenderer(), camera.GetX(), camera.GetY());

        SDL_RenderPresent(game.GetRenderer());
    }

    // Free resources
    game.Shutdown();
    return 0;
}
// End of main()

void ProcessInput(bool& running, SDL_Event& event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
} // done