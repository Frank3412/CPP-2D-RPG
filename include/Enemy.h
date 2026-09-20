#pragma once

#include <SDL3/SDL.h>

class Enemy {
public:
    Enemy(float x, float y, int health);

    void SetTexture(SDL_Texture* texture);

    void Render(
        SDL_Renderer* renderer,
        float cameraX,
        float cameraY) const;

    const SDL_FRect& GetRect() const;
    SDL_FRect GetCollisionBox() const;

    int GetHealth() const;
    void TakeDamage(int damage);

private:
    SDL_Texture* texture;
    SDL_FRect rect;
    int health;
};
