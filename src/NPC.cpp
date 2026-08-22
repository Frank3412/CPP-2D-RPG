#include "NPC.h"

NPC::NPC(
    float x,
    float y,
    const DialogueData& dialogue)
        : InteractableObject(x,y,32.0f,32.0f),
dialogue(dialogue),
collisionBox{
    x+6.0f,
y+ 8.0f,
20.0f,
20.0f
},
interactionRect{
x-24.0f,
y-24.0f,
80.0f,
80.0f}
{
}

void NPC::Interact() {
}

const DialogueData& NPC::GetDialogue() const {
    return dialogue;
}

SDL_FRect NPC::GetCollisionBox() const {
    return collisionBox;
}

bool NPC::IsInNPCInteractionRange(
    const SDL_FRect& playerRect) const {

    return SDL_HasRectIntersectionFloat(
        &playerRect,
        &interactionRect);
}