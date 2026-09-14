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

    if (mapData.contains("layers") &&
        mapData["layers"].is_array()) {

        for (const auto& layerData : mapData["layers"]) {
            MapLayer layer;

            if (layerData.contains("name")) {
                layer.name = layerData["name"].get<std::string>();
            }
            if (layerData.contains("type")) {
                layer.type = layerData["type"].get<std::string>();
            }
            if (layerData.contains("width")) {
                layer.name = layerData["width"].get<std::string>();
            }
            if (layerData.contains("height")) {
                layer.name = layerData["height"].get<std::string>();
            }
            if (layerData.contains("data")&&
                layerData["data"].is_array()) {
                layer.tileData =
                    layerData["data"].get<std::vector<int>>();
            }
            layers.push_back(std::move(layer));
        }
    }

    std::cout << "Layers: "
    << layers.size()
    << '\n';

    for (const MapLayer& layer:layers) {
        std::cout << "Layer: "
        << layer.name
        << layer.type
        << " | Size: "
        << layer.width
        << " x "
        << layer.height
        << " | Tiles: "
        << layer.tileData.size()
        << '\n';
    }

    std::cout
    << "Loaded Tiled map: "
    << mapWidth
    << " x "
    << mapHeight
    << " tiles\n";

    std::cout
    << "Tile size: "
    << tileWidth
    << " x "
    << tileHeight
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