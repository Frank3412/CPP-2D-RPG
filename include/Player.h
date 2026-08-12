#pragma once

#include <SDL3/SDL.h>

class TileMap;

class Player {
public:
    Player();
    void SetAnimationTextures(
        SDL_Texture* walkLeft,
        SDL_Texture* walkDown,
        SDL_Texture* walkUp,
        SDL_Texture* walkRight,
        SDL_Texture* idleLeft,
        SDL_Texture* idleDown,
        SDL_Texture* idleUp,
        SDL_Texture* idleRight);

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

    enum class AnimationState {
        Idle,
          Walking
      };

    Direction direction;
    AnimationState animationState;

    int currentFrame;

    float animationTimer;
    float animationSpeed; // Walking
    float idleAnimationSpeed;  // Idle

    SDL_Texture* walkLeftTexture;
    SDL_Texture* walkDownTexture;
    SDL_Texture* walkUpTexture;
    SDL_Texture* walkRightTexture;

    SDL_Texture* idleLeftTexture;
    SDL_Texture* idleDownTexture;
    SDL_Texture* idleUpTexture;
    SDL_Texture* idleRightTexture;

    SDL_FRect rect;
    float speed;

};
