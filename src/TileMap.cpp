#include "TileMap.h"
#include "AssetManager.h"
#include "Constants.h"

#include <fstream>
#include <random>



TileMap::TileMap() {

    for (int i = 0; i<TILE_COUNT; ++i) {
        tileTextures[i] = nullptr;
    }
}

bool TileMap::LoadMap(const std::string& filename) {

    std::mt19937 rng(12345);

    std::ifstream file(filename);

    if (!file.is_open()) {
        SDL_Log("Failed to open map file: %s", filename.c_str());
        return false;
    }

    if (!(file >> mapColumns >> mapRows)) {
        SDL_Log("Failed to read map dimensions.");
        return false;
    }

    if (mapColumns <= 0 || mapRows <= 0) {
        SDL_Log("Invalid map dimensions: %d x %d",
            mapColumns, mapRows);
        return false;
    }

    SDL_Log("Map size: %d columns x %d rows",
        mapColumns,
        mapRows);


    // Clear the existing map before loading the new map.
    worldMap.clear();
    tileVariants.clear();

    for (int row = 0; row < mapRows; ++row) {
        std::vector<int> rowData;
        std::vector<int> variantRow;

        for (int column = 0; column < mapColumns; ++column) {
            int tile;
            if (!(file>>tile)) {
                SDL_Log("Failed to read tile data.");
                return false;
            }
            if (tile < 0 || tile >= TILE_COUNT) {
                SDL_Log("Invalid tile ID: %d", tile);
                return false;
            }
            rowData.push_back(tile);

            int variant = 0;

            if (tile == TILE_GRASS) {
                std::uniform_int_distribution<int> distribution(
                    0,
                    GRASS_VARIANTS-1);
                variant = distribution(rng);
            }
            else if (tile == TILE_STONE) {
                std::uniform_int_distribution<int> distribution(
                    0,
                    STONE_VARIANTS-1);
                variant = distribution(rng);
            }
            else if (tile == TILE_DIRT) {
                std::uniform_int_distribution<int> distribution(
                    0,
                    DIRT_VARIANTS-1);
                variant = distribution(rng);
            }
            variantRow.push_back(variant);
        }
        worldMap.push_back(rowData);
        tileVariants.push_back(variantRow);
    }
    int extraTile;
    if (file>>extraTile) {
        SDL_Log("Map file contains extra tile data.");
        return false;
    }
    return true;
}

bool TileMap::Initialize(SDL_Renderer* renderer,
    AssetManager& assetManager) {

    return LoadTextures(renderer, assetManager);
}

bool TileMap::LoadTextures(SDL_Renderer* renderer,
    AssetManager& assetManager) {

    grassTextures[0] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/grass_01.bmp");
    grassTextures[1] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/grass_02.bmp");
    grassTextures[2] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/grass_03.bmp");
    grassTextures[3] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/grass_04.bmp");
   stoneTextures[0] =
       assetManager.LoadTexture(
          renderer,
          "../assets/tiles/stone_01.bmp");
    stoneTextures[1] =
       assetManager.LoadTexture(
          renderer,
          "../assets/tiles/stone_02.bmp");
    stoneTextures[2] =
       assetManager.LoadTexture(
          renderer,
          "../assets/tiles/stone_03.bmp");
    tileTextures[TILE_TREE] =
        assetManager.LoadTexture(renderer, "../assets/tiles/tree.bmp");
    tileTextures[TILE_WATER] =
       assetManager.LoadTexture(renderer, "../assets/tiles/water.bmp");
    dirtTextures[0] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/dirt_01.bmp");
    dirtTextures[1] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/dirt_02.bmp");
    dirtTextures[2] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/dirt_03.bmp");

    grassDirtTransitions[0] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/transitions/grass_dirt/grass_dirt_top.bmp");
    grassDirtTransitions[1] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/transitions/grass_dirt/grass_dirt_bottom.bmp");
    grassDirtTransitions[2] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/transitions/grass_dirt/grass_dirt_left.bmp");
    grassDirtTransitions[3] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/transitions/grass_dirt/grass_dirt_right.bmp");
    grassDirtTransitions[4] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/transitions/grass_dirt/grass_dirt_top_left.bmp");
    grassDirtTransitions[5] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/transitions/grass_dirt/grass_dirt_top_right.bmp");
    grassDirtTransitions[6] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/transitions/grass_dirt/grass_dirt_bottom_left.bmp");
    grassDirtTransitions[7] =
        assetManager.LoadTexture(
            renderer,
            "../assets/tiles/transitions/grass_dirt/grass_dirt_bottom_right.bmp");

    return grassTextures[0] &&
        grassTextures[1] &&
            grassTextures[2] &&
                grassTextures[3] &&
        stoneTextures[0] &&
            stoneTextures[1] &&
                stoneTextures[2] &&
            tileTextures[TILE_TREE] &&
                tileTextures[TILE_WATER]&&
                    stoneTextures[0] &&
                        stoneTextures[1] &&
                            stoneTextures[2] &&
                                grassDirtTransitions[0] &&
                                    grassDirtTransitions[1] &&
                                        grassDirtTransitions[2] &&
                                            grassDirtTransitions[3] &&
                                                grassDirtTransitions[4] &&
                                                    grassDirtTransitions[5] &&
                                                        grassDirtTransitions[6] &&
                                                            grassDirtTransitions[7] &&
                                                                dirtTextures[0] &&
                                                                    dirtTextures[1] &&
                                                                        dirtTextures[2];
}

