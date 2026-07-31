#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <string>

class AssetManager; // forward declaration

class TileMap {
    public:

    TileMap();
    ~TileMap();

    bool Initialize(SDL_Renderer* renderer,
        AssetManager& assetManager);

    void Render( SDL_Renderer* renderer,
        float cameraX,
        float cameraY) const;

    bool CheckCollision(const SDL_FRect& rect) const;
    int GetWorldWidth() const;
    int GetWorldHeight() const;

    bool LoadMap(const std::string& filename);

    private:

    static constexpr int TILE_EMPTY = 0;
    static constexpr int TILE_GRASS = 1;
    static constexpr int TILE_STONE = 2;
    static constexpr int TILE_TREE = 3;
    static constexpr int TILE_WATER = 4;
    static constexpr int TILE_HOUSE = 5;

    static constexpr int TILE_COUNT = 6;

    static constexpr int TILE_SIZE = 32;


    std::vector<std::vector<int> > worldMap;
    int mapColumns = 0;
    int mapRows = 0;


    SDL_Texture* tileTextures[TILE_COUNT];

    bool IsSolidTile(int tile) const;

    bool LoadTextures(SDL_Renderer* renderer,
        AssetManager& assetManager);
};
