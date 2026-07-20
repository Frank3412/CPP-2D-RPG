#include "TileMap.h"
#include "AssetManager.h"

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
   //AssetManager is now responsible for texture destruction.
}

bool TileMap::Initialize(SDL_Renderer* renderer,
    AssetManager& assetManager) {

    return LoadTextures(renderer, assetManager);
}

bool TileMap::LoadTextures(SDL_Renderer* renderer,
    AssetManager& assetManager) {
    tileTextures[TILE_GRASS] =
        assetManager.LoadTexture(renderer, "../assets/tiles/grass.bmp");
    tileTextures[TILE_STONE] =
        assetManager.LoadTexture(renderer, "../assets/tiles/stone.bmp");
    tileTextures[TILE_TREE] =
        assetManager.LoadTexture(renderer, "../assets/tiles/tree.bmp");
    tileTextures[TILE_WATER] =
       assetManager.LoadTexture(renderer, "../assets/tiles/water.bmp");

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

int TileMap::GetWorldWidth() const {
    return MAP_COLUMNS * TILE_SIZE;
}
int TileMap::GetWorldHeight() const {
    return MAP_ROWS * TILE_SIZE;
}
bool TileMap::IsSolidTile(int tile) const {
    return tile == TILE_STONE ||
        tile == TILE_TREE ||
            tile == TILE_HOUSE;
}
bool TileMap::CheckCollision(const SDL_FRect& playerRect) const {
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
