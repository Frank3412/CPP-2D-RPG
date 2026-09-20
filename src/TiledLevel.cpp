#include "TiledLevel.h"

#include "AssetManager.h"
#include "Constants.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TiledLevel::TiledLevel()
    : mapWidth(0),
mapHeight(0),
tileWidth(0),
tileHeight(0),
mapDirectory(),
tilesetTexture(nullptr)
{
}

bool TiledLevel::LoadMap(const std::string& filename) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr
        << "Failed to open Tiled map: "
        << filename
        << '\n';

        return false;
    }

    json mapData;

    try {
        file >> mapData;
    }
    catch (const json::parse_error& error) {

        std::cerr
        << "Failed to parse Tiled map: "
        << error.what()
        << '\n';

        return false;
    }

    if (!mapData.contains("width") ||
        !mapData.contains("height") ||
        !mapData.contains("tilewidth") ||
        !mapData.contains("tileheight")) {

        std::cerr
        << "Tiled map is missing required map data.\n";
        return false;
    }

    mapWidth = mapData["width"].get<int>();
    mapHeight = mapData["height"].get<int>();

    tileWidth = mapData["tilewidth"].get<int>();
    tileHeight = mapData["tileheight"].get<int>();

    // Store the directory containing the .tmj file
    std::filesystem::path mapPath(filename);
    mapDirectory = mapPath.parent_path().string();

    layers.clear();
    tilesets.clear();

    // Load map layers.
    if (mapData.contains("layers") &&
        mapData["layers"].is_array()) {

        for (const auto& layerData : mapData["layers"]) {

            MapLayer layer;

            if (layerData.contains("name") &&
                layerData["name"].is_string()) {
                layer.name = layerData["name"].get<std::string>();
            }
            if (layerData.contains("type") &&
                layerData["type"].is_string()) {
                layer.type = layerData["type"].get<std::string>();
            }
            if (layerData.contains("width") &&
                layerData["width"].is_number_integer()) {
                layer.width = layerData["width"].get<int>();
            }
            if (layerData.contains("height") &&
                layerData["height"].is_number_integer()) {
                layer.height = layerData["height"].get<int>();
            }
            if (layerData.contains("data")&&
                layerData["data"].is_array()) {
                layer.tileData =
                    layerData["data"].get<std::vector<int>>();
            }
            layers.push_back(std::move(layer));
        }
    }

    // Load embedded tilesets.
    if (mapData.contains("tilesets") &&
        mapData["tilesets"].is_array()) {

        for (const auto& tilesetData : mapData["tilesets"]) {
            TileSetInfo tileset;

            if (tilesetData.contains("firstgid") &&
                tilesetData["firstgid"].is_number_integer()) {
                tileset.firstGid = tilesetData["firstgid"].get<int>();
            }
            if (tilesetData.contains("name") &&
               tilesetData["name"].is_string()) {
                tileset.name = tilesetData["name"].get<std::string>();
               }
            if (tilesetData.contains("image") &&
               tilesetData["image"].is_string()) {
                tileset.image = tilesetData["image"].get<std::string>();
               }
            if (tilesetData.contains("imagewidth") &&
               tilesetData["imagewidth"].is_number_integer()) {
                tileset.imageWidth = tilesetData["imagewidth"].get<int>();
               }
            if (tilesetData.contains("imageheight") &&
               tilesetData["imageheight"].is_number_integer()) {
                tileset.imageHeight = tilesetData["imageheight"].get<int>();
               }
            if (tilesetData.contains("tilewidth") &&
               tilesetData["tilewidth"].is_number_integer()) {
                tileset.tileWidth = tilesetData["tilewidth"].get<int>();
               }
            if (tilesetData.contains("tileheight") &&
               tilesetData["tileheight"].is_number_integer()) {
                tileset.tileHeight = tilesetData["tileheight"].get<int>();
               }
            if (tilesetData.contains("columns") &&
               tilesetData["columns"].is_number_integer()) {
                tileset.columns = tilesetData["columns"].get<int>();
               }
            if (tilesetData.contains("tilecount") &&
               tilesetData["tilecount"].is_number_integer()) {
                tileset.tileCount = tilesetData["tilecount"].get<int>();
               }
            if (tilesetData.contains("margin") &&
               tilesetData["margin"].is_number_integer()) {
                tileset.margin = tilesetData["margin"].get<int>();
               }
            if (tilesetData.contains("spacing") &&
               tilesetData["spacing"].is_number_integer()) {
                tileset.spacing = tilesetData["spacing"].get<int>();
               }
            tilesets.push_back(tileset);
        }
    }

    std::cout << "Layers: " << layers.size() << '\n';

    for (const MapLayer& layer:layers) {
        std::cout << "Layer: " << layer.name
        << " | Type: " << layer.type
        << " | Size: " << layer.width
        << " x " << layer.height
        << " | Tiles: " << layer.tileData.size()
        << '\n';
    }

    std::cout << "Tilesets: " << tilesets.size() << '\n';

    for (const TileSetInfo& tileset:tilesets) {
        std::cout << "Tileset: " << tileset.name << '\n';
        std::cout << "Image: " << tileset.image << '\n';
        std::cout << "Image size: " << tileset.imageWidth
        << " x " << tileset.imageHeight
        << '\n';
        std::cout << "Tile size: " << tileset.tileWidth
        << " x " << tileset.tileHeight
        << '\n';
        std::cout << "Columns: " << tileset.columns << '\n';
        std::cout << "Tile count: " << tileset.tileCount << '\n';
        std::cout << "First GID: " << tileset.firstGid << '\n';
    }

    std::cout << "Loaded Tiled map: " << mapWidth
    << " x " << mapHeight
    << " tiles\n";

    std::cout << "Tile size: " << tileWidth
    << " x " << tileHeight
    << '\n';

    return true;
}

