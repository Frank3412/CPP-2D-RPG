#include "NPC.h"

NPC::NPC(
    float x,
    float y,
    const DialogueData& dialogue)
        : InteractableObject(x,y,64.0f,64.0f),
dialogue(dialogue),
collisionBox{
    x+22.0f,
y+ 42.0f,
20.0f,
40.0f
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