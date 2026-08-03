#pragma once

#include <SDL3/SDL.h>

class TileMap;

class Player {
public:
    Player();
    void SetTexture(SDL_Texture* newTexture);

    // We will now pass the collision map bounds/checking function context
    // to Update
    void Update(float deltaTime, const TileMap& tileMap);
    void Render(SDL_Renderer* renderer,
        float cameraX,
        float cameraY);
    const SDL_FRect& GetRect() const;


private:
    SDL_FRect GetCollisionBox() const;

    SDL_Texture* texture;
    SDL_FRect rect;
    float speed;

};
