#pragma once

#include <SDL3/SDL.h>

class InteractableObject {
    public:
    InteractableObject(float x, float y, float width, float height);

    const SDL_FRect& GetRect() const;

    SDL_FRect GetInteractionRect() const;

    bool IsInInteractionRange(const SDL_FRect& otherRect) const;

    void SetTexture(SDL_Texture* texture);

    void Render(SDL_Renderer* renderer,
        float CameraX,
        float CameraY) const;

    private:
    SDL_FRect rect;
    SDL_Texture* texture;
};