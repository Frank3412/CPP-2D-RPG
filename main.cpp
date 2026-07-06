#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

void ProcessInput(bool& running, SDL_Event& event);
void Update(SDL_FRect& playerRect, float playerSpeed, double deltaTime);
void Render(SDL_Renderer* renderer,
    SDL_Texture* grassTexture,
    SDL_Texture* stoneTexture,
    SDL_Texture* playerTexture,
    SDL_FRect& playerRect,
    float cameraX,
    float cameraY);
SDL_Texture* LoadTexture(
    SDL_Renderer* renderer,
    const char* filePath);

constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 800;
constexpr int TILE_SIZE = 32;
constexpr int MAP_ROWS = 60;
constexpr int MAP_COLUMNS = 100;
constexpr int WORLD_WIDTH = MAP_COLUMNS * TILE_SIZE;
constexpr int WORLD_HEIGHT = MAP_ROWS * TILE_SIZE;
constexpr int worldMap[MAP_ROWS][MAP_COLUMNS]={
    {1,0,2,2,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,2,0,0,0,0,1,1,1,1},

    {1,0,1,2,1,0,2,0,1,0,0,1,0,1,0,0,0,0,0,1,0,2,0,0,0,0,1,0,2,0,1},

    {1,1,1,0,1,1,1,2,1,0,1,0,0,0,1,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,1},

    {1,0,1,0,1,0,1,0,1,0,0,1,2,0,1,2,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1},
}; //The other unassigned cells will automatically be 0.
int main() {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL failed to initialize: %s", SDL_GetError());
        return 1;
    }


    //Create the window
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

    // Player Texture
   SDL_Texture* playerTexture =
       LoadTexture(renderer, "../assets/player.bmp");
    if (!playerTexture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // Grass Texture
    SDL_Texture* grassTexture =
       LoadTexture(renderer, "../assets/tiles/grass.bmp");
    if (!grassTexture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Stone Texture
    SDL_Texture* stoneTexture =
       LoadTexture(renderer, "../assets/tiles/stone.bmp");
    if (!stoneTexture) {
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

    float cameraX = 0.0f;
    float cameraY = 0.0f;
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
        Update(playerRect, PLAYER_SPEED, deltaTime);

        cameraX = playerRect.x - WINDOW_WIDTH/2.0f;
        cameraY = playerRect.y - WINDOW_HEIGHT/2.0f;

        if (cameraX < 0.0f) {
            cameraX = 0.0f;
        }
        if (cameraY < 0.0f) {
            cameraY = 0.0f;
        }
        if (cameraX > WORLD_WIDTH - WINDOW_WIDTH) {
            cameraX = WORLD_WIDTH - WINDOW_WIDTH;
        }
        if (cameraY > WORLD_HEIGHT - WINDOW_HEIGHT) {
            cameraY = WORLD_HEIGHT - WINDOW_HEIGHT;
        }

        Render(
            renderer,
            grassTexture,
            stoneTexture,
            playerTexture,
            playerRect,
            cameraX,
            cameraY);

        //****

        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(stoneTexture);
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

void Update(SDL_FRect& playerRect, float playerSpeed, double deltaTime) {

    const bool* keyboardStates = SDL_GetKeyboardState(nullptr);

    SDL_FRect nextPlayerRect = playerRect;

    if (keyboardStates[SDL_SCANCODE_UP]) {
        nextPlayerRect.y -= playerSpeed * static_cast<float>(deltaTime);
    }
    if (keyboardStates[SDL_SCANCODE_DOWN]) {
       nextPlayerRect.y += playerSpeed * static_cast<float>(deltaTime);
    }
    if (keyboardStates[SDL_SCANCODE_LEFT]) {
        nextPlayerRect.x -= playerSpeed * static_cast<float>(deltaTime);
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT]) {
        nextPlayerRect.x += playerSpeed * static_cast<float>(deltaTime);
    }

    if (nextPlayerRect.x < 0.0f) {
        nextPlayerRect.x = 0.0f;
    }
    if (nextPlayerRect.y < 0.0f) {
        nextPlayerRect.y = 0.0f;
    }
    if (nextPlayerRect.x > WORLD_WIDTH - nextPlayerRect.w) {
        nextPlayerRect.x = WORLD_WIDTH - nextPlayerRect.w;
    }
    if (nextPlayerRect.y > WORLD_HEIGHT - nextPlayerRect.h) {
        nextPlayerRect.y = WORLD_HEIGHT - nextPlayerRect.h;
    }


    int leftColumn =
        static_cast<int>(nextPlayerRect.x / TILE_SIZE);
    int rightColumn =
        static_cast<int>((nextPlayerRect.x + nextPlayerRect.w -1)/ TILE_SIZE);
    int topRow =
        static_cast<int>(nextPlayerRect.y / TILE_SIZE);
    int bottomRow =
        static_cast<int>((nextPlayerRect.y + nextPlayerRect.h -1)/ TILE_SIZE);

    int topLeftTile = worldMap[topRow][leftColumn];
    int topRightTile = worldMap[topRow][rightColumn];
    int bottomLeftTile = worldMap[bottomRow][leftColumn];
    int bottomRightTile = worldMap[bottomRow][rightColumn];

    if (topLeftTile == 2 ||
        topRightTile == 2 ||
        bottomLeftTile == 2 ||
        bottomRightTile == 2) {
        //Collision!
    }
    else {
        playerRect = nextPlayerRect;
    }

} //updated it by adding deltaTime as one of it is parameters.

void Render(SDL_Renderer* renderer,
    SDL_Texture* grassTexture,
    SDL_Texture* stoneTexture,
    SDL_Texture* playerTexture,
    SDL_FRect& playerRect,
    float cameraX,
    float cameraY) {

    SDL_SetRenderDrawColor(renderer, 40,60,100,255);

    SDL_RenderClear(renderer);

    int firstColumn = static_cast<int>(cameraX/TILE_SIZE);
    int firstRow = static_cast<int>(cameraY/TILE_SIZE);

    int visibleColumns = WINDOW_WIDTH/TILE_SIZE;
    int visibleRows = WINDOW_HEIGHT/TILE_SIZE;

    int lastColumn = firstColumn + visibleColumns + 1;
    if (lastColumn > MAP_COLUMNS) {
        lastColumn = MAP_COLUMNS;
    }
    int lastRow = firstRow + visibleRows + 1;
    if (lastRow > MAP_ROWS) {
        lastRow = MAP_ROWS;
    }

    for (int row = firstRow; row < lastRow; row++) {
        for (int column = firstColumn; column < lastColumn; column++) {

            SDL_FRect tileRect{
                static_cast<float>(column*TILE_SIZE)-cameraX,
                static_cast<float>(row*TILE_SIZE)-cameraY,
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE)
                    };
            if (worldMap[row][column] == 1) {
                SDL_RenderTexture(
                    renderer,
                    grassTexture,
                    nullptr,
                    &tileRect);
            }
            else if (worldMap[row][column] == 2) {
                SDL_RenderTexture(
                    renderer,
                    stoneTexture,
                    nullptr,
                    &tileRect);
            }
        }
    }

    SDL_FRect playerScreenRect = playerRect;
    playerScreenRect.x -= cameraX;
    playerScreenRect.y -= cameraY;
    SDL_RenderTexture(
        renderer,
        playerTexture,
        nullptr,
        &playerScreenRect); //is originally &playerRect

    SDL_RenderPresent(renderer);
} // will update

SDL_Texture* LoadTexture(
    SDL_Renderer* renderer,
    const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
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
} // done

