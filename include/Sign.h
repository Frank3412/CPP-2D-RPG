#pragma once

#include "InteractableObject.h"

class Sign : public InteractableObject {
    public:
    Sign(float x, float y);

    void Interact() override;
};