#include "TiledLevel.h"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TiledLevel::TiledLevel()
    : mapWidth(0),
mapHeight(0),
tileWidth(0),
tileHeight(0)
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
            layers.push_back(layer);
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