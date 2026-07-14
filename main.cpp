#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Player.h"
#include "TileMap.h"

//Forward declarations
void ProcessInput(bool& running, SDL_Event& event);
SDL_Texture* LoadTexture(SDL_Renderer* renderer,const char* filePath);
bool CheckCollision(SDL_FRect& playerRect);
bool IsSolidTile(int tile);

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
// Changed both const below
extern const int WORLD_WIDTH = MAP_COLUMNS * TILE_SIZE;
extern const int WORLD_HEIGHT = MAP_ROWS * TILE_SIZE;

constexpr int worldMap[MAP_ROWS][MAP_COLUMNS]={
    {1,0,2,2,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,2,0,0,0,0,1,1,1,1},

    {1,4,1,2,1,3,2,0,1,0,0,1,0,1,0,0,0,0,0,1,0,2,0,4,4,4,1,0,2,0,1},

    {1,1,1,3,1,1,1,2,1,3,1,3,0,3,1,2,2,2,2,1,0,0,0,3,0,4,4,4,4,0,1},

    {1,0,1,0,1,0,1,0,1,0,0,1,2,0,1,2,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1},
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
        SDL_CreateRenderer(window, "direct3d11");
        //SDL_CreateRenderer(window,nullptr);
    if (renderer) {
        SDL_SetRenderVSync(renderer, 1);// 1 turns VSync ON in SDL3
    }

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
    // Refactoring this line below.
    //SDL_Texture* tileTextures[TILE_COUNT]{};

    // This is going to be re-factor into TileMap.cpp/.h
    //LoadTileTextures(renderer,tileTextures);
    //
    TileMap tileMap;
    if (!tileMap.Initialize(renderer)) {
        return 1;
    }


    Player player;
    player.SetTexture(playerTexture);

    float cameraX = 0.0f;
    float cameraY = 0.0f;
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
        player.Update(static_cast<float>(deltaTime));

        //Camera tracking player position safely
        cameraX = player.GetRect().x - WINDOW_WIDTH/2.0f;
        cameraY = player.GetRect().y - WINDOW_HEIGHT/2.0f;


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

        // Render pass
        SDL_SetRenderDrawColor(renderer, 40, 60, 100, 255);
        SDL_RenderClear(renderer);

        // RenderMap is going to be moved to TileMap.cpp
        tileMap.Render(renderer, cameraX, cameraY);

        //CLEANUP: Player draws itself cleanly now
        player.Render(renderer, cameraX, cameraY);

        SDL_RenderPresent(renderer);
    }

    // Free resources
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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

bool IsSolidTile(int tile) {
    return tile == TILE_STONE ||
        tile == TILE_TREE ||
            tile == TILE_HOUSE;
}

bool CheckCollision(SDL_FRect& playerRect) {
    int leftColumn =
        static_cast<int>(playerRect.x/TILE_SIZE);
    int rightColumn =
        static_cast<int>((playerRect.x + playerRect.w - 1)/TILE_SIZE);
    int topRow =
        static_cast<int>(playerRect.y/TILE_SIZE);
    int bottomRow =
        static_cast<int>((playerRect.y + playerRect.h - 1)/TILE_SIZE);

    // CRITICAL FIX: Prevent accessing rows/columns that
    // don't exist in the array
    if (leftColumn < 0 ||
        rightColumn >= MAP_COLUMNS ||
        topRow < 0 ||
        bottomRow >= MAP_ROWS) {
        return true;
    }

    return
    IsSolidTile(worldMap[topRow][leftColumn]) ||
        IsSolidTile(worldMap[topRow][rightColumn]) ||
        IsSolidTile(worldMap[bottomRow][leftColumn]) ||
                IsSolidTile(worldMap[bottomRow][rightColumn]);
}

SDL_Texture* LoadTexture(
    SDL_Renderer* renderer,
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

