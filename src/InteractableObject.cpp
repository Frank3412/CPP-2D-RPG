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

SDL_FRect InteractableObject::GetInteractionRect() const {
    SDL_FRect interactionRect;

    interactionRect.x = rect.x - 16.0f;
    interactionRect.y = rect.y - 16.0f;
    interactionRect.w = 64.0f;
    interactionRect.h = 64.0f;

    return interactionRect;
}

bool InteractableObject::IsInInteractionRange(
    const SDL_FRect& otherRect) const {

    SDL_FRect interactionRect = GetInteractionRect();

    return
    otherRect.x < interactionRect.x + interactionRect.w &&
        otherRect.x + otherRect.w > interactionRect.x &&
            otherRect.y < interactionRect.y + interactionRect.h &&
                otherRect.y + otherRect.h > interactionRect.y;
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