#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

void ProcessInput(bool& running, SDL_Event& event);
void Update(SDL_FRect& playerRect, float playerSpeed, double deltaTime,
    int windowWidth, int windowHeight);
void Render(SDL_Renderer* renderer,
    SDL_Texture* playerTexture,
    SDL_FRect& playerRect);
int main() {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL failed to initialize: %s", SDL_GetError());
        return 1;
    }

    //Create the window
    constexpr int WINDOW_WIDTH = 1000;
    constexpr int WINDOW_HEIGHT = 800;
    SDL_Window *window = SDL_CreateWindow(
        "Monster Quest",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //Create the renderer
    SDL_Renderer* renderer =
        SDL_CreateRenderer(window,nullptr);

    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load the image into a surface
    SDL_Surface* surface = IMG_Load("../assets/player.bmp");

    if (!surface) {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Convert the surface into a texture
    SDL_Texture* playerTexture =
        SDL_CreateTextureFromSurface(renderer,surface);

    // The surface is no longer needed
    SDL_DestroySurface(surface);

    if (!playerTexture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Position and size of the sprite
    constexpr float PLAYER_SIZE = 64.0F;
    SDL_FRect playerRect{
    100.0f,
    100.0f,
    PLAYER_SIZE,
    PLAYER_SIZE};

    constexpr float PLAYER_SPEED = 200.0f;
    bool running = true;

    SDL_Event event;

    // The Game Loop
    Uint64 previousCounter = SDL_GetPerformanceCounter();
    while (running) {

        Uint64 currentCounter = SDL_GetPerformanceCounter();
        double deltaTime =
            static_cast<double>(currentCounter - previousCounter)/
                SDL_GetPerformanceFrequency();
        previousCounter = currentCounter;

        ProcessInput(running, event);
        Update(playerRect, PLAYER_SPEED, deltaTime,
            WINDOW_WIDTH, WINDOW_HEIGHT);

        Render(renderer,
            playerTexture,
            playerRect);

        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void ProcessInput(bool& running, SDL_Event& event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
} // will update

void Update(SDL_FRect& playerRect, float playerSpeed, double deltaTime,
    int windowWidth, int windowHeight) {

    const bool* keyboardStates = SDL_GetKeyboardState(nullptr);

    if (keyboardStates[SDL_SCANCODE_UP]) {
        playerRect.y -= playerSpeed * static_cast<float>(deltaTime);
    }
    if (keyboardStates[SDL_SCANCODE_DOWN]) {
        playerRect.y += playerSpeed * static_cast<float>(deltaTime);
    }
    if (keyboardStates[SDL_SCANCODE_LEFT]) {
        playerRect.x -= playerSpeed * static_cast<float>(deltaTime);
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT]) {
        playerRect.x += playerSpeed * static_cast<float>(deltaTime);
    }

    if (playerRect.x < 0.0f) {
        playerRect.x = 0.0f;
    }
    if (playerRect.y < 0.0f) {
        playerRect.y = 0.0f;
    }
    if (playerRect.x > windowWidth - playerRect.w) {
        playerRect.x = windowWidth - playerRect.w;
    }
    if (playerRect.y > windowHeight - playerRect.h) {
        playerRect.y = windowHeight - playerRect.h;
    }
} //updated it by adding deltaTime as one of it is parameters.

void Render(SDL_Renderer* renderer,
    SDL_Texture* playerTexture,
    SDL_FRect& playerRect) {

    SDL_SetRenderDrawColor(renderer, 40,60,100,255);

    SDL_RenderClear(renderer);

    SDL_RenderTexture(
        renderer,
        playerTexture,
        nullptr,
        &playerRect);

    SDL_RenderPresent(renderer);
} // will update