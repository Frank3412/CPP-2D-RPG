#pragma once

#include "InteractableObject.h"
#include "DialogueData.h"

class Sign : public InteractableObject {
    public:
    Sign(float x, float y);

    void Interact() override;

    const DialogueData& GetDialogue() const;

    private:
    DialogueData dialogue;

    DialogueData CreateDefaultDialogue() const;
};