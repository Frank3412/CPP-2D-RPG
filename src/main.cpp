#include <iostream>
#include <SDL3/SDL.h>

#include "Game.h"


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


int main() {


    // Game Initialization
    Game game;
    if (!game.Initialize()) {
        return 1;
    }
    // Game Loop
    game.Run();

    // Free resources
    game.Shutdown();
    return 0;
}
// End of main()