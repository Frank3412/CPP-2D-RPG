#include "TileMap.h"

#include <iostream>
#include <ostream>

// Forward declaration so the Tile class knows stuff
SDL_Texture* LoadTexture(SDL_Renderer* renderer,
    const char* filePath);


TileMap::TileMap() {

    for (int i = 0; i < TILE_COUNT; i++) {
        tileTextures[i] = nullptr;
    }
}

TileMap::~TileMap() {
    for (int i = 0; i < TILE_COUNT; i++) {
        SDL_DestroyTexture(tileTextures[i]);
    }
}

bool TileMap::Initialize(SDL_Renderer* renderer) {
    return LoadTextures(renderer);
}

bool TileMap::LoadTextures(SDL_Renderer* renderer) {
    tileTextures[TILE_GRASS] =
        LoadTexture(renderer, "../assets/tiles/grass.bmp");
    tileTextures[TILE_STONE] =
        LoadTexture(renderer, "../assets/tiles/stone.bmp");
    tileTextures[TILE_TREE] =
        LoadTexture(renderer, "../assets/tiles/tree.bmp");
    tileTextures[TILE_WATER] =
        LoadTexture(renderer, "../assets/tiles/water.bmp");

    return tileTextures[TILE_GRASS] &&
        tileTextures[TILE_STONE] &&
            tileTextures[TILE_TREE] &&
                tileTextures[TILE_WATER];
}

void TileMap::Render(SDL_Renderer* renderer,
    float cameraX,
    float cameraY) const {

    int firstColumn = static_cast<int>(cameraX/TILE_SIZE);
    int firstRow = static_cast<int>(cameraY/TILE_SIZE);
    int visibleColumns = WINDOW_WIDTH/TILE_SIZE;
    int visibleRows = WINDOW_HEIGHT/TILE_SIZE;

    int lastColumn = std::min(firstColumn + visibleColumns + 1,
       MAP_COLUMNS);
    int lastRow = std::min(firstRow + visibleRows + 1,
        MAP_ROWS);

    for (int row = firstRow; row < lastRow; row++) {
        for (int column = firstColumn; column < lastColumn; column++) {
            SDL_FRect tileRect{
                static_cast<float>(column*TILE_SIZE)-cameraX,
                static_cast<float>(row*TILE_SIZE)-cameraY,
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE)
                    };
            const int tile = worldMap[row][column];
            if (tileTextures[tile] != nullptr) {
                SDL_RenderTexture(
                    renderer,
                    tileTextures[tile],
                    nullptr,
                    &tileRect);
            }

        }
    }
    }
