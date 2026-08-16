#pragma once

#include "InteractableObject.h"
#include <string>

class Sign : public InteractableObject {
    public:
    Sign(float x, float y, const std::string& message);

    void Interact() override;

    const std::string& GetMessage() const;

    private:
    std::string message;
};