void TileMap::Render(SDL_Renderer* renderer,
    float cameraX,
    float cameraY) const {
    int firstColumn = static_cast<int>(cameraX/TILE_SIZE);
    int firstRow = static_cast<int>(cameraY/TILE_SIZE);
    int visibleColumns = (WINDOW_WIDTH/TILE_SIZE) + 1;
    int visibleRows = (WINDOW_HEIGHT/TILE_SIZE) + 1;

    int lastRow = std::min(firstRow + visibleRows + 1,
        mapRows);

    int lastColumn = std::min(firstColumn + visibleColumns + 1,
       mapColumns);

    for (int row = firstRow; row < lastRow; row++) {
        for (int column = firstColumn; column < lastColumn; column++) {
            SDL_FRect tileRect{
                static_cast<float>(column*TILE_SIZE)-cameraX,
                static_cast<float>(row*TILE_SIZE)-cameraY,
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE)
                    };
            const int tile = worldMap[row][column];
            const int variant = tileVariants[row][column];

            if (tile == TILE_GRASS) {
                if (grassTextures[variant] != nullptr) {
                    SDL_RenderTexture(
                        renderer,
                        grassTextures[variant],
                        nullptr,
                        &tileRect);
                }
            }
            else if (tile == TILE_STONE) {
                if (stoneTextures[variant] != nullptr) {
                    SDL_RenderTexture(
                        renderer,
                        stoneTextures[variant],
                        nullptr,
                        &tileRect);
                }
            }
            else if (tile == TILE_DIRT) {
                if (dirtTextures[variant] != nullptr) {
                    SDL_RenderTexture(
                        renderer,
                        dirtTextures[variant],
                        nullptr,
                        &tileRect);
                }
            }
                else if (tileTextures[tile] != nullptr) {
                    SDL_RenderTexture(
                        renderer,
                        tileTextures[tile],
                        nullptr,
                        &tileRect);
                }

            if (tile == TILE_DIRT) {

                const bool grassAbove = IsGrassTile(row-1,column);
                const bool grassBelow = IsGrassTile(row+1,column);
                const bool grassLeft = IsGrassTile(row,column-1);
                const bool grassRight = IsGrassTile(row,column+1);

                // Corners first
                if (grassAbove && grassLeft &&
                    grassDirtTransitions[4] != nullptr) {

                    SDL_RenderTexture(
                        renderer,
                        grassDirtTransitions[4],
                        nullptr,
                        &tileRect);
                }
                else if (grassAbove && grassRight &&
                    grassDirtTransitions[5] != nullptr) {

                    SDL_RenderTexture(
                        renderer,
                        grassDirtTransitions[5],
                        nullptr,
                        &tileRect);
                }
                else if (grassBelow && grassLeft &&
                    grassDirtTransitions[6] != nullptr) {

                    SDL_RenderTexture(
                        renderer,
                        grassDirtTransitions[6],
                        nullptr,
                        &tileRect);
                }
                else if (grassBelow && grassRight &&
                    grassDirtTransitions[7] != nullptr) {

                    SDL_RenderTexture(
                        renderer,
                        grassDirtTransitions[7],
                        nullptr,
                        &tileRect);
                }

                // Straight edges
                else if (grassAbove &&
                    grassDirtTransitions[0] != nullptr) {

                    SDL_RenderTexture(
                        renderer,
                        grassDirtTransitions[0],
                        nullptr,
                        &tileRect);
                }
                else if (grassBelow &&
                    grassDirtTransitions[1] != nullptr) {

                    SDL_RenderTexture(
                        renderer,
                        grassDirtTransitions[1],
                        nullptr,
                        &tileRect);
                }
                else if (grassLeft &&
                    grassDirtTransitions[2] != nullptr) {

                    SDL_RenderTexture(
                        renderer,
                        grassDirtTransitions[2],
                        nullptr,
                        &tileRect);
                }
                else if (grassRight &&
                    grassDirtTransitions[3] != nullptr) {

                    SDL_RenderTexture(
                        renderer,
                        grassDirtTransitions[3],
                        nullptr,
                        &tileRect);
                }
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
            tile == TILE_WATER ||
            tile == TILE_HOUSE;
}

bool TileMap::IsGrassTile(int row, int column) const {
    if (row < 0 ||
        row >= mapRows ||
        column < 0 ||
        column >= mapColumns) {
        return false;
    }
    return worldMap[row][column] == TILE_GRASS;
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
