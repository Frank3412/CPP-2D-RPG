#pragma once

#include <SDL3/SDL.h>

class TileMap {
    public:

    TileMap();
    ~TileMap();

    bool Initialize(SDL_Renderer* renderer);

    void Render( SDL_Renderer* renderer,
        float cameraX,
        float cameraY) const;

    bool CheckCollision(const SDL_FRect& rect) const;
    int GetWorldWidth() const;
    int GetWorldHeight() const;


    private:

    static constexpr int TILE_EMPTY = 0;
    static constexpr int TILE_GRASS = 1;
    static constexpr int TILE_STONE = 2;
    static constexpr int TILE_TREE = 3;
    static constexpr int TILE_WATER = 4;
    static constexpr int TILE_HOUSE = 5;

    static constexpr int TILE_COUNT = 6;

    static constexpr int TILE_SIZE = 32;

    static constexpr int MAP_ROWS = 60;
    static constexpr int MAP_COLUMNS = 100;

    static constexpr int WINDOW_WIDTH = 1000;
    static constexpr int WINDOW_HEIGHT = 800;

    static constexpr int worldMap[MAP_ROWS][MAP_COLUMNS]={
        {1,0,2,2,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,2,0,0,0,0,1,1,1,1},

        {1,4,1,2,1,3,2,0,1,0,0,1,0,1,0,0,0,0,0,1,0,2,0,4,4,4,1,0,2,0,1},

        {1,1,1,3,1,1,1,2,1,3,1,3,0,3,1,2,2,2,2,1,0,0,0,3,0,4,4,4,4,0,1},

        {1,0,1,0,1,0,1,0,1,0,0,1,2,0,1,2,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1},
    };

    static constexpr int WORLD_WIDTH = MAP_COLUMNS * TILE_SIZE;
    static constexpr int WORLD_HEIGHT = MAP_ROWS * TILE_SIZE;

    SDL_Texture* tileTextures[TILE_COUNT];

    bool IsSolidTile(int tile) const;

    bool LoadTextures(SDL_Renderer* renderer);
};
