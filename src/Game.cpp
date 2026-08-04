#include "Game.h"
#include "Constants.h"


Game::Game()
    : window(nullptr),
      renderer(nullptr),
camera(WINDOW_WIDTH,WINDOW_HEIGHT),
running(true),
previousCounter(0),
event()
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

    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if (!SDL_SetRenderVSync(renderer,1)) {
        SDL_Log("Failed to enable VSync: %s", SDL_GetError());
        Shutdown();
        return false;
    }


    // Player texture initialization
    SDL_Texture* texture =
        assetManager.LoadTexture(renderer,"../assets/player.bmp");
    if (!texture) {
        Shutdown();
        return false;
    }
    player.SetTexture(texture);


    if (!tileMap.Initialize(renderer, assetManager)) {
        Shutdown();
        return false;
    }

    if (!tileMap.LoadMap("../assets/maps/map01.txt")) {
        Shutdown();
        return false;
    }

    return true;
}

void Game::ProcessInput() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
}

void Game::Update(float deltaTime) {

    player.Update(deltaTime, tileMap);
    camera.Update(player.GetRect(), tileMap);

}

void Game::Render() {

    // Render pass
        SDL_SetRenderDrawColor(renderer, 40, 60, 100, 255);
        SDL_RenderClear(renderer);


        tileMap.Render(renderer, camera.GetX(), camera.GetY());

        player.Render(renderer, camera.GetX(), camera.GetY());

        SDL_RenderPresent(renderer);
}

void Game::Run() {

    previousCounter = SDL_GetPerformanceCounter();

    while (running) {

        Uint64 currentCounter = SDL_GetPerformanceCounter();
        // Make sure you divide by the frequency AFTER
        // subtracting the counters
        double deltaTime =
            static_cast<double>(currentCounter - previousCounter)/
                SDL_GetPerformanceFrequency();
        previousCounter = currentCounter;

        ProcessInput();

        // Update Function
        Update(static_cast<float>(deltaTime));

        // Render Function
        Render();

    }
}

void Game::Shutdown() {
    assetManager.Shutdown();

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}