bool TiledLevel::Initialize(
    SDL_Renderer* renderer,
    AssetManager& assetManager) {

    if (tilesets.empty()) {

        SDL_Log("TiledLevel contains no tilesets.");
        return false;
    }

    // Stage C currently supports one tileset
    const TileSetInfo& tileset = tilesets.front();

    if (tileset.image.empty()) {
        SDL_Log("TiledLevel tileset does not contain an image.");
        return false;
    }

    std::filesystem::path imagePath =
        std::filesystem::path(mapDirectory)
    / tileset.image;

    imagePath = imagePath.lexically_normal();

    tilesetTexture =
        assetManager.LoadTexture(
            renderer,
            imagePath.string().c_str());

    if (!tilesetTexture) {

        SDL_Log("Failed to load Tiled tileset image: %s",
            imagePath.string().c_str());
        return false;
    }

    return true;

}

void TiledLevel::Render(
    SDL_Renderer* renderer,
    float cameraX,
    float cameraY) const {

    if (tilesetTexture == nullptr ||
        tilesets.empty()) {
        return;
    }

    const TileSetInfo& tileset = tilesets.front();

    if (tileset.columns <= 0 ||
        tileset.tileWidth <= 0 ||
        tileset.tileHeight <= 0) {
        return;
    }

    for (const MapLayer& layer: layers) {

        if (layer.type != "tilelayer") {
            continue;
        }
        if (layer.width <= 0 ||
            layer.height <= 0) {
            continue;
        }

        const int firstColumn =
            std::max(0, static_cast<int>(cameraX/tileWidth));

        const int firstRow =
            std::max(0, static_cast<int>(cameraY/tileHeight));

        const int visibleColumns =
            (WINDOW_WIDTH/tileWidth)+2;

        const int visibleRows =
            (WINDOW_HEIGHT/tileHeight)+2;

        const int lastColumn =
            std::min(layer.width, firstColumn+visibleColumns);

        const int lastRow = std::min(layer.height,
            firstRow + visibleRows);

        for (int row = firstRow; row < lastRow; ++row) {

            for (int column = firstColumn; column < lastColumn; ++column) {

                const int dataIndex = row * layer.width + column;

                if (dataIndex < 0 || dataIndex >=
                    static_cast<int>(layer.tileData.size())) {
                    continue;
                }

                const int gid = layer.tileData[dataIndex];

                // GID 0 means empty tile.
                if (gid ==0) {
                    continue;
                }

                // Stage C currently supports
                // normal, non-flipped GIDs
                if (gid < tileset.firstGid) {
                    continue;
                }

                const int localTiledID = gid - tileset.firstGid;

                if (localTiledID < 0 || localTiledID >=
                    tileset.tileCount) {
                    continue;
                }

                const int sourceColumn =
                    localTiledID % tileset.columns;

                const int sourceRow =
                    localTiledID / tileset.columns;

                const float sourceX =
                    static_cast<float>(
                        tileset.margin + sourceColumn * (
                            tileset.tileWidth+tileset.spacing));

                const float sourceY =
                    static_cast<float>(tileset.margin + sourceRow *
                        (tileset.tileHeight+tileset.spacing));

                SDL_FRect sourceRect{
                sourceX,
                sourceY,
                static_cast<float>(tileset.tileWidth),
                static_cast<float>(tileset.tileHeight)};

                SDL_FRect destinationRect{
                static_cast<float>(column*tileWidth)-cameraX,
                static_cast<float>(row*tileHeight)-cameraY,
                static_cast<float>(tileWidth),
                static_cast<float>(tileHeight)};

                SDL_RenderTexture(
                    renderer,
                    tilesetTexture,
                    &sourceRect,
                    &destinationRect);
            }
        }
    }
}

int TiledLevel::GetWidth() const {
    return mapWidth;
}

int TiledLevel::GetHeight() const {
    return mapHeight;
}

int TiledLevel::GetTileWidth() const {
    return tileWidth;
}

int TiledLevel::GetTileHeight() const {
    return tileHeight;
}