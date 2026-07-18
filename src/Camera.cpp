#include "camera.h"
#include "TileMap.h"

Camera::Camera(int windowWidth, int windowHeight)
    : cameraX(0.0f),
cameraY(0.0f),
windowWidth(windowWidth),
windowHeight(windowHeight)
{

}

float Camera::GetX() const {
    return cameraX;
}

float Camera::GetY() const {
    return cameraY;
}

void Camera::Update(const SDL_FRect& playerRect,
        const TileMap& tileMap) {
    //Camera tracking player position safely
    cameraX = playerRect.x - windowWidth/2.0f;
    cameraY = playerRect.y - windowWidth/2.0f;


    if (cameraX < 0.0f) {
        cameraX = 0.0f;
    }
    if (cameraY < 0.0f) {
        cameraY = 0.0f;
    }
    if (cameraX > tileMap.GetWorldWidth() - windowWidth) {
        cameraX = tileMap.GetWorldWidth() - windowWidth;
    }
    if (cameraY > tileMap.GetWorldHeight() - windowHeight) {
        cameraY = tileMap.GetWorldHeight() - windowHeight;
    }
}