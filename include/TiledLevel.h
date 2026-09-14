#pragma once

#include <string>
#include <vector>

struct MapLayer {
    std::string name;
    std::string type;

    int width = 0;
    int height = 0;

    std::vector<int> tileData;
};

class TiledLevel {
    public:
    TiledLevel();

    bool LoadMap(const std::string& fileName);

    int GetWidth() const;
    int GetHeight() const;

    int GetTileWidth() const;
    int GetTileHeight() const;

    private:

    int mapWidth;
    int mapHeight;

    int tileWidth;
    int tileHeight;

   std::vector<MapLayer> layers;
};
