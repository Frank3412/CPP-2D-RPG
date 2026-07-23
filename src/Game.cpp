#include "Game.h"
#include "Constants.h"


Game::Game()
    : window(nullptr),
      renderer(nullptr),
camera(WINDOW_WIDTH,WINDOW_HEIGHT)
{
}

bool Game::Initialize() {

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL failed to initialize: %s", SDL_GetError());
        return false;
    }

    //Create the window
    window = SDL_CreateWindow(
        "Monster Quest",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    //Create the renderer
    renderer = SDL_CreateRenderer(window,nullptr);

    if (renderer) {
        SDL_SetRenderVSync(renderer, 1);// 1 turns VSync ON in SDL3
    }

    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

SDL_Window* Game::GetWindow() const {
    return window;
}
SDL_Renderer* Game::GetRenderer() const {
    return renderer;
}

AssetManager& Game::GetAssetManager() {
    return assetManager;
}

TileMap& Game::GetTileMap() {
    return tileMap;
}

Player& Game::GetPlayer() {
    return player;
}

Camera& Game::GetCamera() {
    return camera;
 }


void Game::Run() {

}

void Game::Shutdown() {

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}