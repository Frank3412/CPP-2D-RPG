#pragma once

#include "InteractableObject.h"
#include "DialogueData.h"

class NPC: public InteractableObject {
public:
    NPC(float x, float y, const DialogueData& dialogue);

    void Interact() override;

    const DialogueData& GetDialogue() const;

    SDL_FRect GetCollisionBox() const;

    bool IsInNPCInteractionRange(const SDL_FRect& playerRect) const;

private:
    DialogueData dialogue;
    SDL_FRect collisionBox;
    SDL_FRect interactionRect;
};