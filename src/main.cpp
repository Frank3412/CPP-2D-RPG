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