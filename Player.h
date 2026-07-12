#pragma once

#include <SDL3/SDL.h>

class Player {
public:
    Player();
    void SetTexture(SDL_Texture* newTexture);
    SDL_Texture* GetTexture() const;

    // We will now pass the collision map bounds/checking function context
    // to Update
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer,
        float cameraX,
        float cameraY);
    SDL_FRect& GetRect();
    SDL_FRect GetCollisionBox() const;

private:
    SDL_Texture* texture;
    SDL_FRect rect;
    float speed;
};