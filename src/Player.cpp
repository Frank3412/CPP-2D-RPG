#include "Player.h"
#include "TileMap.h"


Player::Player() {

    walkLeftTexture = nullptr;
    walkDownTexture = nullptr;
    walkUpTexture = nullptr;
    walkRightTexture = nullptr;

    direction = Direction::Down;
    currentFrame = 0;

    animationTimer = 0.0f;
    animationSpeed = 0.15f;

    rect ={0.0f,0.0f,64.0f,64.0f};
    speed = 200.0f;
}

void Player::SetWalkingTextures(
    SDL_Texture* left,
    SDL_Texture* down,
    SDL_Texture* up,
    SDL_Texture* right) {

    walkLeftTexture = left;
    walkDownTexture = down;
    walkUpTexture = up;
    walkRightTexture = right;
}
const SDL_FRect& Player::GetRect() const {
    return rect;
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
        direction = Direction::Up;
    }
    if (keyboardStates[SDL_SCANCODE_DOWN]) {
        movementY += speed * deltaTime;
        direction = Direction::Down;
    }
    if (keyboardStates[SDL_SCANCODE_LEFT]) {
        movementX -= speed * deltaTime;
        direction = Direction::Left;
    }
    if (keyboardStates[SDL_SCANCODE_RIGHT]) {
        movementX += speed * deltaTime;
        direction = Direction::Right;
    }

    bool isMoving = (movementX != 0.0f || movementY != 0.0f);

    if (isMoving) {
        animationTimer += deltaTime;

        if (animationTimer >= animationSpeed) {
            animationTimer -= animationSpeed;
            currentFrame++;

            if (currentFrame >= 4) {
                currentFrame = 0;
            }
        }
    }
    else {
        currentFrame = 0;
        animationTimer = 0.0f;
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

    SDL_Texture* currentTexture = walkDownTexture;

    switch (direction) {
        case Direction::Left:
            currentTexture = walkLeftTexture;
            break;
        case Direction::Right:
            currentTexture = walkRightTexture;
            break;
        case Direction::Up:
            currentTexture = walkUpTexture;
            break;
        case Direction::Down:
            currentTexture = walkDownTexture;
            break;
    }

    SDL_FRect sourceRect{
    static_cast<float>(currentFrame*64),
        0.0f,
        64.0f,
        64.0f
    };

    SDL_RenderTexture(renderer, currentTexture,
        &sourceRect, &screenRect);
}

