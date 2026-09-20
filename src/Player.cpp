#include "Player.h"
#include "TileMap.h"


Player::Player() {

    walkLeftTexture = nullptr;
    walkDownTexture = nullptr;
    walkUpTexture = nullptr;
    walkRightTexture = nullptr;

    idleLeftTexture = nullptr;
    idleDownTexture = nullptr;
    idleUpTexture = nullptr;
    idleRightTexture = nullptr;

    direction = Direction::Down;
    animationState = AnimationState::Idle;
    currentFrame = 0;

    animationTimer = 0.0f;
    animationSpeed = 0.15f;
    idleAnimationSpeed = 0.30f;

    rect ={0.0f,0.0f,64.0f,64.0f};
    speed = 200.0f;
}

void Player::SetAnimationTextures(
    SDL_Texture* walkLeft,
    SDL_Texture* walkDown,
    SDL_Texture* walkUp,
    SDL_Texture* walkRight,
    SDL_Texture* idleLeft,
    SDL_Texture* idleDown,
    SDL_Texture* idleUp,
    SDL_Texture* idleRight) {

    walkLeftTexture = walkLeft;
    walkDownTexture = walkDown;
    walkUpTexture = walkUp;
    walkRightTexture = walkRight;

    idleLeftTexture = idleLeft;
    idleDownTexture = idleDown;
    idleUpTexture = idleUp;
    idleRightTexture = idleRight;

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

void Player::Update(
    float deltaTime,
    const TileMap& tileMap,
    const std::vector<SDL_FRect>& solidObjects) {

    const bool* keyboardStates = SDL_GetKeyboardState(nullptr);
    float movementX = 0.0f;
    float movementY = 0.0f;

    // 1. Calculate movement
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
        animationState = AnimationState::Walking;
    }
    else {
        if (animationState == AnimationState::Walking) {
            currentFrame = 0;
            animationTimer = 0.0f;
        }
        animationState = AnimationState::Idle;
    }

    // 2. Update animation
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
        animationTimer += deltaTime;
        if (animationTimer >= idleAnimationSpeed) {
            animationTimer -= idleAnimationSpeed;
            currentFrame++;
            if (currentFrame >= 4) {
                currentFrame = 0;
            }
        }
    }

    // 3. Calculate the player's next position

    SDL_FRect nextRect = rect;
    nextRect.x += movementX;
    nextRect.y += movementY;

    // 4. Keep the player inside the current world boundaries
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

    // 5. Object collision only
    //
    // Tile collision is intentionally disabled for now
    // The new Tiled level will eventually provide its own
    // collision information

    SDL_FRect testX = GetCollisionBox();
    testX.x += movementX;

    bool blockedX = false;

    for (const SDL_FRect& solidObject:solidObjects) {
        if (SDL_HasRectIntersectionFloat(&testX, &solidObject)) {
            blockedX = true;
            break;
        }
    }

    if (!blockedX) {
        rect.x = nextRect.x;
    }

    // Test Y axis
    SDL_FRect testY = GetCollisionBox();
    testY.y += movementY;

    bool blockedY = false;

   for (const SDL_FRect& solidObject:solidObjects) {
       if (SDL_HasRectIntersectionFloat(&testY, &solidObject)) {
           blockedY = true;
           break;
       }
   }
    if (!blockedY) {
        rect.y = nextRect.y;
    }
}

void Player::Render(SDL_Renderer* renderer,
    float cameraX, float cameraY) {

    SDL_FRect screenRect = rect;
    screenRect.x -= cameraX;
    screenRect.y -= cameraY;

    SDL_Texture* currentTexture = idleDownTexture;

    if (animationState == AnimationState::Walking) {

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
    }
    else {
        switch (direction) {
            case Direction::Left:
                currentTexture = idleLeftTexture;
                break;
            case Direction::Right:
                currentTexture = idleRightTexture;
                break;
            case Direction::Up:
                currentTexture = idleUpTexture;
                break;
            case Direction::Down:
                currentTexture = idleDownTexture;
                break;
        }
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

