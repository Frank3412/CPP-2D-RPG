#include "Enemy.h"

Enemy::Enemy (float x, float y, int health)
    : texture(nullptr),
rect{x,y, 64.0f, 64.0f},
health(health) {

}

void Enemy::SetTexture(SDL_Texture* newTexture) {
    texture = newTexture;
}

void Enemy::Render(
    SDL_Renderer* renderer,
    float cameraX,
    float cameraY) const {

    if (!texture) {
        return;
    }

    SDL_FRect screenRect = rect;

    screenRect.x -= cameraX;
    screenRect.y -= cameraY;

    SDL_RenderTexture(
        renderer,
        texture,
        nullptr,
        &screenRect);
}

const SDL_FRect& Enemy::GetRect() const {
    return rect;
}

SDL_FRect Enemy::GetCollisionBox() const {

    SDL_FRect collisionBox;

    collisionBox.x = rect.x+14.0f;
    collisionBox.y = rect.y + 30.0f;
    collisionBox.w = 36.0f;
    collisionBox.h = 24.0f;

    return collisionBox;
}

int Enemy::GetHealth() const {
    return health;
}

void Enemy::TakeDamage(int damage) {

    if (damage <= 0) {
        return;
    }

    health -= damage;

    if (health < 0) {
        health = 0;
    }
}
