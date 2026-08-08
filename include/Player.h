#pragma once

#include <SDL3/SDL.h>

class TileMap;

class Player {
public:
    Player();
    void SetWalkingTextures(
        SDL_Texture* left,
        SDL_Texture* down,
        SDL_Texture* up,
        SDL_Texture* right);

    // We will now pass the collision map bounds/checking function context
    // to Update
    void Update(float deltaTime, const TileMap& tileMap);
    void Render(SDL_Renderer* renderer,
        float cameraX,
        float cameraY);
    const SDL_FRect& GetRect() const;


private:
    SDL_FRect GetCollisionBox() const;

    enum class Direction {
        Down,
          Up,
          Left,
          Right
      };

    Direction direction;
    int currentFrame;

    float animationTimer;
    float animationSpeed;

    SDL_Texture* walkLeftTexture;
    SDL_Texture* walkDownTexture;
    SDL_Texture* walkUpTexture;
    SDL_Texture* walkRightTexture;

    SDL_FRect rect;
    float speed;

};
