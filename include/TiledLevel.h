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

struct TileSetInfo {
    int firstGid = 0;

    std::string name;
    std::string image;

    int imageWidth = 0;
    int imageHeight = 0;

    int tileWidth = 0;
    int tileHeight = 0;

    int columns = 0;
    int tileCount = 0;
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
    std::vector<TileSetInfo> tilesets;
};
