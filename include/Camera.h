#pragma once

#include <SDL3/SDL.h>

class TileMap; //forward declaration of tileMap class

class Camera {
public:
    Camera(int windowWidth, int windowHeight);

    void Update(const SDL_FRect& playerRect,
        const TileMap& tileMap);

    float GetX() const;
    float GetY() const;

private:
    float cameraX;
    float cameraY;
    int windowWidth;
    int windowHeight;
};