#include "TileMap.h"
#include "AssetManager.h"

#include <ostream>
#include <fstream>
#include <sstream>


// Forward declaration so the Tile class knows stuff
SDL_Texture* LoadTexture(SDL_Renderer* renderer,
    const char* filePath);


TileMap::TileMap() {

    for (int i = 0; i<TILE_COUNT; ++i) {
        tileTextures[i] = nullptr;
    }

    worldMap = {
        {1,0,2,2,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,2,0,0,0,0,1,1,1,1},

        {1,4,1,2,1,3,2,0,1,0,0,1,0,1,0,0,0,0,0,1,0,2,0,4,4,4,1,0,2,0,1},

        {1,1,1,3,1,1,1,2,1,3,1,3,0,3,1,2,2,2,2,1,0,0,0,3,0,4,4,4,4,0,1},

        {1,0,1,0,1,0,1,0,1,0,0,1,2,0,1,2,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1},
};
}

bool TileMap::LoadMap(const std::string& filename) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        SDL_Log("Failed to open map file: %s", filename.c_str());
        return false;
    }

    file >> mapColumns >> mapRows;

    SDL_Log("Map size: %d columns x %d rows",
        mapColumns,
        mapRows);

    worldMap.clear();

    for (int row = 0; row < mapRows; ++row) {
        std::vector<int> rowData;

        for (int column = 0; column < mapColumns; ++column) {
            int tile;
            if (!(file>>tile)) {
                SDL_Log("Failed to read tile data.");
                return false;
            }
            rowData.push_back(tile);
        }
        worldMap.push_back(rowData);
    }
    return true;
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

    int lastRow = std::min(firstRow + visibleRows + 1,
        mapRows); // static_cast<int>(worldMap.size())

    int lastColumn = std::min(firstColumn + visibleColumns + 1,
       mapColumns); // static_cast<int>(worldMap[row].size())

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
    return mapColumns * TILE_SIZE;
}
int TileMap::GetWorldHeight() const {
    return mapRows * TILE_SIZE;
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

    // Prevent accessing rows/columns that don't exist in the map
    if (leftColumn < 0 ||
        rightColumn >= mapColumns ||
        topRow < 0 ||
        bottomRow >= mapRows) {
        return true;
        }

    return
    IsSolidTile(worldMap[topRow][leftColumn]) ||
        IsSolidTile(worldMap[topRow][rightColumn]) ||
        IsSolidTile(worldMap[bottomRow][leftColumn]) ||
                IsSolidTile(worldMap[bottomRow][rightColumn]);
}
