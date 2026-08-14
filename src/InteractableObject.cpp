#include "InteractableObject.h"

InteractableObject::InteractableObject(
    float x,
    float y,
    float width,
    float height)
        : rect{x,y,width,height},
texture(nullptr)
{

}

const SDL_FRect& InteractableObject::GetRect() const {
    return rect;
}

void InteractableObject::SetTexture(SDL_Texture* texture) {
    this->texture = texture;
}

void InteractableObject::Render(
    SDL_Renderer* renderer,
    float cameraX,
    float cameraY) const {

    if (texture == nullptr) {
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