#include "Sign.h"

Sign::Sign(
    float x,
    float y)
    : InteractableObject(x,y, 32.0f,32.0f),
dialogue(CreateDefaultDialogue())
{
}

void Sign::Interact() {
    if (!dialogue.lines.empty()) {
        SDL_Log("%s", dialogue.lines[0].c_str());
    }
}

const DialogueData& Sign::GetDialogue() const {
    return dialogue;
}

DialogueData Sign::CreateDefaultDialogue() const {
    return DialogueData{
        {
            "Welcome to Monster Quest!",
            "There is much to discover in this world.",
            "Your adventure is just beginning."
        }
    };
}