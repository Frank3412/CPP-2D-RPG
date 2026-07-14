#include "Player.h"
#include "TileMap.h"


Player::Player() {
    texture = nullptr;
    rect ={0.0f,0.0f,64.0f,64.0f};
    speed = 200.0f;
}

void Player::SetTexture(SDL_Texture* newTexture) {
    texture = newTexture;
}
SDL_FRect& Player::GetRect() {
    return rect;
}
SDL_Texture* Player::GetTexture() const {
    return texture;
}

SDL_FRect Player::GetCollisionBox() const {
    SDL_FRect collisionBox;
    collisionBox.x = rect.x + 18.0f;
    collisionBox.y = rect.y + 42.0f;
    collisionBox.w = 28.0f;
    collisionBox.h = 20.0f;

    return collisionBox;
}

void Player::Update(float deltaTime, const TileMap& tileMap) {
    const bool* keyboardStates = SDL_GetKeyboardState(nullptr);
    float movementX = 0.0f;
    float movementY = 0.0f;

    // 1. Calculate the raw step distance for this exact frame
    if (keyboardStates[SDL_SCANCODE_UP]) {
        movementY -= speed * deltaTime;
    }
    if (keyboardStates[SDL_SCANCODE_DOWN]) {
        movementY += speed * deltaTime;
    }
    if (keyboardStates[SDL_SCANCODE_LEFT]) {
        movementX -= speed * deltaTime;
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT]) {
        movementX += speed * deltaTime;
    }

    // 2. Map boundary check *before* modifying position to prevent
    // sticking
    // Creating a temporary rectangle representing where the player
    // wants to go
    SDL_FRect nextRect = rect;
    nextRect.x += movementX;
    nextRect.y += movementY;

    // Hard boundary clamps on the temporary prediction path
    if (nextRect.x < 0.0f) {
        nextRect.x = 0.0f;
    }
    if (nextRect.y < 0.0f) {
        nextRect.y = 0.0f;
    }
    if (nextRect.x > tileMap.GetWorldWidth() - nextRect.w) {
        nextRect.x = tileMap.GetWorldWidth() - nextRect.w;
    }
    if (nextRect.y > tileMap.GetWorldHeight() - nextRect.h) {
        nextRect.y = tileMap.GetWorldHeight() - nextRect.h;
    }

    // 3. Axis-separated title collisions to maintain smooth wall
    // sliding
    // Test X Axis movement safely
    SDL_FRect testX = GetCollisionBox();
    testX.x += movementX;
    if (!tileMap.CheckCollision(testX)) {
        rect.x = nextRect.x;
    }

    // Test Y Axis movement safely
    SDL_FRect testY = GetCollisionBox();
    testY.y += movementY;
    if (!tileMap.CheckCollision(testY)) {
        rect.y = nextRect.y;
    }
}

void Player::Render(SDL_Renderer* renderer,
    float cameraX, float cameraY) {
    SDL_FRect screenRect = rect;
    screenRect.x -= cameraX;
    screenRect.y -= cameraY;

    SDL_RenderTexture(renderer, texture,
        nullptr, &screenRect);
}

