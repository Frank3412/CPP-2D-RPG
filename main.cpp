#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

void ProcessInput(bool& running, SDL_Event& event);
void Update(SDL_FRect& playerRect, float playerSpeed);
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
    SDL_Window *window = SDL_CreateWindow(
        "Monster Quest",
        1000,
        800,
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
    SDL_FRect playerRect{
    100.0f,
    100.0f,
    64.0f,
    64.0f};

    float playerSpeed = 10.0f;
    bool running = true;

    SDL_Event event;

    // The Game Loop
    while (running) {

        ProcessInput(running, event);
        Update(playerRect, playerSpeed);

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
} //done

void Update(SDL_FRect& playerRect, float playerSpeed) {

    const bool* keyboardStates = SDL_GetKeyboardState(nullptr);

    if (keyboardStates[SDL_SCANCODE_UP]) {
        playerRect.y -= playerSpeed;
    }
    if (keyboardStates[SDL_SCANCODE_DOWN]) {
        playerRect.y += playerSpeed;
    }
    if (keyboardStates[SDL_SCANCODE_LEFT]) {
        playerRect.x -= playerSpeed;
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT]) {
        playerRect.x += playerSpeed;
    }
} //done

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
} //done