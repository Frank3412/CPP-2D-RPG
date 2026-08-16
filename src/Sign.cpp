#include "Sign.h"

Sign::Sign(
    float x,
    float y,
    const std::string& message)
    : InteractableObject(x,y, 32.0f,32.0f),
message(message)
{

}

void Sign::Interact() {
    SDL_Log("%s", message.c_str());
}

const std::string& Sign::GetMessage() const {
    return message;
}